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
#include "cmd_ik.hpp"
#include "cmd_parser.hpp"
using namespace VSido;


#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test.hpp>

#include <typeinfo>

#include "test_dump.hpp"
#include "test_uart.hpp"



#define DATA(x,y,z) (x+100),(y+100),(z+100)

BOOST_AUTO_TEST_CASE(test_case_IKSetJSONRequest_0)
{
	JSONRequestParser parser("{\"cmd\":\"SetIK\",\"ikflag\":{\"position\":true,\"rotation\":true,\"torque\":true},\"kdt\":[{\"kid\":1,\"position\":{\"x\":77,\"y\":23,\"z\":30},\"rotation\":{\"x\":17,\"y\":52,\"z\":34},\"torque\":{\"x\":26,\"y\":58,\"z\":44}}]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x7,
		1,
		DATA(77,23,30),
		DATA(17,52,34),
		DATA(26,58,44),
	};
	GEN_COMMON_SEND_UART('k');
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKSetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	
	DO_ACK_GOOD_CHECK();
}


BOOST_AUTO_TEST_CASE(test_case_IKSetJSONRequest_1)
{
	JSONRequestParser parser("{\"cmd\":\"SetIK\",\"ikflag\":{\"position\":true,\"rotation\":true,\"torque\":true},\"kdt\":[{\"position\":{\"x\":77,\"y\":23,\"z\":30},\"rotation\":{\"x\":17,\"y\":52,\"z\":34},\"torque\":{\"x\":26,\"y\":58,\"z\":44}}]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x7,
	};
	GEN_COMMON_SEND_UART('k');
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKSetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	
	DO_ACK_GOOD_CHECK();
}


BOOST_AUTO_TEST_CASE(test_case_IKSetJSONRequest_2)
{
	JSONRequestParser parser("{\"cmd\":\"SetIK\",\"ikflag\":{\"position\":true,\"rotation\":true,\"torque\":true},\"kdt\":[{\"kid\":1}]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x7,
	};
	GEN_COMMON_SEND_UART('k');
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKSetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	
	DO_ACK_GOOD_CHECK();
}

BOOST_AUTO_TEST_CASE(test_case_IKSetJSONRequest_3)
{
	JSONRequestParser parser("{\"cmd\":\"SetIK\",\"ikflag\":{\"position\":true,\"rotation\":true,\"torque\":true},\"kdt\":[{\"kid\":\"body\",\"position\":{\"x\":77,\"y\":23,\"z\":30},\"rotation\":{\"x\":17,\"y\":52,\"z\":34},\"torque\":{\"x\":26,\"y\":58,\"z\":44}}]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x7,
		0,
		DATA(77,23,30),
		DATA(17,52,34),
		DATA(26,58,44),
	};
	GEN_COMMON_SEND_UART('k');
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKSetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	
	DO_ACK_GOOD_CHECK();
}

BOOST_AUTO_TEST_CASE(test_case_IKSetJSONRequest_4)
{
	JSONRequestParser parser("{\"cmd\":\"SetIK\",\"ikflag\":{\"position\":true,\"rotation\":true,\"torque\":true},\"kdt\":[{\"kid\":\"head\",\"position\":{\"x\":77,\"y\":23,\"z\":30},\"rotation\":{\"x\":17,\"y\":52,\"z\":34},\"torque\":{\"x\":26,\"y\":58,\"z\":44}}]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x7,
		1,
		DATA(77,23,30),
		DATA(17,52,34),
		DATA(26,58,44),
	};
	GEN_COMMON_SEND_UART('k');
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKSetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	
	DO_ACK_GOOD_CHECK();
}

BOOST_AUTO_TEST_CASE(test_case_IKSetJSONRequest_5)
{
	JSONRequestParser parser("{\"cmd\":\"SetIK\",\"ikflag\":{\"position\":true,\"rotation\":true,\"torque\":true},\"kdt\":[{\"kid\":\"right_hand\",\"position\":{\"x\":77,\"y\":23,\"z\":30},\"rotation\":{\"x\":17,\"y\":52,\"z\":34},\"torque\":{\"x\":26,\"y\":58,\"z\":44}}]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x7,
		2,
		DATA(77,23,30),
		DATA(17,52,34),
		DATA(26,58,44),
	};
	GEN_COMMON_SEND_UART('k');
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKSetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	
	DO_ACK_GOOD_CHECK();
}


BOOST_AUTO_TEST_CASE(test_case_IKSetJSONRequest_6)
{
	JSONRequestParser parser("{\"cmd\":\"SetIK\",\"ikflag\":{\"position\":true,\"rotation\":true,\"torque\":true},\"kdt\":[{\"kid\":\"left_hand\",\"position\":{\"x\":77,\"y\":23,\"z\":30},\"rotation\":{\"x\":17,\"y\":52,\"z\":34},\"torque\":{\"x\":26,\"y\":58,\"z\":44}}]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x7,
		3,
		DATA(77,23,30),
		DATA(17,52,34),
		DATA(26,58,44),
	};
	GEN_COMMON_SEND_UART('k');
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKSetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	
	DO_ACK_GOOD_CHECK();
}

BOOST_AUTO_TEST_CASE(test_case_IKSetJSONRequest_7)
{
	JSONRequestParser parser("{\"cmd\":\"SetIK\",\"ikflag\":{\"position\":true,\"rotation\":true,\"torque\":true},\"kdt\":[{\"kid\":\"right_foot\",\"position\":{\"x\":77,\"y\":23,\"z\":30},\"rotation\":{\"x\":17,\"y\":52,\"z\":34},\"torque\":{\"x\":26,\"y\":58,\"z\":44}}]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x7,
		4,
		DATA(77,23,30),
		DATA(17,52,34),
		DATA(26,58,44),
	};
	GEN_COMMON_SEND_UART('k');
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKSetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	
	DO_ACK_GOOD_CHECK();
}


BOOST_AUTO_TEST_CASE(test_case_IKSetJSONRequest_8)
{
	JSONRequestParser parser("{\"cmd\":\"SetIK\",\"ikflag\":{\"position\":true,\"rotation\":true,\"torque\":true},\"kdt\":[{\"kid\":\"left_foot\",\"position\":{\"x\":77,\"y\":23,\"z\":30},\"rotation\":{\"x\":17,\"y\":52,\"z\":34},\"torque\":{\"x\":26,\"y\":58,\"z\":44}}]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x7,
		5,
		DATA(77,23,30),
		DATA(17,52,34),
		DATA(26,58,44),
	};
	GEN_COMMON_SEND_UART('k');
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKSetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	
	DO_ACK_GOOD_CHECK();
}



BOOST_AUTO_TEST_CASE(test_case_IKSetJSONRequest_9)
{
	JSONRequestParser parser("{\"command\":\"SetIK\",\"ikflag\":{\"position\":true,\"torque\":false,\"rotation\":false},\"kdt\":[{\"kid\":\"right_hand\",\"position\":{\"x\":0,\"y\":0,\"z\":-99},\"torque\":{\"x\":0,\"y\":0,\"z\":0},\"rotation\":{\"x\":0,\"y\":0,\"z\":0}},{\"kid\":\"left_hand\",\"position\":{\"x\":0,\"y\":0,\"z\":-99},\"torque\":{\"x\":0,\"y\":0,\"z\":0},\"rotation\":{\"x\":0,\"y\":0,\"z\":0}},{\"kid\":\"right_foot\",\"position\":{\"x\":-2,\"y\":0,\"z\":-99},\"torque\":{\"x\":0,\"y\":0,\"z\":0},\"rotation\":{\"x\":0,\"y\":0,\"z\":0}},{\"kid\":\"left_foot\",\"position\":{\"x\":2,\"y\":0,\"z\":-99},\"torque\":{\"x\":0,\"y\":0,\"z\":0},\"rotation\":{\"x\":0,\"y\":0,\"z\":0}}]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x1,
		2,
		DATA(0,0,-99),
		3,
		DATA(0,0,-99),
		4,
		DATA(-2,0,-99),
		5,
		DATA(2,0,-99),
	};
	GEN_COMMON_SEND_UART('k');
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKSetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	
	DO_ACK_GOOD_CHECK();
}



BOOST_AUTO_TEST_CASE(test_case_IKSetJSONRequest_10)
{
	JSONRequestParser parser("{\"command\":\"SetIK\",\"ikflag\":{\"position\":true,\"torque\":false,\"rotation\":false},\"kdt\":[{\"kid\":\"head\",\"position\":{\"x\":0,\"y\":0,\"z\":-99},\"torque\":{\"x\":0,\"y\":0,\"z\":0},\"rotation\":{\"x\":0,\"y\":0,\"z\":0}},{\"kid\":\"right_hand\",\"position\":{\"x\":0,\"y\":0,\"z\":-99},\"torque\":{\"x\":0,\"y\":0,\"z\":0},\"rotation\":{\"x\":0,\"y\":0,\"z\":0}},{\"kid\":\"left_hand\",\"position\":{\"x\":0,\"y\":0,\"z\":-99},\"torque\":{\"x\":0,\"y\":0,\"z\":0},\"rotation\":{\"x\":0,\"y\":0,\"z\":0}},{\"kid\":\"right_foot\",\"position\":{\"x\":-2,\"y\":0,\"z\":-99},\"torque\":{\"x\":0,\"y\":0,\"z\":0},\"rotation\":{\"x\":0,\"y\":0,\"z\":0}},{\"kid\":\"left_foot\",\"position\":{\"x\":2,\"y\":0,\"z\":-99},\"torque\":{\"x\":0,\"y\":0,\"z\":0},\"rotation\":{\"x\":0,\"y\":0,\"z\":0}}]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x1,
		1,
		DATA(0,0,-99),
		2,
		DATA(0,0,-99),
		3,
		DATA(0,0,-99),
		4,
		DATA(-2,0,-99),
		5,
		DATA(2,0,-99),
	};
	GEN_COMMON_SEND_UART('k');
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKSetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	
	DO_ACK_GOOD_CHECK();
}


BOOST_AUTO_TEST_CASE(test_case_IKSetJSONRequest_11)
{
	JSONRequestParser parser("{\"command\":\"SetIK\",\"ikflag\":{\"position\":true,\"rotation\":false,\"torque\":false},\"kdt\":[{\"kid\":\"head\",\"position\":{\"x\":0,\"y\":0,\"z\":-99},\"rotation\":{\"x\":0,\"y\":0,\"z\":0},\"torque\":{\"x\":0,\"y\":0,\"z\":0}},{\"kid\":\"right_hand\",\"position\":{\"x\":0,\"y\":0,\"z\":-99},\"rotation\":{\"x\":0,\"y\":0,\"z\":0},\"torque\":{\"x\":0,\"y\":0,\"z\":0}},{\"kid\":\"left_hand\",\"position\":{\"x\":0,\"y\":0,\"z\":-99},\"rotation\":{\"x\":0,\"y\":0,\"z\":0},\"torque\":{\"x\":0,\"y\":0,\"z\":0}},{\"kid\":\"right_foot\",\"position\":{\"x\":-2,\"y\":0,\"z\":-99},\"rotation\":{\"x\":0,\"y\":0,\"z\":0},\"torque\":{\"x\":0,\"y\":0,\"z\":0}},{\"kid\":\"left_foot\",\"position\":{\"x\":2,\"y\":0,\"z\":-99},\"rotation\":{\"x\":0,\"y\":0,\"z\":0},\"torque\":{\"x\":0,\"y\":0,\"z\":0}}]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x1,
		1,
		DATA(0,0,-99),
		2,
		DATA(0,0,-99),
		3,
		DATA(0,0,-99),
		4,
		DATA(-2,0,-99),
		5,
		DATA(2,0,-99),
	};
	GEN_COMMON_SEND_UART('k');
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKSetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	
	DO_ACK_GOOD_CHECK();
}



BOOST_AUTO_TEST_CASE(test_case_IKGetJSONRequest_0)
{
	JSONRequestParser parser("{\"cmd\":\"GetIK\",\"ikflag\":{\"position\":true,\"rotation\":true,\"torque\":true},\"kid\":[1,2]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x38,
		1,
		2,
	};
	GEN_COMMON_SEND_UART('k');
	dumpTestData(uartExpectedSend);
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKGetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());

	/// uart data
	list<unsigned char> uartAck = 
	{
		0x38,
		0,DATA(77,20,30),DATA(17,-51,34),DATA(26,58,44),
		1,DATA(77,21,31),DATA(17,-52,34),DATA(26,58,44),
		2,DATA(27,22,32),DATA(26,-53,44),DATA(17,52,34),
		3,DATA(37,23,33),DATA(17,-54,34),DATA(26,58,44),
		4,DATA(47,24,34),DATA(26,-55,44),DATA(17,52,34),
		5,DATA(57,25,35),DATA(26,-56,44),DATA(17,52,34),
		6,DATA(67,26,36),DATA(26,-57,44),DATA(17,52,34),
	};

	GEN_COMMON_READ_UART('k');
	DUMMY_ACK(uartAck);

	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("{\"ikflag\":{\"position\":true,\"rotation\":true,\"torque\":true},\"kdt\":[{\"kid\":\"body\",\"position\":{\"x\":77,\"y\":20,\"z\":30},\"rotation\":{\"x\":17,\"y\":-51,\"z\":34},\"torque\":{\"x\":26,\"y\":58,\"z\":44}},{\"kid\":\"head\",\"position\":{\"x\":77,\"y\":21,\"z\":31},\"rotation\":{\"x\":17,\"y\":-52,\"z\":34},\"torque\":{\"x\":26,\"y\":58,\"z\":44}},{\"kid\":\"right_hand\",\"position\":{\"x\":27,\"y\":22,\"z\":32},\"rotation\":{\"x\":26,\"y\":-53,\"z\":44},\"torque\":{\"x\":17,\"y\":52,\"z\":34}},{\"kid\":\"left_hand\",\"position\":{\"x\":37,\"y\":23,\"z\":33},\"rotation\":{\"x\":17,\"y\":-54,\"z\":34},\"torque\":{\"x\":26,\"y\":58,\"z\":44}},{\"kid\":\"right_foot\",\"position\":{\"x\":47,\"y\":24,\"z\":34},\"rotation\":{\"x\":26,\"y\":-55,\"z\":44},\"torque\":{\"x\":17,\"y\":52,\"z\":34}},{\"kid\":\"left_foot\",\"position\":{\"x\":57,\"y\":25,\"z\":35},\"rotation\":{\"x\":26,\"y\":-56,\"z\":44},\"torque\":{\"x\":17,\"y\":52,\"z\":34}},{\"kid\":6,\"position\":{\"x\":67,\"y\":26,\"z\":36},\"rotation\":{\"x\":26,\"y\":-57,\"z\":44},\"torque\":{\"x\":17,\"y\":52,\"z\":34}}],\"raw\":\"0xff,0x6b,0x4b,0x38,0x00,0xb1,0x78,0x82,0x75,0x31,0x86,0x7e,0x9e,0x90,0x01,0xb1,0x79,0x83,0x75,0x30,0x86,0x7e,0x9e,0x90,0x02,0x7f,0x7a,0x84,0x7e,0x2f,0x90,0x75,0x98,0x86,0x03,0x89,0x7b,0x85,0x75,0x2e,0x86,0x7e,0x9e,0x90,0x04,0x93,0x7c,0x86,0x7e,0x2d,0x90,0x75,0x98,0x86,0x05,0x9d,0x7d,0x87,0x7e,0x2c,0x90,0x75,0x98,0x86,0x06,0xa7,0x7e,0x88,0x7e,0x2b,0x90,0x75,0x98,0x86,0xe0\",\"type\":\"GetIK\"}");
	BOOST_CHECK_EQUAL(ack,jsonAck);
	
}





BOOST_AUTO_TEST_CASE(test_case_IKGetJSONRequest_1)
{
	JSONRequestParser parser("{\"cmd\":\"GetIK\",\"ikflag\":{\"position\":true,\"rotation\":true,\"torque\":true},\"kid\":[\"body\",\"head\",\"right_hand\",\"left_hand\",\"right_foot\",\"left_foot\",9]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x38,
		0,
		1,
		2,
		3,
		4,
		5,
		9
	};
	GEN_COMMON_SEND_UART('k');
	dumpTestData(uartExpectedSend);
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKGetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());

	/// uart data
	list<unsigned char> uartAck = 
	{
		0x38,
		0,DATA(77,20,30),DATA(17,-51,34),DATA(26,58,44),
		1,DATA(77,21,31),DATA(17,-52,34),DATA(26,58,44),
		2,DATA(27,22,32),DATA(26,-53,44),DATA(17,52,34),
		3,DATA(37,23,33),DATA(17,-54,34),DATA(26,58,44),
		4,DATA(47,24,34),DATA(26,-55,44),DATA(17,52,34),
		5,DATA(57,25,35),DATA(26,-56,44),DATA(17,52,34),
		6,DATA(67,26,36),DATA(26,-57,44),DATA(17,52,34),
	};

	GEN_COMMON_READ_UART('k');
	DUMMY_ACK(uartAck);

	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("{\"ikflag\":{\"position\":true,\"rotation\":true,\"torque\":true},\"kdt\":[{\"kid\":\"body\",\"position\":{\"x\":77,\"y\":20,\"z\":30},\"rotation\":{\"x\":17,\"y\":-51,\"z\":34},\"torque\":{\"x\":26,\"y\":58,\"z\":44}},{\"kid\":\"head\",\"position\":{\"x\":77,\"y\":21,\"z\":31},\"rotation\":{\"x\":17,\"y\":-52,\"z\":34},\"torque\":{\"x\":26,\"y\":58,\"z\":44}},{\"kid\":\"right_hand\",\"position\":{\"x\":27,\"y\":22,\"z\":32},\"rotation\":{\"x\":26,\"y\":-53,\"z\":44},\"torque\":{\"x\":17,\"y\":52,\"z\":34}},{\"kid\":\"left_hand\",\"position\":{\"x\":37,\"y\":23,\"z\":33},\"rotation\":{\"x\":17,\"y\":-54,\"z\":34},\"torque\":{\"x\":26,\"y\":58,\"z\":44}},{\"kid\":\"right_foot\",\"position\":{\"x\":47,\"y\":24,\"z\":34},\"rotation\":{\"x\":26,\"y\":-55,\"z\":44},\"torque\":{\"x\":17,\"y\":52,\"z\":34}},{\"kid\":\"left_foot\",\"position\":{\"x\":57,\"y\":25,\"z\":35},\"rotation\":{\"x\":26,\"y\":-56,\"z\":44},\"torque\":{\"x\":17,\"y\":52,\"z\":34}},{\"kid\":6,\"position\":{\"x\":67,\"y\":26,\"z\":36},\"rotation\":{\"x\":26,\"y\":-57,\"z\":44},\"torque\":{\"x\":17,\"y\":52,\"z\":34}}],\"raw\":\"0xff,0x6b,0x4b,0x38,0x00,0xb1,0x78,0x82,0x75,0x31,0x86,0x7e,0x9e,0x90,0x01,0xb1,0x79,0x83,0x75,0x30,0x86,0x7e,0x9e,0x90,0x02,0x7f,0x7a,0x84,0x7e,0x2f,0x90,0x75,0x98,0x86,0x03,0x89,0x7b,0x85,0x75,0x2e,0x86,0x7e,0x9e,0x90,0x04,0x93,0x7c,0x86,0x7e,0x2d,0x90,0x75,0x98,0x86,0x05,0x9d,0x7d,0x87,0x7e,0x2c,0x90,0x75,0x98,0x86,0x06,0xa7,0x7e,0x88,0x7e,0x2b,0x90,0x75,0x98,0x86,0xe0\",\"type\":\"GetIK\"}");
	BOOST_CHECK_EQUAL(ack,jsonAck);
	
}
















BOOST_AUTO_TEST_CASE(test_case_IKJSONRequest_0)
{
	JSONRequestParser parser("{\"cmd\":\"ik\",\"ikf\":{\"dist\":{\"torq\":false,\"pos\":true,\"rot\":false}},\"kdts\":[{\"kid\":3,\"kdt\":{\"pos\":{\"x\":55,\"y\":-29,\"z\":0},\"rot\":{\"Rx\":0,\"Ry\":0,\"Rz\":0},\"torq\":{\"Tx\":0,\"Ty\":0,\"Tz\":0}}}]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x1,
		3,
		DATA(55,-29,0),
	};
	GEN_COMMON_SEND_UART('k');
	dumpTestData(uartExpectedSend);
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());

	DO_ACK_GOOD_CHECK();
	
}


BOOST_AUTO_TEST_CASE(test_case_IKJSONRequest_1)
{
	JSONRequestParser parser("{\"cmd\":\"ik\",\"ikf\":{\"cur\":{\"torq\":true,\"pos\":true,\"rot\":true}},\"kids\":[0,1,2]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x38,
		0,1,2,
	};
	GEN_COMMON_SEND_UART('k');
	dumpTestData(uartExpectedSend);
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());

	/// uart data
	list<unsigned char> uartAck = 
	{
		0x38,
		1,DATA(77,23,30),DATA(17,52,34),DATA(26,58,44),
		2,DATA(77,23,30),DATA(26,58,44),DATA(17,52,34),
	};

	GEN_COMMON_READ_UART('k');
	DUMMY_ACK(uartAck);

	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("");
	BOOST_CHECK_EQUAL(ack,jsonAck);
	
}


BOOST_AUTO_TEST_CASE(test_case_IKJSONRequest_2)
{
	JSONRequestParser parser("{\"cmd\":\"ik\",\"ikf\":{\"cur\":{\"torq\":false,\"pos\":true,\"rot\":true}},\"kids\":[0,1,2]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x18,
		0,1,2,
	};
	GEN_COMMON_SEND_UART('k');
	dumpTestData(uartExpectedSend);
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());

	/// uart data
	list<unsigned char> uartAck = 
	{
		0x38,
		1,DATA(77,23,30),DATA(17,52,34),DATA(26,58,44),
		2,DATA(77,23,30),DATA(26,58,44),DATA(17,52,34),
	};

	GEN_COMMON_READ_UART('k');
	DUMMY_ACK(uartAck);

	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("");
	BOOST_CHECK_EQUAL(ack,jsonAck);
	
}

BOOST_AUTO_TEST_CASE(test_case_IKJSONRequest_3)
{
	JSONRequestParser parser("{\"cmd\":\"ik\",\"ikf\":{\"cur\":{\"torq\":true,\"pos\":false,\"rot\":true}},\"kids\":[0,1,2]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x30,
		0,1,2,
	};
	GEN_COMMON_SEND_UART('k');
	dumpTestData(uartExpectedSend);
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());

	/// uart data
	list<unsigned char> uartAck = 
	{
		0x38,
		1,DATA(77,23,30),DATA(17,52,34),DATA(26,58,44),
		2,DATA(77,23,30),DATA(26,58,44),DATA(17,52,34),
	};

	GEN_COMMON_READ_UART('k');
	DUMMY_ACK(uartAck);

	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("");
	BOOST_CHECK_EQUAL(ack,jsonAck);
	
}


BOOST_AUTO_TEST_CASE(test_case_IKJSONRequest_4)
{
	JSONRequestParser parser("{\"cmd\":\"ik\",\"ikf\":{\"cur\":{\"torq\":true,\"pos\":true,\"rot\":false}},\"kids\":[0,1,2]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x28,
		0,1,2,
	};
	GEN_COMMON_SEND_UART('k');
	dumpTestData(uartExpectedSend);
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());

	/// uart data
	list<unsigned char> uartAck = 
	{
		0x38,
		1,DATA(77,23,30),DATA(17,52,34),DATA(26,58,44),
		2,DATA(77,23,30),DATA(26,58,44),DATA(17,52,34),
	};

	GEN_COMMON_READ_UART('k');
	DUMMY_ACK(uartAck);

	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("");
	BOOST_CHECK_EQUAL(ack,jsonAck);
	
}


BOOST_AUTO_TEST_CASE(test_case_IKJSONRequest_5)
{
	JSONRequestParser parser("{\"cmd\":\"ik\",\"ikf\":{\"cur\":{\"torq\":false,\"pos\":false,\"rot\":false}},\"kids\":[0,1,2]}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x00,
		0,1,2,
	};
	GEN_COMMON_SEND_UART('k');
	dumpTestData(uartExpectedSend);
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IKJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());

	/// uart data
	list<unsigned char> uartAck = 
	{
		0x38,
		0,DATA(77,20,30),DATA(17,-51,34),DATA(26,58,44),
		1,DATA(77,21,31),DATA(17,-52,34),DATA(26,58,44),
		2,DATA(27,22,32),DATA(26,-53,44),DATA(17,52,34),
		3,DATA(37,23,33),DATA(17,-54,34),DATA(26,58,44),
		4,DATA(47,24,34),DATA(26,-55,44),DATA(17,52,34),
		5,DATA(57,25,35),DATA(26,-56,44),DATA(17,52,34),
		6,DATA(67,26,36),DATA(26,-57,44),DATA(17,52,34),
	};

	GEN_COMMON_READ_UART('k');
	DUMMY_ACK(uartAck);

	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("");
	BOOST_CHECK_EQUAL(ack,jsonAck);
	
}

