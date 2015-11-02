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
#include "cmd_acceleration.hpp"
#include "cmd_parser.hpp"
using namespace VSido;


#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test.hpp>

#include <typeinfo>

#include "test_dump.hpp"
#include "test_uart.hpp"


BOOST_AUTO_TEST_CASE(test_case_RequestAcceleration_1)
{
	JSONRequestParser parser("{\"cmd\":\"GetAcceleration\"]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0xff,
		(unsigned char)'a',
		4,
	};
	{
		unsigned char _sum = 0;
		for(auto data : uartExpectedSend)
		{
			_sum ^= data;
		}
		uartExpectedSend.push_back(_sum);
	}
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<AccelerationJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	
	
	
	list<unsigned char> uartAck = 
	{
		0xff,
		(unsigned char)'a',
		7,
		100,230,171
	};
	{
		unsigned char _sum = 0;
		for(auto data : uartAck)
		{
			_sum ^= data;
		}
		uartAck.push_back(_sum);
	}
	
	DUMMY_ACK(uartAck);
	
	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("{\"ax\":-0.875,\"ay\":3.1875,\"az\":1.34375,\"raw\":\"0xff,0x61,0x07,0x64,0xe6,0xab,0xb0\",\"type\":\"GetAcceleration\"}");
	BOOST_CHECK_EQUAL(ack,jsonAck);
}


BOOST_AUTO_TEST_CASE(test_case_RequestAcceleration_2)
{
	JSONRequestParser parser("{\"cmd\":\"getacceleration\"]}");
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<AccelerationJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr == _request);
}

BOOST_AUTO_TEST_CASE(test_case_RequestAcceleration_3)
{
	JSONRequestParser parser("{\"cmd\":\"etAcceleration\"]}");
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<AccelerationJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr == _request);
}


