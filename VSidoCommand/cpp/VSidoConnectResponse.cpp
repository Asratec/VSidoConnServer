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
#include "VSidoConnect.hpp"
#include "vsido_servo_info_table.hpp"
#include "vsido_vid_table.hpp"
#include "VSidoConnectInner.hpp"
using namespace VSido;
#include "debug.h"


#include <iostream>
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <thread>
#include <tuple>
using namespace std;




#define log_erro_param(...)


/// mili sec
static const int iConstRequestWaitTimeout = 200;

/**
* 要求コマンドを実行する
* @return VSido CONNECTから返事クラス
*/
template <typename T1,typename T2> T2 ExecRequest<T1,T2>::execBK()
{
	T1 *req = reinterpret_cast<T1*>(this);
	if(nullptr != req)
	{
		mutex reqMtx;
		T2 *resObj;
		condition_variable reqCv;
		auto fn = [&resObj,&reqCv](T2& resp){
			resObj = &resp;
			reqCv.notify_all();
		};
		req->exec(fn);
		unique_lock<mutex> lck(reqMtx);
		auto waitRet = reqCv.wait_for(lck,std::chrono::milliseconds(iConstRequestWaitTimeout));
		if(cv_status::timeout != waitRet)
		{
			return *resObj;
		}
		else
		{
			cout << "Can not Read an anwser" << endl;
		}
	}
	else
	{
		log_erro_param("wrong class");
	}
	T2 resObj;
	return resObj;
}


template AckResponse ExecRequest<AngleRequest,AckResponse>::execBK();
template AckResponse ExecRequest<ComplianceRequest,AckResponse>::execBK();
template AckResponse ExecRequest<MinMaxAngleRequest,AckResponse>::execBK();
template AckResponse ExecRequest<FeedBackIDRequest,AckResponse>::execBK();
template AckResponse ExecRequest<SetVIDRequest,AckResponse>::execBK();
template AckResponse ExecRequest<WriteFlashRequest,AckResponse>::execBK();
template AckResponse ExecRequest<GPIORequest,AckResponse>::execBK();
template AckResponse ExecRequest<PWMRequest,AckResponse>::execBK();
template AckResponse ExecRequest<IKSetRequest,AckResponse>::execBK();
template AckResponse ExecRequest<WalkRequest,AckResponse>::execBK();

template ServoInfoResponse ExecRequest<ServoInfoRequest,ServoInfoResponse>::execBK();
template FeedBackResponse ExecRequest<FeedBackRequest,FeedBackResponse>::execBK();
template GetVIDResponse ExecRequest<GetVIDRequest,GetVIDResponse>::execBK();
template JointResponse ExecRequest<JointRequest,JointResponse>::execBK();
template IKResponse ExecRequest<IKReadRequest,IKResponse>::execBK();
template AccelerationResponse ExecRequest<AccelerationRequest,AccelerationResponse>::execBK();
template VoltageResponse ExecRequest<VoltageRequest,VoltageResponse>::execBK();
template RawResponse ExecRequest<RawRequest,RawResponse>::execBK();





static mutex handlerMtx;

static map<int,tuple<Request,function<void(AckResponse&)> > > AckRespHandler;
/**
* 要求コマンドを実行する(No block)
* @param fptr 関数
* @return id
*/
int Request::exec(function<void(AckResponse&)> fptr)
{
	Request::exec();
	lock_guard<mutex> lock(handlerMtx);
	Request *req = this;
	AckRespHandler[uid_] = make_tuple(*req,fptr);
	DUMP_VAR(uid_);
	return uid_;
}


static map<int,tuple<JointRequest,function<void(JointResponse&)> > > JointRespHandler;
/**
* 要求コマンドを実行する(No block)
* @param fptr 関数
* @return id
*/
int JointRequest::exec(function<void(JointResponse&)> fptr)
{
	Request::exec();
	lock_guard<mutex> lock(handlerMtx);
	JointRespHandler[uid_] = make_tuple(*this,fptr);
	DUMP_VAR(uid_);
	return uid_;
}



static map<int,tuple<AccelerationRequest,function<void(AccelerationResponse&)> > > AccelerationRespHandler;
/**
* 要求コマンドを実行する(No block)
* @param fptr 関数
* @return id
*/
int AccelerationRequest::exec(function<void(AccelerationResponse&)> fptr)
{
	Request::exec();
	lock_guard<mutex> lock(handlerMtx);
	AccelerationRespHandler[uid_] = make_tuple(*this,fptr);
	DUMP_VAR(uid_);
	return uid_;
}

static map<int,tuple<VoltageRequest,function<void(VoltageResponse&)> > > VoltageRespHandler;
/**
* 要求コマンドを実行する(No block)
* @param fptr 関数
* @return id
*/
int VoltageRequest::exec(function<void(VoltageResponse&)> fptr)
{
	Request::exec();
	lock_guard<mutex> lock(handlerMtx);
	VoltageRespHandler[uid_] = make_tuple(*this,fptr);
	DUMP_VAR(uid_);
	return uid_;
}



static map<int,tuple<RawRequest,function<void(RawResponse&)> > > RawRespHandler;
/**
* 要求コマンドを実行する(No block)
* @param fptr 関数
* @return id
*/
int RawRequest::exec(function<void(RawResponse&)> fptr)
{
	Request::exec();
	lock_guard<mutex> lock(handlerMtx);
	RawRespHandler[uid_] = make_tuple(*this,fptr);
	DUMP_VAR(uid_);
	return uid_;
}


static map<int,tuple<FeedBackRequest,function<void(FeedBackResponse&)> > > FeedBackRespHandler;
/**
* 要求コマンドを実行する(No block)
* @param fptr 関数
* @return id
*/
int FeedBackRequest::exec(function<void(FeedBackResponse&)> fptr)
{
	Request::exec();
	lock_guard<mutex> lock(handlerMtx);
	FeedBackRespHandler[uid_] = make_tuple(*this,fptr);
	DUMP_VAR(uid_);
	return uid_;
}


static map<int,tuple<IKReadRequest,function<void(IKResponse&)> > > IKRespHandler;
/**
* 要求コマンドを実行する(No block)
* @param fptr 関数
* @return id
*/
int IKReadRequest::exec(function<void(IKResponse&)> fptr)
{
	Request::exec();
	lock_guard<mutex> lock(handlerMtx);
	IKRespHandler[uid_] = make_tuple(*this,fptr);
	DUMP_VAR(uid_);
	return uid_;
}

static map<int,tuple<ServoInfoRequest,function<void(ServoInfoResponse&)> > > ServoInfoRespHandler;
/**
* 要求コマンドを実行する(No block)
* @param fptr 関数
* @return id
*/
int ServoInfoRequest::exec(function<void(ServoInfoResponse&)> fptr)
{
	Request::exec();
	lock_guard<mutex> lock(handlerMtx);
	ServoInfoRespHandler[uid_] = make_tuple(*this,fptr);
	DUMP_VAR(uid_);
	return uid_;
}

static map<int,tuple<GetVIDRequest,function<void(GetVIDResponse&)> > > GetVIDRespHandler;
/**
* 要求コマンドを実行する(No block)
* @param fptr 関数
* @return id
*/
int GetVIDRequest::exec(function<void(GetVIDResponse&)> fptr)
{
	Request::exec();
	lock_guard<mutex> lock(handlerMtx);
	GetVIDRespHandler[uid_] = make_tuple(*this,fptr);
	DUMP_VAR(uid_);
	return uid_;
}



static void dispatchResponse(int uid,list<unsigned char> &binary);

void onRecieve(list<unsigned char> binary)
{
	DUMP_VAR(binary.size());
	// check user id.
	int uid = 0;
	if(2 < binary.size())
	{
		auto binaryTmp = binary;
		binaryTmp.pop_front();
		uid = binaryTmp.front() << 8;
		if(uid >= 0x80)
		{
			binaryTmp.pop_front();
			uid |= binaryTmp.front();
			DUMP_VAR(uid);
			if(0 != uid)
			{
				dispatchResponse(uid,binary);
			}
		}
		else
		{
			// no use uid for response.
			// raw no ack command.
			lock_guard<mutex> lock(handlerMtx);
			auto it = RawRespHandler.find(0);
			if(it != RawRespHandler.end())
			{
				auto req = std::get<0>(it->second);
				RawResponse resp(binary,req);
				auto fn = std::get<1>(it->second);
				fn(resp);
				RawRespHandler.erase(it);
			}
		}
	}
}

#define TRY_INVOKE_RESPONSE_HANDLER(x)\
{\
	lock_guard<mutex> lock(handlerMtx);\
	auto it = x ## RespHandler.find(uid);\
	if(x ## RespHandler.end() != it )\
	{\
		auto req = std::get<0>(it->second);\
		x ## Response resp(binary,req);\
		auto fn = std::get<1>(it->second);\
		DUMP_VAR(typeid(resp).name());\
		DUMP_VAR(typeid(fn).name());\
		fn(resp);\
		x ## RespHandler.erase(it);\
	}\
}


static void dispatchResponse(int uid,list<unsigned char> &binary)
{
	DUMP_VAR(uid);
	DUMP_VAR(binary.size());
	TRY_INVOKE_RESPONSE_HANDLER(Ack);
	TRY_INVOKE_RESPONSE_HANDLER(Joint);
	TRY_INVOKE_RESPONSE_HANDLER(Acceleration);
	TRY_INVOKE_RESPONSE_HANDLER(Voltage);
	TRY_INVOKE_RESPONSE_HANDLER(Raw);
	TRY_INVOKE_RESPONSE_HANDLER(FeedBack);
	TRY_INVOKE_RESPONSE_HANDLER(Joint);
	TRY_INVOKE_RESPONSE_HANDLER(IK);
	TRY_INVOKE_RESPONSE_HANDLER(ServoInfo);
	TRY_INVOKE_RESPONSE_HANDLER(GetVID);
}

