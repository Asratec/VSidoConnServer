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
#include "cmd_feedback.hpp"
#include "cmd_parser.hpp"
using namespace VSido;


#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test.hpp>


#include "test_dump.hpp"
#include "test_uart.hpp"





BOOST_AUTO_TEST_CASE(test_case_IDFeedbackJSONRequest_0)
{
	string json = 
	"  {"
	"    \"cmd\": \"SetFeedbackID\","
	"    \"servo\":["
	"      1,2,3,4,5"
	"    ]"
	"  }"
	"";
	JSONRequestParser parser(json);
	
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
/*		
		0xff,
		(unsigned char)'f',
		0x4,
*/
		1,2,3,4,5
	};
	GEN_COMMON_SEND_UART('f');
	dumpTestData(uartExpectedSend);
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<IDFeedbackJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);


	DO_ACK_GOOD_CHECK();

}















#define SERVO_DATA_1B(x,y,z) (z)
#define SERVO_DATA_2B(x,y,z) _HBYTE(z),_LBYTE(z)

BOOST_AUTO_TEST_CASE(test_case_FeedbackJSONRequest_0)
{
	string json = 
	"  {"
	"    \"cmd\": \"GetServoFeedback\","
	"    \"item\":["
	"    \"rom_servo_ID\",\"rom_cw_agl_lmt\" "
	"    ]"
	"  }"
	"";
	JSONRequestParser parser(json);
	
	/// uart data
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
/*		
		0xff,
		(unsigned char)'r',
		,,
*/
		2,3
	};
	GEN_COMMON_SEND_UART('r');
	dumpTestData(uartExpectedSend);
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<FeedbackJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);


	list<unsigned char> uartAck = 
	{
		1,
		SERVO_DATA_1B(3,rom_servo_ID,			10),
		SERVO_DATA_2B(3,rom_cw_agl_lmt,	156	),
	};
	GEN_COMMON_READ_UART('r');
	
	DUMMY_ACK(uartAck);
	
	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("{\"raw\":\"0xff,0x72,0x08,0x01,0x0a,0x38,0x02,0xb4\",\"servo\":[{\"rom_cw_agl_lmt\":15.6,\"rom_servo_ID\":10,\"sid\":1}],\"type\":\"GetServoFeedback\"}");
	BOOST_CHECK_EQUAL(ack,jsonAck);

}


BOOST_AUTO_TEST_CASE(test_case_FeedbackJSONRequest_0_0)
{
	string json = 
	"  {"
	"    \"command\": \"GetServoFeedback\","
	"    \"item\":["
	"    \"rom_servo_ID\",\"rom_cw_agl_lmt\" "
	"    ]"
	"  }"
	"";
	JSONRequestParser parser(json);
	
	/// uart data
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
/*		
		0xff,
		(unsigned char)'r',
		,,
*/
		2,3
	};
	GEN_COMMON_SEND_UART('r');
	dumpTestData(uartExpectedSend);
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<FeedbackJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);


	list<unsigned char> uartAck = 
	{
		1,
		SERVO_DATA_1B(3,rom_servo_ID,			10),
		SERVO_DATA_2B(3,rom_cw_agl_lmt,	156	),
	};
	GEN_COMMON_READ_UART('r');
	
	DUMMY_ACK(uartAck);
	
	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("{\"raw\":\"0xff,0x72,0x08,0x01,0x0a,0x38,0x02,0xb4\",\"servo\":[{\"rom_cw_agl_lmt\":15.6,\"rom_servo_ID\":10,\"sid\":1}],\"type\":\"GetServoFeedback\"}");
	BOOST_CHECK_EQUAL(ack,jsonAck);

}


BOOST_AUTO_TEST_CASE(test_case_FeedbackJSONRequest_1)
{
	string json = 
	"  {"
	"    \"command\": \"GetServoFeedback\","
	"    \"item\":["
	"    \"all\""
	"    ]"
	"  }"
	"";
	JSONRequestParser parser(json);
	
	/// uart data
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
/*		
		0xff,
		(unsigned char)'r',
		,,
*/
		0,53
	};
	GEN_COMMON_SEND_UART('r');
	dumpTestData(uartExpectedSend);
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<FeedbackJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);


	list<unsigned char> uartAck = 
	{
		1,
		SERVO_DATA_1B(3,rom_servo_ID,			10),
		SERVO_DATA_2B(3,rom_cw_agl_lmt,	156	),
	};
	GEN_COMMON_READ_UART('r');
	
	DUMMY_ACK(uartAck);
	
	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("{\"raw\":\"0xff,0x72,0x08,0x01,0x0a,0x38,0x02,0xb4\",\"servo\":[],\"type\":\"GetServoFeedback\"}");
	BOOST_CHECK_EQUAL(ack,jsonAck);

}

