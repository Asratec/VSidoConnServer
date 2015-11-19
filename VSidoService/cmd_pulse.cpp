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
#include "cmd_pulse.hpp"
#include "VSido.hpp"
using namespace VSido;
#include <string>
#include <iostream>
using namespace std;


#include "debug.h"


/** コンストラクタ
* @param[in] raw httpサーバーからのJson要求
*  {
*	 "cmd" : "SetPWMPulse",
*	 "pwm"  : [
*	   {"port":6~7,"pul":0~16383}
*	 ]
*  }
*/
PulseJSONRequest::PulseJSONRequest(picojson::object &raw)
:JSONRequest()
,_raw(raw)
{
}

/** コマンドを実行する。
* @param None
* @return 返事のJSON文字列
*/
string PulseJSONRequest::exec()
{
	PWMRequest req;
	try
	{
		{
			SetVIDRequest setVid;
			setVid.value("IO_PA_PWM",1);
			if(setVid)
			{
				setVid.execNA();
			}
		}
	//	DUMP_VAR(_raw);
		auto servo = _raw["pwm"].get<picojson::array>();
		for (auto it = servo.begin(); it != servo.end(); it++)
		{
			auto &tmpObject = it->get<picojson::object>();
			unsigned char  iid = (unsigned char )tmpObject["port"].get<double>();
			DUMP_VAR(iid);
			if(it->contains("width"))
			{
				uint16_t width = (uint16_t)tmpObject["width"].get<double>();
				DUMP_VAR(width);
				// 4 us
				width /= 4;
				req.pluse(iid,width);
			}
			if(it->contains("duty"))
			{
				// read cycles first.
				GetVIDRequest varReq;
				varReq.vid("IO_PA_PWM_CYCLE");
				uint16_t pwm_cycle = 0;
				if(varReq)
				{
					auto ackVar = varReq.execBK();
					ackVar.forEachVar( [this,&pwm_cycle](const string &name,int value){
						if("IO_PA_PWM_CYCLE" == name)
						{
							pwm_cycle = (uint16_t)value;
						}
					}
					);
				}
				auto duty = tmpObject["duty"].get<double>();
				DUMP_VAR(duty);
				uint16_t width = (uint16_t)((duty)*(double)(pwm_cycle));
				req.pluse(iid,width);
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

