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
#include "vsido_response_common.hpp"
using namespace VSido;
#include <string>
#include <iostream>
using namespace std;


#include "debug.h"

/** コンストラクタ
*/
Response::Response()
:_jsonObj()
,_uart()
,_rawUart()
{
	
}

/** コンストラクタ
* @param[in] uart VSidoからの返事 共通
*/
Response::Response(const list<unsigned char> &uart)
:_jsonObj()
,_uart(uart)
,_rawUart(uart)
{
	DUMP_VAR(uart.size());
	DUMP_VAR(_uart.size());
	DUMP_VAR(_rawUart.size());
	if(false == _uart.empty())
	{
		_uart.pop_front();
	}
	if(false == _uart.empty())
	{
		_uart.pop_front();
	}
	if(false == _uart.empty())
	{
		_uart.pop_front();
	}
	if(false == _uart.empty())
	{
		_uart.pop_back();
	}
	DUMP_VAR(uart.size());
	DUMP_VAR(_uart.size());
	DUMP_VAR(_rawUart.size());
}


/** VSidoの返事をJsonに変換する
* @param None
* @return Jsonデータ
* {
* 	"type": "xxx",
* 	"raw": [0xff,...]
* 	・・・
* }	
*/
string Response::conv(void)
{
	DUMP_VAR(_rawUart.size());
	picojson::array rawArray;
	for(auto &raw :_rawUart)
	{
		double dRaw = (double)raw;
		rawArray.push_back(picojson::value(dRaw));
	}
	_jsonObj["raw"] = picojson::value(rawArray);

	picojson::value json(_jsonObj);
	return json.serialize();
}


