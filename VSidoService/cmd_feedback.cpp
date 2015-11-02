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
#include "cmd_feedback.hpp"
#include "VSido.hpp"
using namespace VSido;
#include <string>
#include <iostream>
#include <algorithm>
#include <cstddef>
using namespace std;

#include "debug.h"



/** コンストラクタ
* @param[in] raw httpサーバーからのJson要求
*  {
*	  "cmd" : "SetFeedbackID",
*	  "servo"   : [
	   sid(1~254)
*	  ]
*  }
*/
IDFeedbackJSONRequest::IDFeedbackJSONRequest(picojson::object &raw)
:JSONRequest()
,_raw(raw)
{
}

/** コマンドを実行する。
* @param None
* @return 返事のJSON文字列
*/
string IDFeedbackJSONRequest::exec()
{
	FeedBackIDRequest req;
	try
	{
	//	DUMP_VAR(_raw);
		auto sids = _raw["servo"].get<picojson::array>();
		for (auto it = sids.begin(); it != sids.end(); it++)
		{
			unsigned char sid = (unsigned char)it->get<double>();
			DUMP_VAR(sid);
			req.sid(sid);
		}
	}
	catch(const std::runtime_error &e)
	{
		FATAL_VAR(e.what());
	}
	if(req)
	{
		auto ack = req.exec();
		return Ack(ack);
	}
	else
	{
		return req.msg();
	}
}



/** コンストラクタ
* @param[in] raw httpサーバーからのJson要求
*  {
*	"cmd" : "GetServoFeedback",
*	"item":[
*		"rom_model_num",
*		"rom_servo_ID",
*		"rom_cw_agl_lmt",
*		"rom_ccw_agl_lmt",
*		"rom_damper",
*		"rom_cw_cmp_margin",
*		"rom_ccw_cmp_margin",
*		"rom_cw_cmp_slope",
*		"rom_ccw_cmp_slope",
*		"rom_punch",
*		"ram_goal_pos",
*		"ram_goal_tim",
*		"ram_max_torque",
*		"ram_torque_mode",
*		"ram_pres_pos",
*		"ram_pres_time",
*		"ram_pres_spd",
*		"ram_pres_curr",
*		"ram_pres_temp",
*		"ram_pres_volt",
*		"Flags",
*		"alg_ofset",
*		"parents_ID",
*		"connected",
*		"read_time",
*		"_ram_goal_pos",
*		"__ram_goal_pos",
*		"_ram_res_pos",
*		"_send_speed",
*		"_send_cmd_time",
*		"flg_min_max",
*		"flg_goal_pos",
*		"flg_parent_inv",
*		"flg_cmp_slope",
*		"flg_check_angle",
*		"port_type",
*		"servo_type"
*	]
*  }
*/
FeedbackJSONRequest::FeedbackJSONRequest(picojson::object &raw)
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
string FeedbackJSONRequest::exec()
{
	FeedBackRequest req;
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
		if(false == _offsets.empty())
		{
			std::sort(_offsets.begin(), _offsets.end() );
			unsigned char add = (unsigned char)(_offsets.front());
			unsigned char length = (unsigned char)(_offsets.back() - _offsets.front());
			//次項目があれば、最後の項目サイズは１バイト、なければ、２バイト
			if(_fields.end() != _fields.find(_offsets.back() +1))
			{
				length += 1;
			}
			else
			{
				length += 2;
			}
			req.feedback(add,length);
		}
		else
		{
			if(all)
			{
				req.feedback(0,53);
			}
		}

	}
	catch(const std::runtime_error &e)
	{
		DUMP_VAR(e.what());
	}
	if(req)
	{
		auto ack = req.exec();
		if(ack.timeout())
		{
			_res["type"] = picojson::value(string("timeout"));
			picojson::value resValue(_res);
			return resValue.serialize();
		}
		if(ack)
		{
			_res["type"] = picojson::value(string("GetServoFeedback"));
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

