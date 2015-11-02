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
#include "cmd_gpio.hpp"
#include "VSido.hpp"
using namespace VSido;
#include <string>
#include <iostream>
using namespace std;


#include "debug.h"


/** コンストラクタ
* @param[in] raw httpサーバーからのJson要求
*  {
*	  "cmd" : "SetGPIOValue",
*	  "gpio"    : [
*	    {"port":4~7,"val":0~1}
*	  ]
*  }
*/
GpioJSONRequest::GpioJSONRequest(picojson::object &raw)
:JSONRequest()
,_raw(raw)
{
}

/** コマンドを実行する。
* @param None
* @return 返事のJSON文字列
*/
string GpioJSONRequest::exec()
{
	GPIORequest req;
	SetVIDRequest setVid;
	try
	{
	//	DUMP_VAR(_raw);
		unsigned char iomodeVID = 0;
		auto sids = _raw["gpio"].get<picojson::array>();
		for (auto it = sids.begin(); it != sids.end(); it++)
		{
			auto &tmpObject = it->get<picojson::object>();
			
			unsigned char iid = (unsigned char)tmpObject["port"].get<double>();
			DUMP_VAR(iid);
			bool val =  (0 != (int)tmpObject["val"].get<double>());
			DUMP_VAR(val);
			req.gpio(iid,val);
			switch(iid)
			{
			case 4:
				{
					iomodeVID |= 0x08;
					break;
				}
			default:
				break;
			}
		}
		setVid.value("IO_PA_IO_Mode",1);
	}
	catch(const std::runtime_error &e)
	{
		FATAL_VAR(e.what());
	}
	if(setVid)
	{
		setVid.exec();
	}
	else
	{
		return setVid.msg();
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

