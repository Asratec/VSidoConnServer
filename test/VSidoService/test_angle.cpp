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
#include "cmd_angle.hpp"
#include "cmd_parser.hpp"
using namespace VSido;


#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test.hpp>


#include "test_dump.hpp"
#include "test_uart.hpp"



#define _CYCLE(x) x/10


BOOST_AUTO_TEST_CASE(test_case_RequestAngle_0)
{
	string json = 
	"  {"
	"    \"cmd\": \"SetServoAngle\","
	"    \"cycle\": 11,"
	"    \"servo\":["
	"      {\"sid\":2,\"angle\":10}"
	"    ]"
	"  }"
	"";
	JSONRequestParser parser(json);
	
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
/*		
		0xff,
		(unsigned char)'o',
		0x4,
*/
		_CYCLE(11),
		2,
		_HBYTE(100),_LBYTE(100),
	};
	uartExpectedSend.push_front(uartExpectedSend.size()+3 +1);
	uartExpectedSend.push_front((unsigned char)'o');
	uartExpectedSend.push_front(0xff);
	
	unsigned char _sum = 0;
	for(auto data : uartExpectedSend)
	{
		_sum ^= data;
	}
	uartExpectedSend.push_back(_sum);
	dumpTestData(uartExpectedSend);
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<AngleJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);

	DO_ACK_GOOD_CHECK();


}


BOOST_AUTO_TEST_CASE(test_case_RequestAngle_1)
{
	string json = 
	"  {"
	"    \"cmd\": \"SetServoAngle\","
	"    \"cycle\": 10,"
	"    \"servo\":["
	"      {\"sid\":1,\"angle\":100}"
	"    ]"
	"  }"
	"";
	JSONRequestParser parser(json);
	
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
/*		
		0xff,
		(unsigned char)'o',
		0x4,
*/
		_CYCLE(10),

		1,
		_HBYTE(1000),_LBYTE(1000),
	};
	uartExpectedSend.push_front(uartExpectedSend.size()+3 +1);
	uartExpectedSend.push_front((unsigned char)'o');
	uartExpectedSend.push_front(0xff);
	
	unsigned char _sum = 0;
	for(auto data : uartExpectedSend)
	{
		_sum ^= data;
	}
	uartExpectedSend.push_back(_sum);
	dumpTestData(uartExpectedSend);
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<AngleJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);


	DO_ACK_GOOD_CHECK();

}



BOOST_AUTO_TEST_CASE(test_case_RequestAngle_2)
{
	string json = 
	"  {"
	"    \"cmd\": \"SetServoAngle\","
	"    \"cycle\": 1000,"
	"    \"servo\":["
	"      {\"sid\":1,\"angle\":-140},"
	"      {\"sid\":100,\"angle\":140}"
	"    ]"
	"  }"
	"";
	JSONRequestParser parser(json);
	
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
/*		
		0xff,
		(unsigned char)'o',
		0x4,
*/
		_CYCLE(1000),

		1,_HBYTE(-1400),_LBYTE(-1400),

		100,_HBYTE(1400),_LBYTE(1400),
	};
	uartExpectedSend.push_front(uartExpectedSend.size()+3 +1);
	uartExpectedSend.push_front((unsigned char)'o');
	uartExpectedSend.push_front(0xff);
	
	unsigned char _sum = 0;
	for(auto data : uartExpectedSend)
	{
		_sum ^= data;
	}
	uartExpectedSend.push_back(_sum);
	dumpTestData(uartExpectedSend);
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<AngleJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);


	DO_ACK_GOOD_CHECK();
}

BOOST_AUTO_TEST_CASE(test_case_RequestAngle_3)
{
	string json = 
	"  {"
	"    \"cmd\": \"SetServoAngle\","
	"    \"cycle\": 51,"
	"    \"servo\":["
	"      {\"sid\":5,\"angle\":-10},"
	"      {\"sid\":35,\"angle\":0},"
	"      {\"sid\":76,\"angle\":56.1},"
	"      {\"sid\":8,\"angle\":99.9}"
	"    ]"
	"  }"
	"";
	JSONRequestParser parser(json);
	
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
/*		
		0xff,
		(unsigned char)'o',
		0x4,
*/
		_CYCLE(51),
		
		5,_HBYTE(-100),_LBYTE(-100),
		
		
		35,_HBYTE(0),_LBYTE(0),
		
		76,_HBYTE(561),_LBYTE(561),
		
		8,_HBYTE(999),_LBYTE(999),
		
	};
	uartExpectedSend.push_front(uartExpectedSend.size()+3 +1);
	uartExpectedSend.push_front((unsigned char)'o');
	uartExpectedSend.push_front(0xff);
	
	unsigned char _sum = 0;
	for(auto data : uartExpectedSend)
	{
		_sum ^= data;
	}
	uartExpectedSend.push_back(_sum);
	dumpTestData(uartExpectedSend);
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<AngleJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);


	DO_ACK_GOOD_CHECK();

}





#define SERVO_DATA_1B(x,y,z) (z)
#define SERVO_DATA_2B(x,y,z) _HBYTE(z),_LBYTE(z)


BOOST_AUTO_TEST_CASE(test_case_AngleGetJSONRequest_0)
{
	string json = 
	"  {"
	"    \"cmd\": \"GetServoAngle\","
	"    \"servo\":["
	"      1,"
	"      2"
	"    ]"
	"  }"
	"";
	JSONRequestParser parser(json);
	
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
/*		
		0xff,
		(unsigned char)'d',
		,,
*/
		1,19,2,
		2,19,2
	};
	GEN_COMMON_SEND_UART('d');
	dumpTestData(uartExpectedSend);
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<AngleGetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);


	list<unsigned char> uartAck = 
	{
		1,
		SERVO_DATA_2B(19,ram_pres_pos,			1288),
		2,
		SERVO_DATA_2B(19,ram_pres_pos,			-840),
	};
	GEN_COMMON_READ_UART('d');
	
	DUMMY_ACK(uartAck);
	
	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("{\"raw\":\"0xff,0x64,0x0a,0x01,0x10,0x14,0x02,0x70,0xf2,0x14\",\"servo\":[{\"angle\":128.80000000000001,\"sid\":1},{\"angle\":-84,\"sid\":2}],\"type\":\"GetServoAngle\"}");
	BOOST_CHECK_EQUAL(ack,jsonAck);

}









BOOST_AUTO_TEST_CASE(test_case_MinMaxAngleJSONRequest_0)
{
	string json = 
	"  {"
	"    \"cmd\": \"SetServoMinMaxAngle\","
	"    \"servo\":["
	"      {\"sid\":2,\"min\":1.0,\"max\":13.0},"
	"      {\"sid\":1,\"min\":5.5,\"max\":24.5} "
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
		(unsigned char)'m',
		,,
*/
		2,_HBYTE(130),_LBYTE(130),_HBYTE(10),_LBYTE(10),
		1,_HBYTE(245),_LBYTE(245),_HBYTE(55),_LBYTE(55)
	};
	GEN_COMMON_SEND_UART('m');
	dumpTestData(uartExpectedSend);
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<MinMaxAngleJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	DO_ACK_GOOD_CHECK();

}


