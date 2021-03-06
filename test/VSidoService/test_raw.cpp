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
#include "cmd_raw.hpp"
#include "cmd_parser.hpp"
using namespace VSido;


#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test.hpp>

#include <typeinfo>

#include "test_dump.hpp"
#include "test_uart.hpp"


BOOST_AUTO_TEST_CASE(test_case_RawJSONRequest_0)
{
	JSONRequestParser parser("{\"cmd\":\"Raw\",\"exec\":[255,34,4,85]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		255,34,4,85
	};
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<RawJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	
	list<unsigned char> uartAck = 
	{
		0xff,0x22,0x4,0x55
	};
	
	DUMMY_ACK(uartAck);
	
	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("{\"raw\":\"0xff,0x22,0x04,0x55\",\"type\":\"Binary\"}");
	BOOST_CHECK_EQUAL(ack,jsonAck);
}



BOOST_AUTO_TEST_CASE(test_case_RawJSONRequest_1)
{
	JSONRequestParser parser("{\"cmd\":\"Binary\",\"op\":120,\"bin\":[2,1]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		2,1
	};
	GEN_COMMON_SEND_UART(120);
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<RawJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	
	list<unsigned char> uartAck = 
	{
		0xff,0x22,0x4,0x55
	};
	
	DUMMY_ACK(uartAck);
	
	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("{\"raw\":\"0xff,0x22,0x04,0x55\",\"type\":\"Binary\"}");
	BOOST_CHECK_EQUAL(ack,jsonAck);
}


BOOST_AUTO_TEST_CASE(test_case_RawJSONRequest_2)
{
	JSONRequestParser parser("{\"cmd\":\"Binary\",\"op\":120,\"bin\":[null]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
	};
	GEN_COMMON_SEND_UART(120);
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<RawJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	
	list<unsigned char> uartAck = 
	{
		0xff,0x22,0x4,0x55
	};
	
	DUMMY_ACK(uartAck);
	
	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("{\"raw\":\"0xff,0x22,0x04,0x55\",\"type\":\"Binary\"}");
	BOOST_CHECK_EQUAL(ack,jsonAck);
}



BOOST_AUTO_TEST_CASE(test_case_RawJSONRequest_3)
{
	JSONRequestParser parser("{\"cmd\":\"raw\",\"exec\":[255,34,4,85]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		255,34,4,85
	};
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<RawJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	
	list<unsigned char> uartAck = 
	{
		0xff,0x22,0x4,0x55
	};
	
	DUMMY_ACK(uartAck);
	
	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("{\"raw\":\"0xff,0x22,0x04,0x55\",\"type\":\"Binary\"}");
	BOOST_CHECK_EQUAL(ack,jsonAck);
}

