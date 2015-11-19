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
#include "cmd_var.hpp"
#include "vsido_vid_table.hpp"
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
*	 "cmd" : "SetVIDValue",
*	"vid":[
*	    {"RS485_Baudrate": "9600" ~ "57600" ~ "115200" ~ "1000000" or 0~255},
*	    {"TTL_Baudrate":"9600" ~ "57600" ~ "115200" ~ "1000000" or 0~255},
*	    {"RS232_Baudrate":"9600" ~ "57600" ~ "115200" ~ "1000000" or 0~255},
*	    {"IO_PA_IO_Mode":4~7},
*	    {"IO_PA_Analog_Mode":4~7},
*	    {"IO_PA_PWM":0~1},
*	    {"IO_PA_PWM_CYCLE":0~16383},
*	    {"Through_Port":0~255},
*	    {"Servo_Type_RS485":"None" ~ "FUTABA" ~ "ROBOTIS_v1.0" ~ ":ROBOTIS_v2.0" or 0~255},
*	    {"Servo_Type_TTL":"None" ~ "FUTABA" ~ "ROBOTIS_v1.0" ~ ":ROBOTIS_v2.0" or 0~255},
*	    {"IMU_Type": "None" or 0~255},
*	    {"Balancer_Flag":0~1},
*	    {"Theta_Th":0.1~10.0},
*	    {"Cycletime":1~100},
*	    {"Min_Cmp":1~250},
*	    {"Flag_Ack":0~1},
*	    {"Volt_Th":6.0~9.0},
*	    {"Initialize_Torque":0~1},
*	    {"Initialize_Angle":0~1},
*	    {"Inspection_Flag":0~1},
*	    {"Inspection_Type":0~1},
*	    {"Robot_Model":"None" ~ "GR-001" ~ "DAWIN-MINI" or 0~255},
*	    {"UID_Flag":0~1}
*	]
* }
*/
VarSetJSONRequest::VarSetJSONRequest(picojson::object &raw)
:JSONRequest()
,_raw(raw)
,_fields(vid_label_table)
,_builtinVars(
{
	{"B_9600",0},
	{"B_57600",1},
	{"B_115200",2},
	{"B_1000000",3},
	{"None",0},
	{"FUTABA",1},
	{"ROBOTIS_v1.0",2},
	{"ROBOTIS_v2.0",3},
	{"None",0},
	{"GR-001",1},
	{"DARWIN-MINI",2}
}
)
{
}

/** コマンドを実行する。
* @param None
* @return 返事のJSON文字列
*/
string VarSetJSONRequest::exec()
{
	SetVIDRequest req;
	try
	{
	//	DUMP_VAR(_raw);
		auto vars = _raw["vid"].get<picojson::array>();
		for (auto it = vars.begin(); it != vars.end(); it++)
		{
			const auto  &object = it->get<picojson::object>();
			if(object.empty())
			{
				break;
			}
			auto itObj = object.begin();
			const string name = itObj->first;
			
			
			int value = 0;
			auto f_it = _fields.find(name);
			if(f_it != _fields.end())
			{
				unsigned char vid = std::get<0>(f_it->second);
				auto size = std::get<1>(f_it->second);
				if(itObj->second.is<string>())
				{
					auto valueStr = itObj->second.get<string>();
					auto itBI = _builtinVars.find(valueStr);
					if(itBI != _builtinVars.end())
					{
						value = itBI->second;
					}
				}
				else
				{
					value = (int)(itObj->second.get<double>());
				}
				if(1 == size)
				{
					req.value(vid,(unsigned char)value);
				}
				else if(2 == size)
				{
					if("IO_PA_PWM_CYCLE" == name)
					{
						// 4 us.
						value /= 4;
					}
					req.value2B(vid,(unsigned short)value);
				}
				else
				{
					FATAL_VAR(size);
					FATAL_VAR("error!!!!");
				}
				DUMP_VAR(name);
				DUMP_VAR((int)vid);
				DUMP_VAR(value);
			}
			else
			{
				FATAL_VAR(name);
			}
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
*	"cmd" : "GetVIDValue",
*	"vid":[
*	 "all",
*    "RS485_Baudrate",
*    "TTL_Baudrate",
*    "RS232_Baudrate",
*    "IO_PA_IO_Mode",
*    "IO_PA_Analog_Mode",
*    "IO_PA_PWM",
*    "IO_PA_PWM_CYCLE",
*    "Through_Port",
*    "Servo_Type_RS485",
*    "Servo_Type_TTL",
*    "IMU_Type",
*    "Balancer_Flag",
*    "Theta_Th",
*    "Cycletime",
*    "Min_Cmp",
*    "Flag_Ack",
*    "Volt_Th",
*    "Initialize_Torque",
*    "Initialize_Angle",
*    "Inspection_Flag",
*    "Inspection_Type",
*    "Robot_Model",
*    "UID_Flag"
*   ]
*  }
*/
VarGetJSONRequest::VarGetJSONRequest(picojson::object &raw)
:JSONRequest()
,_raw(raw)
,_fields(vid_label_table)
,_builtinVars(
{
	{"B_9600",0},
	{"B_57600",1},
	{"B_115200",2},
	{"B_1000000",3},
	{"None",0},
	{"FUTABA",1},
	{"ROBOTIS_v1.0",2},
	{"ROBOTIS_v2.0",3},
	{"None",0},
	{"GR-001",1},
	{"DAWIN-MINI",2}
}
)
{
}

/** コマンドを実行する。
* @param None
* @return 返事のJSON文字列
*/
string VarGetJSONRequest::exec()
{
	GetVIDRequest req;
	try
	{
	//	DUMP_VAR(_raw);
		auto vars = _raw["vid"].get<picojson::array>();
		for (auto it = vars.begin(); it != vars.end(); it++)
		{
			auto &tmpName = it->get<string>();
			if("all" == tmpName)
			{
				for(auto field : _fields)
				{
					unsigned char vid = std::get<0>(field.second);
					DUMP_VAR(vid);
					req.vid(vid);
				}
				break;
			}
			auto f_it = _fields.find(tmpName);
			if(f_it != _fields.end())
			{
				unsigned char vid = std::get<0>(f_it->second);
				DUMP_VAR(vid);
				req.vid(vid);
			}
		}	
	}
	catch(const std::runtime_error &e)
	{
		DUMP_VAR(e.what());
	}
	if(req)
	{
		auto fn = [this](GetVIDResponse &resp){
			if(resp.timeout())
			{
				picojson::value resValue(_res);
				_res["type"] = picojson::value(string("timeout"));
				Dispatcher::onResponse(uid_,resValue.serialize());
				return ;
			}
			if(resp)
			{
				_res["type"] = picojson::value(string("GetVIDValue"));
				string raw = static_cast<string>(resp);
				_res["raw"] = picojson::value(raw);
				picojson::object vars;
				resp.forEachVar( [this,&vars](const string &name,int value){
					if("IO_PA_PWM_CYCLE" == name)
					{
						// 4 us.
						value *= 4;
					}
					vars[name] =  picojson::value(double(value));
				}
				);
				_res["vid"] = picojson::value(vars);
				picojson::value resValue(_res);
				Dispatcher::onResponse(uid_,resValue.serialize());
				return ;
			}
			else
			{
				picojson::value resValue(_res);
				_res["type"] = picojson::value(string("error"));
				_res["detail"] = picojson::value(string(""));
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


