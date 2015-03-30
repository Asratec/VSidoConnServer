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
#include "vsido_response_parser.hpp"
#include "ws_response.hpp"
#include "rs_response.hpp"
#include "vsido_request_common.hpp"
#include "vsido_response_common.hpp"
#include "uart_send.hpp"
#include "vsido_request_parser.hpp"
using namespace VSido;

#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
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

static const int iConstRequestWaitTimeout = 10;

static list<tuple<string,shared_ptr<WSResponse>>> uniq_wsReq;
static list<tuple<string,shared_ptr<RSResponse>>> uniq_rsReq;
static list<shared_ptr<WSResponse>> uniq_wsRes;
static list<shared_ptr<RSResponse>> uniq_rsRes;

/** コンストラクタ
* @param send VSidoと繋がるURATの送信
*/
Dispatcher::Dispatcher(UARTSend &send)
:_send(send)
,_wsReq(uniq_wsReq)
,_rsReq(uniq_rsReq)
,_wsRes(uniq_wsRes)
,_rsRes(uniq_rsRes)
{
}


#include <fstream>
#include <string>
#include <iostream>

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



/** 要求を追加する
* @param req 要求
* @param res WebSocket返事先
*/
void Dispatcher::addRequest(const string &req,shared_ptr<WSResponse> res)
{
	logReq(req);
	DUMP_VAR_DETAILS(req);
	if(this->filterBusy())
	{
		res->ackBusy();
	}
	else
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
void Dispatcher::addRequest(const string &req,shared_ptr<RSResponse> res)
{
	logReq(req);
	DUMP_VAR_DETAILS(req);
	
	if(this->filterBusy())
	{
		res->ackBusy();
		logRes("{\"type\":\"busy\"}\n");
	}
	else
	{
		this->_rsReq.push_back(make_tuple(req,res));
	}
	DUMP_VAR_DETAILS(req);
	reqCv.notify_all();
}


void Dispatcher::clearTimeout(void)
{
	DUMP_VAR_DETAILS(&dataMtx);
	
	auto distWS = this->ackDistWS();
	if(nullptr != distWS && false == _skipTimeout)
	{
		logRes("{\"type\":\"timeout\"}\n");
		distWS->ackTimeout();
	}
	auto distRS = this->ackDistRS();
	if(nullptr != distRS && false == _skipTimeout)
	{
		logRes("{\"type\":\"timeout\"}\n");
		distRS->ackTimeout();
	}
	
	{
		/// lock mini scope
		lock_guard<mutex> lock(dataMtx);
		this->_wsReq.clear();
		this->_rsReq.clear();
		this->_wsRes.clear();
		this->_rsRes.clear();
		DUMP_VAR_DETAILS(&dataMtx);
	}
}


bool Dispatcher::filterBusy(void)
{
	bool busy = false;
	DUMP_VAR_DETAILS(this->_wsRes.size());
	DUMP_VAR_DETAILS(this->_wsRes.empty());
	lock_guard<mutex> lock(dataMtx);
	busy = ! this->_wsRes.empty();
	if(busy)
	{
		return true;
	}

	DUMP_VAR_DETAILS(this->_rsRes.size());
	DUMP_VAR_DETAILS(this->_rsRes.empty());
	busy = ! this->_rsRes.empty();
	if(busy)
	{
		return true;
	}
	return false;
}

shared_ptr<WSResponse> Dispatcher::ackDistWS()
{
	shared_ptr<WSResponse> topWS;
	{
		/// lock mini scope
		lock_guard<mutex> lock(dataMtx);
		if(false == this->_wsRes.empty())
		{
			topWS = this->_wsRes.front();
		}
		DUMP_VAR_DETAILS(&dataMtx);
	}
	return topWS;
}
shared_ptr<RSResponse> Dispatcher::ackDistRS()
{
	shared_ptr<RSResponse> topRS;
	{
		/// lock mini scope
		lock_guard<mutex> lock(dataMtx);
		if(false == this->_rsRes.empty())
		{
			topRS = this->_rsRes.front();
		}
		DUMP_VAR_DETAILS(&dataMtx);
	}
	return topRS;
}



/** 返事を返す
* @param uart VSidoコネクターから返事
* @return None
*/
void Dispatcher::ack(const list<unsigned char> &uart)
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	long long now = tv.tv_sec *1000 + tv.tv_usec /1000;
	long long repsponseTimeLag = now - globalSendTime;
	DUMP_VAR(repsponseTimeLag);
	
	ResponseParser response(uart);
	DUMP_VAR_DETAILS(&response);
	
	
	auto distWS = this->ackDistWS();
	if(nullptr != distWS)
	{
		auto expect = distWS->getExpect();
		auto resMsg = response.conv(expect);
		logRes(resMsg);
		distWS->ack(resMsg);
	}
	auto distRS = this->ackDistRS();
	if(nullptr != distRS)
	{
		auto expect = distRS->getExpect();
		auto resMsg = response.conv(expect);
		logRes(resMsg);
		distRS->ack(resMsg);
	}

	{
		/// lock mini scope
		lock_guard<mutex> lock(dataMtx);
		this->_wsReq.clear();
		this->_rsReq.clear();
		this->_wsRes.clear();
		this->_rsRes.clear();
		DUMP_VAR_DETAILS(&dataMtx);
	}
	ackCv.notify_all();
	
}

/** 分配スレッド本体
* @return None
*/
void Dispatcher::operator()()
{
	while(true)
	{
		_skipTimeout = false;
		/// send
		bool wait = false;
		{
			lock_guard<mutex> lock(dataMtx);
			wait =  (true == this->_wsReq.empty()) &&  (true == this->_rsReq.empty());
		}
		if(wait)
		{
			DUMP_VAR_DETAILS(&reqMtx);
			DUMP_VAR_DETAILS(&reqCv);
			unique_lock<mutex> lck(reqMtx);
			auto now = chrono::system_clock::now();
			auto waitRet = reqCv.wait_until(lck,now + std::chrono::milliseconds(iConstRequestWaitTimeout));
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
		
		struct timeval tv;
		gettimeofday(&tv,NULL);
		globalSendTime = tv.tv_sec *1000 + tv.tv_usec /1000;
		DUMP_VAR_DETAILS(&ackMtx);
		DUMP_VAR_DETAILS(&ackCv);
		unique_lock<mutex> lck(ackMtx);
		
		auto now = chrono::system_clock::now();
		auto waitRet = ackCv.wait_until(lck,now + std::chrono::milliseconds(iConstVSidoCommTimeout));
		if(cv_status::timeout == waitRet)
		{
			this->clearTimeout();
		}
	}
}

void Dispatcher::trySendWSReq(void)
{
	list<tuple<string,shared_ptr<WSResponse>>> localReq;
	{
		lock_guard<mutex> lock(dataMtx);
		localReq = this->_wsReq;
	}
	for(auto &req : localReq)
	{
		RequestParser parser(std::get<0>(req));
		DUMP_VAR_DETAILS(&parser);
		auto request = parser.create();
		auto uart = request->conv();
		auto expect = request->expect();
		if(nullptr != expect)
		{
			auto res = std::get<1>(req);
			res->setExpect(expect);
			lock_guard<mutex> lock(dataMtx);
			_wsRes.push_back(res);
		}
		if(false == uart.empty())
		{
		/// uart request
			this->_send.send(uart);
		}
		else
		{
			/// other request now only for bluetooth 
			auto result = request->runTask();
			auto res = std::get<1>(req);
			if(false == result.empty())
			{
				res->ack(result);
				_skipTimeout = true;
			}
		}
		
	}
}
void Dispatcher::trySendRSReq(void)
{
	list<tuple<string,shared_ptr<RSResponse>>> localReq;
	{
		lock_guard<mutex> lock(dataMtx);
		localReq = this->_rsReq;
	}
	for(auto &req : localReq)
	{
		RequestParser parser(std::get<0>(req));
		DUMP_VAR_DETAILS(&parser);
		auto request = parser.create();
		auto uart = request->conv();
		auto expect = request->expect();
		if(nullptr != expect)
		{
			auto res = std::get<1>(req);
			res->setExpect(expect);
			lock_guard<mutex> lock(dataMtx);
			_rsRes.push_back(res);
		}
		if(false == uart.empty())
		{
		/// uart request
			this->_send.send(uart);
		}
		else
		{
			////
		}
	}
}

