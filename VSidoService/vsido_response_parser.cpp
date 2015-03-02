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
using namespace std;


#include "debug.h"


#include "vsido_response_parser.hpp"
#include "vsido_response_ng_param.hpp"
#include "vsido_response_ack.hpp"
#include "vsido_response_acceleration.hpp"
#include "vsido_response_ik.hpp"
#include "vsido_response_feedback.hpp"
#include "vsido_response_ping.hpp"
#include "vsido_response_servo_info.hpp"
#include "vsido_response_var_get.hpp"
#include "vsido_response_voltage.hpp"
using namespace VSido;


/** コンストラクタ
* @param[in] uart VSidoからの返事
*/
ResponseParser::ResponseParser(const list<unsigned char> &uart)
:_uart(uart)
{
	dumpUart();
}

/** VSidoの返事をJsonに変換する
* @param expect 予期の返事
* @return Jsonデータ
* {
* 	"type": "xxx",
* 	・・・
* }	
*/
string ResponseParser::conv(shared_ptr<VSido::Response> expect)
{
	auto response = create();
	response->setExpect(expect);
	auto ret = response->conv();
	DUMP_VAR(ret);
	return ret;
}

void ResponseParser::dumpUart(void)
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




unique_ptr<Response> ResponseParser::create(void)
{
	if(4 > _uart.size())
	{
		return unique_ptr<Response>(new ResponseNGParam(_uart));
	}
	list<unsigned char> cmd = _uart;
	cmd.pop_front();
	char response = (char)cmd.front();
	DUMP_VAR(response);
	if('!' == response)
	{
		return unique_ptr<Response>(new ResponseAck(_uart));
	}
	else if('k' == response)
	{
		return unique_ptr<Response>(new ResponseIK(_uart));
	}
	else if('a' == response)
	{
		return unique_ptr<Response>(new ResponseAcceleration(_uart));
	}
	else if('r' == response)
	{
		return unique_ptr<Response>(new ResponseFeedback(_uart));
	}
	else if('d' == response)
	{
		return unique_ptr<Response>(new ResponseServoInfo(_uart));
	}
	else if('g' == response)
	{
		return unique_ptr<Response>(new ResponseVarGet(_uart));
	}
	else if('v' == response)
	{
		return unique_ptr<Response>(new ResponseVoltage(_uart));
	}
	else
	{
		return unique_ptr<Response>(new ResponseNGParam(_uart));
	}
}

