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
#include "cmd_scan_bt.hpp"
#include "cmd_bind_bt.hpp"
#include "cmd_parser.hpp"
using namespace VSido;


#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test.hpp>


#include "test_dump.hpp"
#include "test_uart.hpp"

#include <string.h>

extern bool popen_null;
extern char fgets_result[256];

#define CMD_RESULT(x)\
{\
	int size = sizeof(x) > sizeof(fgets_result) ? sizeof(fgets_result): sizeof(x);\
	memcpy(fgets_result,x,size);\
}

BOOST_AUTO_TEST_CASE(test_case_ScanBTJSONRequest_0)
{
	JSONRequestParser parser("{\"cmd\":\"scan_bt\"}");
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<ScanBTJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	popen_null = true;
	_request->exec();

	popen_null = false;
	_request->exec();
	
	popen_null = false;
	CMD_RESULT("edison\n");
	_request->exec();
	

	popen_null = false;
	CMD_RESULT("\t 08:00:27:d0:61:59 \t test_bt1 \n \t 08:00:27:88:e0:68 \t test_bt2 \n");
	auto ack = _request->exec();
	
	const string jsonAck("{\"devices\":[{\"mac\":\" 08:00:27:d0:61:59 \",\"name\":\" test_bt1 \"},{\"mac\":\" 08:00:27:88:e0:68 \",\"name\":\" test_bt2 \"}],\"type\":\"ScanBluetooth\"}");
	BOOST_CHECK_EQUAL(ack,jsonAck);
}



BOOST_AUTO_TEST_CASE(test_case_ScanBTJSONRequest_1)
{
	JSONRequestParser parser("{\"command\":\"ScanBluetooth\"}");
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<ScanBTJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	popen_null = true;
	_request->exec();

	popen_null = false;
	_request->exec();
	
	popen_null = false;
	CMD_RESULT("edison\n");
	_request->exec();
	

	popen_null = false;
	CMD_RESULT("\t 08:00:27:d0:61:59 \t test_bt1 \n \t 08:00:27:88:e0:68 \t test_bt2 \n");
	auto ack = _request->exec();
	
	const string jsonAck("{\"devices\":[{\"mac\":\" 08:00:27:d0:61:59 \",\"name\":\" test_bt1 \"},{\"mac\":\" 08:00:27:88:e0:68 \",\"name\":\" test_bt2 \"}],\"type\":\"ScanBluetooth\"}");
	BOOST_CHECK_EQUAL(ack,jsonAck);
}



BOOST_AUTO_TEST_CASE(test_case_BindBTJSONRequest_0)
{
	JSONRequestParser parser("{\"cmd\":\"bind_bt\",\"device\":{\"mac\":\"08:00:27:d0:61:59\"}}");
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<BindBTJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());

	popen_null = true;
	_request->exec();

	popen_null = false;
	auto ack = _request->exec();

	const string jsonAck("{\"type\":\"PairBluetooth\"}");
	BOOST_CHECK_EQUAL(ack,jsonAck);
}

BOOST_AUTO_TEST_CASE(test_case_BindBTJSONRequest_1)
{
	JSONRequestParser parser("{\"command\":\"PairBluetooth\",\"device\":{\"mac\":\"08:00:27:d0:61:59\"}}");
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<BindBTJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());

	popen_null = true;
	_request->exec();

	popen_null = false;
	auto ack = _request->exec();

	const string jsonAck("{\"type\":\"PairBluetooth\"}");
	BOOST_CHECK_EQUAL(ack,jsonAck);
}

