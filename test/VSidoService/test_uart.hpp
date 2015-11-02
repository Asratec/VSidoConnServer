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
#pragma once

#include <list>
#include <map>
#include <chrono>
#include <tuple>
using namespace std;

extern list<unsigned char> ut_uart_send;

extern map<int,tuple<list<unsigned char>,chrono::milliseconds>> ut_uart_read;

#define DUMMY_ACK(x) {\
	ut_uart_read.clear(); \
	chrono::milliseconds dummy(0);\
	static int i = 0;\
	ut_uart_read[i++] = make_tuple(x,dummy);\
}


static const string jsonAck("{\"message\":\"\",\"raw\":\"0xff,0x21,0x04,0xda\",\"type\":\"ack\"}");

#define DO_ACK_GOOD_CHECK() \
{  \
	list<unsigned char> uartAck =  \
	{ \
		0xff, \
		(unsigned char)'!', \
		4, \
	};\
	{\
		unsigned char _sum = 0;\
		for(auto data : uartAck)\
		{\
			_sum ^= data;\
		}\
		uartAck.push_back(_sum);\
	}\
	DUMMY_ACK(uartAck);\
	auto ack = request->exec();\
	dumpTestData(ut_uart_send);\
	dumpTestData(uartExpectedSend);\
	BOOST_CHECK(ut_uart_send == uartExpectedSend);\
	DUMP_VAR(ack);\
	BOOST_CHECK_EQUAL(ack,jsonAck);\
}




