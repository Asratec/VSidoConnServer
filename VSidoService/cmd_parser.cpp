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


#include "cmd_parser.hpp"
#include "cmd_ng_param.hpp"
#include "cmd_angle.hpp"
#include "cmd_compliance.hpp"
#include "cmd_feedback.hpp"
#include "cmd_servo_info.hpp"
#include "cmd_var.hpp"
#include "cmd_write_flash.hpp"
#include "cmd_gpio.hpp"
#include "cmd_pulse.hpp"
#include "cmd_ping.hpp"
#include "cmd_ik.hpp"
#include "cmd_walk.hpp"
#include "cmd_acceleration.hpp"
#include "cmd_voltage.hpp"
#include "cmd_raw.hpp"
#include "cmd_scan_bt.hpp"
#include "cmd_bind_bt.hpp"
#include "cmd_serial.hpp"
using namespace VSido;


/** コンストラクタ
* @param[in] raw httpサーバーからのJson要求
*  {
*	 "command" : コマンド,
*     コマンドに応じて変わる内容
*  }
*/
JSONRequestParser::JSONRequestParser(const string &json)
:_raw()
{
	DUMP_VAR(json);
	try
	{
		picojson::parse(_raw, json.c_str(),json.c_str() + json.size(),&err_);
		if(false == err_.empty())
		{
			FATAL_VAR(err_);
			FATAL_VAR(json);
		}
		DUMP_VAR(_raw);
		auto ser = _raw.serialize();
		DUMP_VAR(ser);
	}
	catch(const std::runtime_error &e)
	{
		FATAL_VAR(e.what());
	}
}





/** 要求のインスタンス作成する。
* @param None
* @return インスタンス
*/
shared_ptr<JSONRequest>JSONRequestParser::create(void)
{
	try
	{
		string cmdStr = "";
		picojson::object& _rawObj = _raw.get<picojson::object>();
		DUMP_VAR(&_rawObj);
		if(_raw.contains("command"))
		{
			cmdStr = _rawObj["command"].get<string>();
		}
		else if(_raw.contains("cmd"))
		{
			cmdStr = _rawObj["cmd"].get<string>();
		}
		else
		{
		}
		DUMP_VAR(cmdStr);

	
		if("servoAngle" == cmdStr || "SetServoAngle" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new AngleJSONRequest(_rawObj));
		}
		if("GetServoAngle" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new AngleGetJSONRequest(_rawObj));
		}
		else if("SetServoCompliance" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new ComplianceJSONRequest(_rawObj));
		}
		else if("SetServoMinMaxAngle" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new MinMaxAngleJSONRequest(_rawObj));
		}
		else if("SetFeedbackID" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new IDFeedbackJSONRequest(_rawObj));
		}
		else if("GetServoFeedback" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new FeedbackJSONRequest(_rawObj));
		}
		else if("servo_info" == cmdStr || "GetServoInfo" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new ServoInfoJSONRequest(_rawObj));
		}
		else if("SetVIDValue" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new VarSetJSONRequest(_rawObj));
		}
		else if("GetVIDValue" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new VarGetJSONRequest(_rawObj));
		}
		else if("WriteFlash" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new WriteFlashJSONRequest(_rawObj));
		}
		else if("SetGPIOConfig" == cmdStr || "SetGPIOValue" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new GpioJSONRequest(_rawObj));
		}
		else if("SetPWMPulseWidth" == cmdStr || "SetPWMPulse" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new PulseJSONRequest(_rawObj));
		}
		else if("CheckConnectedServo" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new PingJSONRequest(_rawObj));
		}
		else if("ik" == cmdStr )
		{
			return shared_ptr<JSONRequest>(new IKJSONRequest(_rawObj));
		}
		else if("SetIK" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new IKSetJSONRequest(_rawObj));
		}
		else if("GetIK" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new IKGetJSONRequest(_rawObj));
		}
		else if("walk" == cmdStr || "Walk" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new WalkJSONRequest(_rawObj));
		}
		else if("GetAcceleration" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new AccelerationJSONRequest(_rawObj));
		}
		else if("GetVoltage" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new VoltageJSONRequest(_rawObj));
		}
		else if("raw" == cmdStr || "Binary" == cmdStr || "Raw" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new RawJSONRequest(_rawObj));
		}
		else if("scan_bt" == cmdStr || "ScanBluetooth" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new ScanBTJSONRequest(_rawObj));
		}
		else if("bind_bt" == cmdStr|| "PairBluetooth" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new BindBTJSONRequest(_rawObj));
		}
		else if("ListSerialPort" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new ListSerialJSONRequest(_rawObj));
		}
		else if("SelectSerialPort" == cmdStr)
		{
			return shared_ptr<JSONRequest>(new SelectSerialJSONRequest(_rawObj));
		}
		else
		{
			return shared_ptr<JSONRequest>(new NGParamJSONRequest(_rawObj));
		}
	}
	catch(const std::runtime_error &e)
	{
		FATAL_VAR(e.what());
		picojson::object obj;
		return shared_ptr<JSONRequest>(new NGParamJSONRequest(obj));
	}
}

