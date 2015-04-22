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
#include "vsido_request_id_feedback.hpp"
using namespace VSido;
#include <string>
#include <iostream>
#include <cstddef>
using namespace std;

#include "debug.h"

/** コンストラクタ
* @param[in] raw httpサーバーからのJson要求
*  {
*	  "cmd" : "id_feedback",
*	  "SIDs"   : [
	   1~127
*	  ]
*  }
*/
RequestIDFeedback::RequestIDFeedback(picojson::object &raw)
:Request()
,_raw(raw)
{
	_cmd = (unsigned char)'f';
}

/** Json要求からVSidoパケットへ変換する
* @param None
* @return VSidoパケット
*/
list<unsigned char> RequestIDFeedback::conv(void)
{
	try
	{
	//	DUMP_VAR(_raw);
		auto sids = _raw["SIDs"].get<picojson::array>();
		for (auto it = sids.begin(); it != sids.end(); it++)
		{
			unsigned char sid = (unsigned char)it->get<double>();
			_uart.push_back((unsigned char)sid);
			_length++;
			DUMP_VAR(sid);
		}
	}
	catch(const std::runtime_error &e)
	{
		DUMP_VAR(e.what());
	}
	putCommon();
	return _uart;
}

