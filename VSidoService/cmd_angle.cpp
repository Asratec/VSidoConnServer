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
#include "cmd_angle.hpp"
#include "VSido.hpp"
using namespace VSido;
#include <string>
#include <iostream>
#include <cstddef>
using namespace std;

#include "debug.h"


/** コンストラクタ
* @param[in] raw httpサーバーからのJson要求
*  {
*    "cmd": "servoAngle",
*    "cycle": 10~1000,
*    "servo":[
*      {"sid":1~254,"angle":-180.0~180.0}
*    ]
*  }
*/
AngleJSONRequest::AngleJSONRequest(picojson::object &raw)
:JSONRequest()
,_raw(raw)
{
}

/** コマンドを実行する。
* @param None
* @return 返事のJSON文字列
*/
string AngleJSONRequest::exec()
{
	AngleRequest req;
	try
	{
	//	DUMP_VAR(_raw);
		unsigned int  cycle = (unsigned int)(_raw["cycle"].get<double>());
		cycle /= 10;
		DUMP_VAR(cycle);
		req.cycle(cycle);

		auto servo = _raw["servo"].get<picojson::array>();
		for (auto it = servo.begin(); it != servo.end(); it++)
		{
			auto &tmpObject = it->get<picojson::object>();
			unsigned char sid = (unsigned char)tmpObject["sid"].get<double>();
			DUMP_VAR(sid);
			auto  angle = tmpObject["angle"].get<double>();
			DUMP_VAR(angle);
			req.angle(sid,angle);
		}	
	}
	catch(const std::runtime_error &e)
	{
		FATAL_VAR(e.what());
	}
	if(req)
	{
		req.execNA();
		return Ack();
	}
	else
	{
		return req.msg();
	}
}


/** コンストラクタ
* @param[in] raw httpサーバーからのJson要求
*  {
*	 "cmd" : "GetServoAngle",
*	 "servo"  : [
*	   {"sid":1~254}
*	 ]
*  }
*/
AngleGetJSONRequest::AngleGetJSONRequest(picojson::object &raw)
:JSONRequest()
,_raw(raw)
{
}

/** コマンドを実行する。
* @param None
* @return 返事のJSON文字列
*/
string AngleGetJSONRequest::exec()
{
	ServoInfoRequest req;
	try
	{
	//	DUMP_VAR(_raw);
		auto servo = _raw["servo"].get<picojson::array>();
		for (auto it = servo.begin(); it != servo.end(); it++)
		{
			unsigned char sid = (unsigned char)it->get<double>();
			DUMP_VAR((int)sid);
			req.info(sid,19,2);
		}	
	}
	catch(const std::runtime_error &e)
	{
		FATAL_VAR(e.what());
	}
	if(req)
	{
		auto fn = [this](ServoInfoResponse & resp){
			if(resp.timeout())
			{
				_res["type"] = picojson::value(string("timeout"));
				picojson::value resValue(_res);
				Dispatcher::onResponse(uid_,resValue.serialize());
				return ;
			}
			if(resp)
			{
				_res["type"] = picojson::value(string("GetServoAngle"));
				string raw = static_cast<string>(resp);
				_res["raw"] = picojson::value(raw);
				picojson::array servoArray;
				resp.forEachItem( [&servoArray](unsigned char sid,string name,int value){
					if("ram_pres_pos" == name)
					{
						picojson::object servo;
						servo["sid"] =  picojson::value(double(sid));
						servo["angle"] =  picojson::value(double(value)/double(10));
						servoArray.push_back(picojson::value(servo));
					}
				}
				);
				_res["servo"] = picojson::value(servoArray);
				picojson::value resValue(_res);
	            DUMP_VAR(resValue.serialize());
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

/** コンストラクタ
* @param[in] raw httpサーバーからのJson要求
*  {
*	  "cmd" : "SetServoMinMaxAngle",
*	  "angle"   : [
	   {"sid":1~254,"min":-180.0~180.0,"max":-180.0~180.0}
*	  ]
*  }
*/
MinMaxAngleJSONRequest::MinMaxAngleJSONRequest(picojson::object &raw)
:JSONRequest()
,_raw(raw)
{
}

/** コマンドを実行する。
* @param None
* @return 返事のJSON文字列
*/
string MinMaxAngleJSONRequest::exec()
{
	MinMaxAngleRequest req;
	try
	{
	//	DUMP_VAR(_raw);
		auto servo = _raw["servo"].get<picojson::array>();
		for (auto it = servo.begin(); it != servo.end(); it++)
		{
			auto &tmpObject = it->get<picojson::object>();
			
			unsigned char sid = (unsigned char)tmpObject["sid"].get<double>();
			DUMP_VAR(sid);
			auto min =  tmpObject["min"].get<double>();
			DUMP_VAR(min);
			auto max = tmpObject["max"].get<double>();
			DUMP_VAR(max);
			req.minMax(sid,min,max);
		}	
	}
	catch(const std::runtime_error &e)
	{
		FATAL_VAR(e.what());
	}
	if(req)
	{
		req.execNA();
		return Ack();
	}
	else
	{
		return req.msg();
	}
}


