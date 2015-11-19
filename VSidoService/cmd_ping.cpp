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
#include "cmd_ping.hpp"
#include "VSido.hpp"
#include "dispatch.hpp"
using namespace VSido;
#include <string>
#include <iostream>
using namespace std;


#include "debug.h"

/** コンストラクタ
* @param[in] raw httpサーバーからのJson要求
*  {
*	 "cmd" : "CheckConnectedServo"
*  }
*/
PingJSONRequest::PingJSONRequest(picojson::object &raw)
:JSONRequest()
,_raw(raw)
{
}



/** 返事を通知する。
* @param res レスポンス
* @return 返事のJSON文字列
string PingJSONRequest::onResponse(JointResponse& resp)
*/


/** コマンドを実行する。
* @param None
* @return 返事のJSON文字列
*/
string PingJSONRequest::exec()
{
	JointRequest req;
	if(req)
	{
		auto fn = [this](JointResponse& resp) {
			if(resp.timeout())
			{
				_res["type"] = picojson::value(string("timeout"));
				picojson::value resValue(_res);
				Dispatcher::onResponse(uid_,resValue.serialize());
				return ;
			}
			if(resp)
			{
				_res["type"] = picojson::value(string("CheckConnectedServo"));
				string raw = static_cast<string>(resp);
				_res["raw"] = picojson::value(raw);
				picojson::array servoArray;
				resp.forEach( [this,&servoArray](unsigned char sid,unsigned char tim){
					picojson::object servo;
					double timMicroSec = tim * 10;
					servo["sid"] =  picojson::value(double(sid));
					servo["tim"] =  picojson::value(double(timMicroSec));
					servoArray.push_back(picojson::value(servo));
				}
				);
				_res["servo"] = picojson::value(servoArray);
				picojson::value resValue(_res);
				Dispatcher::onResponse(uid_,resValue.serialize());
				return ;
			}
			else
			{
				_res["type"] = picojson::value(string("error"));
				_res["detail"] = picojson::value(string(""));
				picojson::value resValue(_res);
				Dispatcher::onResponse(uid_,resValue.serialize());
				return ;
			}
		};
		uid_ = req.exec(fn);
		return "";
	}
	else
	{
		return req.msg();
	}
}


