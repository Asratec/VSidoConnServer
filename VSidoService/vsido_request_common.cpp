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
#include "vsido_request_common.hpp"
#include "vsido_response_common.hpp"
#include "vsido_response_ack.hpp"
using namespace VSido;

#include <string>
#include <iostream>
using namespace std;



#include "debug.h"


/** コンストラクタ
* @param None
*/
Request::Request()
:_st(0xff)
,_cmd(0x0)
,_length(0x4)
,_sum(0x0)
,_uart{}
,_expect(nullptr)
//,_expect(new ResponseAck({0xff}))
{
}


/** Json要求からVSidoパケットへ変換する
* @param None
* @return VSidoパケット
*/
list<unsigned char> Request::conv(void)
{
	list<unsigned char> ret;
	return ret;
}

/** Json要求で、実行する。
* @param None
* @return VSidoパケット
*/
string Request::runTask(void)
{
	return "";
}


/** VSidoパケット共通の部分を生成する
* @param None
* @return None
*/
void Request::putCommon()
{
	_uart.push_front(_length);
	_uart.push_front(_cmd);	
	_uart.push_front(_st);
	/// do check sum
	for(auto data : _uart)
	{
		_sum ^= data;
	}
	_uart.push_back(_sum);
	dumpUart();
	
}


void Request::dumpUart()
{
	DUMP_VAR(&_uart);
#if 0 // no debug
	for(auto ch:_uart)
	{
		printf("0x%02x,",ch);
	}
	cout << endl;
#endif 
}
