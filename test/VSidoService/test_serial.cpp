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
#include "cmd_serial.hpp"
#include "cmd_parser.hpp"
using namespace VSido;


#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test.hpp>


#include "test_dump.hpp"
#include "test_uart.hpp"

#include <string.h>

extern bool popen_null;
extern char fgets_result[256];

extern char system_result[256];


#define CMD_RESULT(x)\
{\
	int size = sizeof(x) > sizeof(fgets_result) ? sizeof(fgets_result): sizeof(x);\
	memcpy(fgets_result,x,size);\
}

BOOST_AUTO_TEST_CASE(test_case_ListSerialJSONRequest_0)
{
	JSONRequestParser parser("{\"command\":\"ListSerialPort\"}");
	
	
	auto request = parser.create();
	DUMP_VAR(request);
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<ListSerialJSONRequest>(request);
	DUMP_VAR(_request);
	DUMP_VAR(typeid(_request).name());
	
	
	popen_null = true;
	_request->exec();

	popen_null = false;
	_request->exec();
	
	popen_null = false;
	CMD_RESULT("edison\n");
	auto ack = _request->exec();
	

	
	const string jsonAck("{\"port\":[\"\\/dev\\/ttyMFD0\",\"\\/dev\\/ttyMFD1\",\"\\/dev\\/ttyMFD2\"],\"type\":\"ListSerialPort\"}");
	BOOST_CHECK_EQUAL(ack,jsonAck);
}


BOOST_AUTO_TEST_CASE(test_case_ListSerialJSONRequest_1)
{
	JSONRequestParser parser("{\"command\":\"ListSerialPort\"}");
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<ListSerialJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	popen_null = true;
	_request->exec();

	popen_null = false;
	_request->exec();
	
	popen_null = false;
	CMD_RESULT("raspberrypi\n");
	auto ack = _request->exec();
	

	
	const string jsonAck("{\"port\":[\"\\/dev\\/ttyAMA0\"],\"type\":\"ListSerialPort\"}");
	BOOST_CHECK_EQUAL(ack,jsonAck);
}




BOOST_AUTO_TEST_CASE(test_case_SelectSerialJSONRequest_0)
{
	JSONRequestParser parser("{\"command\":\"SelectSerialPort\",\"port\":\"tty00\",\"baudrate\":\"115200\"}");
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<SelectSerialJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());

	popen_null = true;
	_request->exec();

	popen_null = false;
	auto ack = _request->exec();

	const string jsonAck("{\"type\":\"SelectSerialPort\"}");
	BOOST_CHECK_EQUAL(ack,jsonAck);
}


BOOST_AUTO_TEST_CASE(test_case_SelectSerialJSONRequest_1)
{
	JSONRequestParser parser("{\"command\":\"SelectSerialPort\",\"port\":\"tty00\",\"baudrate\":\"115200\"}");
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<SelectSerialJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());

	popen_null = true;
	_request->exec();

	popen_null = false;
	CMD_RESULT("edison\n");
	_request->exec();

	popen_null = false;
	CMD_RESULT("raspberrypi\n");
	_request->exec();
}

