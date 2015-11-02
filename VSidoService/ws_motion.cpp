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
#include "VSido.hpp"
#include "VSidoMotion.hpp"
using namespace VSido;
#include <string>
#include <iostream>
using namespace std;

#include <libwebsockets.h>


#include "picojson.h"
#include "debug.h"


static void ackWebSocket(const string &msg,libwebsocket *ws);

/** WebSocketの受信メッセージを処理する
* @param name モデル名
* @param ws WebSocket
* @return None
*/
void handleMotionMsg(const string &msg,libwebsocket *ws)
{
    picojson::value _raw;
    picojson::object _res;
	bool doAck = true;

	try
	{
		std::string err;
		picojson::parse(_raw, msg.c_str(),msg.c_str() + msg.size(),&err);
		DUMP_VAR(err);
		DUMP_VAR(_raw);
		auto ser = _raw.serialize();
		DUMP_VAR(ser);

		string cmdStr = "";
		picojson::object& _rawObj = _raw.get<picojson::object>();
		DUMP_VAR(&_rawObj);
		cmdStr = _rawObj["motion"].get<string>();
		DUMP_VAR(cmdStr);
		if("start" == cmdStr)
		{
			Motion motion;
			auto loop = _rawObj["loop"].get<bool>();
			if(_raw.contains("id"))
			{
				auto id = _rawObj["id"].get<string>();
				motion.start(id,loop);
			}
			else
			{
				motion.start("",loop);
			}
			_res["success"] = picojson::value(true);
		}
		if("stop" == cmdStr)
		{
			Motion motion;
			motion.stop();
			_res["success"] = picojson::value(true);
		}
		if("list" == cmdStr)
		{
			Motion motion;
			auto ret = motion.list();
			picojson::array motionArray;
			for(const auto &resM :ret)
			{
				motionArray.push_back(picojson::value(resM));
			}
			_res["id"] = picojson::value(motionArray);
		}
		if("upload" == cmdStr)
		{
			
			FATAL_VAR(msg);
			Motion motion;
			auto status = _rawObj["status"].get<string>();
			static string dataMessg;
			if("begin"==status)
			{
				dataMessg = "";
				doAck = false;
			}
			if("continue"==status)
			{
				auto data = _rawObj["data"].get<string>();
				dataMessg += data;
				doAck = false;
			}
			if("end"==status)
			{
				auto id = _rawObj["id"].get<string>();
				motion.upload(id,dataMessg);
				_res["success"] = picojson::value(true);
			}

		}
	}
	catch(const std::runtime_error &e)
	{
		DUMP_VAR(e.what());
		_res["error"] = picojson::value(e.what());
	}
	if(doAck)
	{
		picojson::value val(_res);
		ackWebSocket(val.serialize(),ws);
	}
}

extern mutex mtxWSMotionConnection;
extern list<struct libwebsocket *> wsMotionCallbacks;
extern mutex mtxWSMotionAck;
extern map<libwebsocket *,list<string>> globalMotionAckList;

static void ackWebSocket(const string &msg,libwebsocket *ws)
{
	if(nullptr != ws)
	{
		{
			lock_guard<mutex> lock(mtxWSMotionConnection);
			const auto ret =std::find(wsMotionCallbacks.begin(),wsMotionCallbacks.end(),ws);
			if(wsMotionCallbacks.end() == ret)
			{
				DUMP_VAR(ws);
				return;
			}
		}
		lock_guard<mutex> lock(mtxWSMotionAck);
		auto que = globalMotionAckList.find(ws);
		if(globalMotionAckList.end() != que)
		{
			que->second.push_back(msg);
			libwebsocket_callback_on_writable_all_protocol(libwebsockets_get_protocol(ws));
			DUMP_VAR(ws);
		}
		else
		{
			list<string> acks = {msg};
			globalMotionAckList[ws] = acks;
			libwebsocket_callback_on_writable_all_protocol(libwebsockets_get_protocol(ws));
			DUMP_VAR(ws);
		}
	}
}
