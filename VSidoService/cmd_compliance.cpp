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
#include "cmd_compliance.hpp"
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
*	  "cmd" : "SetServoCompliance",
*	  "comp"   : [
	   {"sid":1~254,"cpCW":1~254,"cpCCW":1~254}
*	  ]
*  }
*/
ComplianceJSONRequest::ComplianceJSONRequest(picojson::object &raw)
:JSONRequest()
,_raw(raw)
{
}

/** コマンドを実行する。
* @param None
* @return 返事のJSON文字列
*/
string ComplianceJSONRequest::exec()
{
	ComplianceRequest req;
	try
	{
	//	DUMP_VAR(_raw);
		auto servo = _raw["servo"].get<picojson::array>();
		for (auto it = servo.begin(); it != servo.end(); it++)
		{
			auto &tmpObject = it->get<picojson::object>();
			unsigned char sid = (unsigned char)tmpObject["sid"].get<double>();
			DUMP_VAR(sid);
			unsigned char cpCW = (unsigned char)tmpObject["cpCW"].get<double>();
			DUMP_VAR(cpCW);
			unsigned char cpCCW = (unsigned char)tmpObject["cpCCW"].get<double>();
			DUMP_VAR(cpCCW);
			req.comp(sid,cpCW,cpCCW);
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

