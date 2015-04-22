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
#include <string>
#include <cstdio>
#include <iostream>
using namespace std;


#include "debug.h"


#include "vsido_request_parser.hpp"
#include "vsido_request_ng_param.hpp"
#include "vsido_request_angle.hpp"
#include "vsido_request_compliance.hpp"
#include "vsido_request_min_max_angle.hpp"
#include "vsido_request_id_feedback.hpp"
#include "vsido_request_feedback.hpp"
#include "vsido_request_servo_info.hpp"
#include "vsido_request_var_set.hpp"
#include "vsido_request_var_get.hpp"
#include "vsido_request_write_flash.hpp"
#include "vsido_request_gpio.hpp"
#include "vsido_request_pulse.hpp"
#include "vsido_request_ping.hpp"
#include "vsido_request_ik.hpp"
#include "vsido_request_walk.hpp"
#include "vsido_request_acceleration.hpp"
#include "vsido_request_voltage.hpp"
#include "vsido_request_raw.hpp"
#include "vsido_request_scan_bt.hpp"
#include "vsido_request_bind_bt.hpp"
using namespace VSido;


/** コンストラクタ
* @param[in] raw httpサーバーからのJson要求
*  {
*	 "command" : コマンド,
*     コマンドに応じて変わる内容
*  }
*/
RequestParser::RequestParser(const string &json)
:_raw()
{
	try
	{
		std::string err;
		picojson::parse(_raw, json.c_str(),json.c_str() + json.size(),&err);
		DUMP_VAR(err);
		DUMP_VAR(_raw);
		auto ser = _raw.serialize();
		DUMP_VAR(ser);
	}
	catch(const std::runtime_error &e)
	{
		DUMP_VAR(e.what());
	}
}

/** Json要求からVSidoパケットへ変換する
* @param None
* @return VSidoパケット
*/
list<unsigned char> RequestParser::conv(void)
{
	auto pCmd = create();
	auto ret = pCmd->conv();
	dumpUart(ret);
	return ret;
}

void RequestParser::dumpUart(const list<unsigned char>&uart)
{
	DUMP_VAR(&uart);
#if 0 // not debug
	for(auto ch:uart)
	{
		printf("0x%02x,",ch);
	}
	cout << endl;
#endif
}



/** 要求のインスタンス作成する。
* @param None
* @return インスタンス
*/
shared_ptr<Request>RequestParser::create(void)
{
	try
	{
		string cmdStr = "";
		picojson::object& _rawObj = _raw.get<picojson::object>();
		DUMP_VAR(&_rawObj);
		cmdStr = _rawObj["cmd"].get<string>();
		DUMP_VAR(cmdStr);

	
		if("servoAngle" == cmdStr)
		{
			return shared_ptr<Request>(new RequestAngle(_rawObj));
		}
		else if("compliance" == cmdStr)
		{
			return shared_ptr<Request>(new RequestCompliance(_rawObj));
		}
		else if("minmax" == cmdStr)
		{
			return shared_ptr<Request>(new RequestMinMaxAngle(_rawObj));
		}
		else if("id_feedback" == cmdStr)
		{
			return shared_ptr<Request>(new RequestIDFeedback(_rawObj));
		}
		else if("feedback" == cmdStr)
		{
			return shared_ptr<Request>(new RequestFeedback(_rawObj));
		}
		else if("servo_info" == cmdStr)
		{
			return shared_ptr<Request>(new RequestServoInfo(_rawObj));
		}
		else if("varset" == cmdStr)
		{
			return shared_ptr<Request>(new RequestVarSet(_rawObj));
		}
		else if("varget" == cmdStr)
		{
			return shared_ptr<Request>(new RequestVarGet(_rawObj));
		}
		else if("writeflash" == cmdStr)
		{
			return shared_ptr<Request>(new RequestWriteFlash(_rawObj));
		}
		else if("gpio" == cmdStr)
		{
			return shared_ptr<Request>(new RequestGpio(_rawObj));
		}
		else if("pulse" == cmdStr)
		{
			return shared_ptr<Request>(new RequestPulse(_rawObj));
		}
		else if("ping" == cmdStr)
		{
			return shared_ptr<Request>(new RequestPing(_rawObj));
		}
		else if("ik" == cmdStr)
		{
			return shared_ptr<Request>(new RequestIK(_rawObj));
		}
		else if("walk" == cmdStr)
		{
			return shared_ptr<Request>(new RequestWalk(_rawObj));
		}
		else if("acceleration" == cmdStr)
		{
			return shared_ptr<Request>(new RequestAcceleration(_rawObj));
		}
		else if("voltage" == cmdStr)
		{
			return shared_ptr<Request>(new RequestVoltage(_rawObj));
		}
		else if("raw" == cmdStr)
		{
			return shared_ptr<Request>(new RequestRaw(_rawObj));
		}
		else if("scan_bt" == cmdStr)
		{
			return shared_ptr<Request>(new RequestScanBT(_rawObj));
		}
		else if("bind_bt" == cmdStr)
		{
			return shared_ptr<Request>(new RequestBindBT(_rawObj));
		}
		else
		{
			return shared_ptr<Request>(new RequestNGParam(_rawObj));
		}
	}
	catch(const std::runtime_error &e)
	{
		DUMP_VAR(e.what());
		picojson::object obj;
		return shared_ptr<Request>(new RequestNGParam(obj));
	}
}

