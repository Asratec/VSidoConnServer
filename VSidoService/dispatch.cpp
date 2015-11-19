/*
Copyright (c) 2015, Asratec Corp.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of VSidoConnServer nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "dispatch.hpp"
#include "ws_response.hpp"
#include "rs_response.hpp"
#include "cmd_common.hpp"
#include "cmd_parser.hpp"
using namespace VSido;

#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <thread>
#include <tuple>
#include <atomic>
using namespace std;


#include <sys/time.h>

#include "debug.h"

static mutex ackMtx;
static condition_variable ackCv;

static mutex reqMtx;
static condition_variable reqCv;

static mutex dataMtx;



static long long globalSendTime = 0;

/// mili sec
static const int iConstVSidoCommTimeout = 100;

static const int iConstRequestWaitTimeout = 1;


static const int iConstBufferCommandCounter = 2;

static list<tuple<string,shared_ptr<WSResponse>>> uniq_wsReq;
static list<tuple<string,shared_ptr<RSResponse>>> uniq_rsReq;




/** コンストラクタ
* @param send VSidoと繋がるURATの送信
*/
Dispatcher::Dispatcher()
:_wsReq(uniq_wsReq)
,_rsReq(uniq_rsReq)
{
}


#include <fstream>
#include <string>
#include <iostream>

#define logReq(...)
#define logRes(...)

#if 0
static int gLogID = 0;
extern string exec(string cmd);
static void logReq(const string &req)
{
	if(0 == gLogID++ % 100 )
	{
        string shell("rm -f /mnt/vsido/log/*.log");
        int ret = ::system(shell.c_str());
	}
	string logName = "/mnt/vsido/log/";
	char buffer [10] ={0};
	snprintf(buffer,sizeof(buffer)-1,"%08d",gLogID);
	logName += string(buffer);
	logName += ".req.log";
	std::ofstream out(logName,std::ios_base::app | std::ios_base::out);
	out << req;
	out <<  endl;
    out.close();	
}
static void logRes(const string &res)
{
	string logName = "/mnt/vsido/log/";
	char buffer [10] ={0};
	snprintf(buffer,sizeof(buffer)-1,"%08d",gLogID);
	logName += string(buffer);
	logName += ".res.log";
	std::ofstream out(logName,std::ios_base::app | std::ios_base::out);
	out << res;
    out.close();
}
#endif


/** 要求を追加する
* @param req 要求
* @param res WebSocket返事先
*/
void Dispatcher::addRequest(const string req,shared_ptr<WSResponse> res)
{
	logReq(req);
	DUMP_VAR_DETAILS(req);
	{
		lock_guard<mutex> lock(dataMtx);
		this->_wsReq.push_back(make_tuple(req,res));
	}
	DUMP_VAR_DETAILS(req);
	reqCv.notify_all();
	
}

/** 要求を追加する
* @param req 要求
* @param res TCPSocket返事先
*/
void Dispatcher::addRequest(const string req,shared_ptr<RSResponse> res)
{
	logReq(req);
	DUMP_VAR_DETAILS(req);
	
	{
		lock_guard<mutex> lock(dataMtx);
		this->_rsReq.push_back(make_tuple(req,res));
	}
	DUMP_VAR_DETAILS(req);
	reqCv.notify_all();
}






/** 分配スレッド本体
* @return None
*/
void Dispatcher::operator()()
{
	while(true)
	{
		/// send
		bool wait = false;
		{
			lock_guard<mutex> lock(dataMtx);
			wait =  (this->_wsReq.empty()) &&  (this->_rsReq.empty());
		}
		if(wait)
		{
			DUMP_VAR_DETAILS(&reqMtx);
			DUMP_VAR_DETAILS(&reqCv);
			unique_lock<mutex> lck(reqMtx);
			auto waitRet = reqCv.wait_for(lck,std::chrono::milliseconds(iConstRequestWaitTimeout));
			if(cv_status::timeout==waitRet)
			{
				//printf("%s,%d timeout\n",__FILE__,__LINE__);
				continue;
			}
		}
		/// try send websocket request.
		trySendWSReq();
		/// try send tpc socket request.
		trySendRSReq();

		this_thread::yield();
	}
}

static mutex dataResMtx;
static map<int,shared_ptr<WSResponse> > uniq_nb_ws;
static map<int,shared_ptr<RSResponse> > uniq_nb_rs;

/// no block request object will be kept until message is recieved.
static map<int,shared_ptr<JSONRequest> > uniq_nb_JSONReq;

static atomic<int> gLastRequestUID(0);


void Dispatcher::trySendWSReq(void)
{
	tuple<string,shared_ptr<WSResponse>> req;

	{
		lock_guard<mutex> lock(dataMtx);
		if(false == this->_wsReq.empty())
		{
			req = this->_wsReq.back();
		}
		else
		{
			return;
		}
	}
	JSONRequestParser parser(std::get<0>(req));
	DUMP_VAR_DETAILS(&parser);
	auto errMsg = parser.errorMsg();
	if(errMsg.empty())
	{
		auto request = parser.create();
		auto result = request->exec();
		auto wsRes = std::get<1>(req);
		if(result.empty())
		{
			/// noblock message.
			lock_guard<mutex> lock(dataResMtx);
			auto itHolder = uniq_nb_ws.find(request->uid());
			// send timeout to old one
			if(itHolder != uniq_nb_ws.end())
			{
				itHolder->second->ackMiss();
			}
			// add new one.
			uniq_nb_ws[request->uid()] = wsRes;
			uniq_nb_JSONReq[request->uid()] = request;
			gLastRequestUID = request->uid();
		}
		else
		{
			wsRes->ack(result);
		}
	}
	else
	{
		picojson::object _res;
		picojson::value resValue(_res);
		_res["type"] = picojson::value(string("error"));
		_res["detail"] = picojson::value(errMsg);
		auto rsRes = std::get<1>(req);
		rsRes->ack(resValue.serialize());
	}
	// clear up
	{
		// last one is sent,so pop it.
		lock_guard<mutex> lock(dataMtx);
		this->_wsReq.pop_back();
		// if there are more buffered than iConstBufferCommandCounter
		// sent busy and pop,from front side.
		while( iConstBufferCommandCounter < this->_wsReq.size())
		{
			auto rsRes = std::get<1>(this->_wsReq.front());
			rsRes->ackBusy();
			this->_wsReq.pop_front();
		}
	}
}
void Dispatcher::trySendRSReq(void)
{
	tuple<string,shared_ptr<RSResponse>> req;

	{
		lock_guard<mutex> lock(dataMtx);
		if(false == this->_rsReq.empty())
		{
			req = this->_rsReq.back();
		}
		else
		{
			return;
		}
	}
	JSONRequestParser parser(std::get<0>(req));
	DUMP_VAR_DETAILS(&parser);
	auto errMsg = parser.errorMsg();
	if(errMsg.empty())
	{
		auto request = parser.create();
		auto result = request->exec();
		auto rsRes = std::get<1>(req);
		if(result.empty())
		{
			/// noblock message.
			lock_guard<mutex> lock(dataResMtx);
			auto itHolder = uniq_nb_rs.find(request->uid());
			// send timeout to old one
			if(itHolder != uniq_nb_rs.end())
			{
				itHolder->second->ackMiss();
			}
			// add new one.
			uniq_nb_rs[request->uid()] = rsRes;
			uniq_nb_JSONReq[request->uid()] = request;
			gLastRequestUID = request->uid();
		}
		else
		{
			rsRes->ack(result);
		}
	}
	else
	{
		picojson::object _res;
		picojson::value resValue(_res);
		_res["type"] = picojson::value(string("error"));
		_res["detail"] = picojson::value(errMsg);
		auto rsRes = std::get<1>(req);
		rsRes->ack(resValue.serialize());
		
	}
	// clear up
	{
		// last one is sent,so pop it.
		lock_guard<mutex> lock(dataMtx);
		this->_rsReq.pop_back();
		// if there are more buffered than iConstBufferCommandCounter
		// sent busy and pop,from front side.
		while( iConstBufferCommandCounter < this->_rsReq.size())
		{
			auto rsRes = std::get<1>(this->_rsReq.front());
			rsRes->ackBusy();
			this->_rsReq.pop_front();
		}
	}
}

void Dispatcher::onResponse(int id ,const string & msg)
{
	auto it = uniq_nb_ws.find(id);
	if(uniq_nb_ws.end() != it)
	{
		auto res = it->second;
		res->ack(msg);
		// 正常に返事したものをキューから削除する
		uniq_nb_ws.erase(it);
		
		// 返事していないゴミを削除する
		// 先に要求出したが、まだ来ないものについて、エラーを返し。
		auto tryIt = uniq_nb_ws.begin();
		while(tryIt != uniq_nb_ws.end())
		{
			// 基本が、古い番号から、エラーを返す。
			// UIDが最大値まだ行ったら、採番が頭に戻るため。
			// 番号古くでも、若い要求かも。
			if(id <= gLastRequestUID )
			{
				if(tryIt->first < id || tryIt->first > gLastRequestUID)
				{
					auto resBad = tryIt->second;
					res->ackMiss();
					tryIt = uniq_nb_ws.erase(tryIt);
				}
				else
				{
					tryIt++;
				}
			}
			else
			{
				if(tryIt->first > gLastRequestUID && tryIt->first < id )
				{
					auto resBad = tryIt->second;
					res->ackMiss();
					tryIt = uniq_nb_ws.erase(tryIt);
				}
				else
				{
					tryIt++;
				}
			}
		}
	}
	auto itRS = uniq_nb_rs.find(id);
	if(uniq_nb_rs.end() != itRS)
	{
		auto res = itRS->second;
		res->ack(msg);
		// 正常に返事したものをキューから削除する
		uniq_nb_rs.erase(itRS);
		// 返事していないゴミを削除する
		// 先に要求出したが、まだ来ないものについて、エラーを返し。
		auto tryIt = uniq_nb_rs.begin();
		while(tryIt != uniq_nb_rs.end())
		{
			// 基本が、古い番号から、エラーを返す。
			// UIDが最大値まだ行ったら、採番が頭に戻るため。
			// 番号古くでも、若い要求かも。
			if(id <= gLastRequestUID )
			{
				if(tryIt->first < id || tryIt->first > gLastRequestUID)
				{
					auto resBad = tryIt->second;
					res->ackMiss();
					tryIt = uniq_nb_rs.erase(tryIt);
				}
				else
				{
					tryIt++;
				}
			}
			else
			{
				if(tryIt->first > gLastRequestUID && tryIt->first < id )
				{
					auto resBad = tryIt->second;
					res->ackMiss();
					tryIt = uniq_nb_rs.erase(tryIt);
				}
				else
				{
					tryIt++;
				}
			}
		}
	}
	auto itReq = uniq_nb_JSONReq.find(id);
	if(uniq_nb_JSONReq.end() != itReq)
	{
		uniq_nb_JSONReq.erase(itReq);
	}
}


