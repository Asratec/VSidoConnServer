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
#include "cmd_servo_info.hpp"
#include "VSido.hpp"
using namespace VSido;
#include <string>
#include <algorithm>
#include <iostream>
#include <cstddef>
using namespace std;

#include "debug.h"


/** コンストラクタ
* @param[in] raw httpサーバーからのJson要求
*  {
*	 "cmd" : "GetServoInfo"("servo_info"),
*	 "servo"  : [
*	   sid(1~254)
*	 ]
*  }
*/
ServoInfoJSONRequest::ServoInfoJSONRequest(picojson::object &raw)
:JSONRequest()
,_raw(raw)
,_fields(servo_table)
,_angleType(angle_type_list)
{
}

/** コマンドを実行する。
* @param None
* @return 返事のJSON文字列
*/
string ServoInfoJSONRequest::exec()
{
	ServoInfoRequest req;
	try
	{
		bool all = false;
		auto items = _raw["item"].get<picojson::array>();
		for (auto it = items.begin(); it != items.end(); it++)
		{
			auto item = it->get<string>();
			for(auto field :_fields)
			{
				if(std::get<0>(field.second) == item)
				{
					_offsets.push_back(field.first);
				}
			}
			if("all" ==item)
			{
				all = true;
				_offsets.clear();
				break;
			}
			DUMP_VAR(item);
		}
		unsigned char add = 0;
		unsigned char length = 0;
		if(false == _offsets.empty())
		{
			std::sort(_offsets.begin(), _offsets.end() );
			add = (unsigned char)(_offsets.front());
			length = (unsigned char)(_offsets.back() - _offsets.front());
			//次項目があれば、最後の項目サイズは１バイト、なければ、２バイト
			if(_fields.end() != _fields.find(_offsets.back() +1))
			{
				length += 1;
			}
			else
			{
				length += 2;
			}
		}
		else
		{
			if(all)
			{
				add = 0;
				length = 53;
			}
		}
	//	DUMP_VAR(_raw);
		auto servo = _raw["servo"].get<picojson::array>();
		for (auto it = servo.begin(); it != servo.end(); it++)
		{
			unsigned char sid = (unsigned char) it->get<double>();
			DUMP_VAR((int)sid);
			req.info(sid,add,length);
		}	
	}
	catch(const std::runtime_error &e)
	{
		FATAL_VAR(e.what());
	}
	if(req)
	{
		auto ack = req.exec();
		_res["type"] = picojson::value(string("GetServoInfo"));
		if(ack.timeout())
		{
			_res["type"] = picojson::value(string("timeout"));
			picojson::value resValue(_res);
			return resValue.serialize();
		}
		if(ack)
		{
			string raw = static_cast<string>(ack);
			_res["raw"] = picojson::value(raw);
			picojson::array servoArray;
			ack.forEachServo( [this,&servoArray](unsigned char sid,vector<tuple<string,int>>info){
				picojson::object servo;
				servo["sid"] =  picojson::value(double(sid));
				for(const auto &ir:info)
				{
					auto name = std::get<0>(ir);
					auto value = std::get<1>(ir);
					auto angleType = std::find(_angleType.begin(), _angleType.end(), name);
					if(angleType != _angleType.end())
					{
						servo[name] =  picojson::value(double(value)/double(10));
					}
					else
					{
						servo[name] =  picojson::value(double(value));
					}
				}
				servoArray.push_back(picojson::value(servo));
			}
			);
			_res["servo"] = picojson::value(servoArray);
			picojson::value resValue(_res);
			return resValue.serialize();
		}
		else
		{
			_res["type"] = picojson::value(string("error"));
			_res["detail"] = picojson::value(string(""));
			picojson::value resValue(_res);
			return resValue.serialize();
		}
	}
	else
	{
		return req.msg();
	}
}

