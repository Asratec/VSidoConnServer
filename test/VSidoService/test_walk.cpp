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
#include "cmd_walk.hpp"
#include "cmd_parser.hpp"
using namespace VSido;


#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test.hpp>


#include "test_dump.hpp"
#include "test_uart.hpp"


#define SHIFT_PERCENTAGE(x) (unsigned char)(x+100)


BOOST_AUTO_TEST_CASE(test_case_WalkJSONRequest_0)
{
	JSONRequestParser parser("{\"cmd\":\"Walk\",\"forward\":0,\"turnCW\":0}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0,2,
		SHIFT_PERCENTAGE(0),
		SHIFT_PERCENTAGE(0),
	};
	GEN_COMMON_SEND_UART('t');
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<WalkJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());

	DO_ACK_GOOD_CHECK();
}

BOOST_AUTO_TEST_CASE(test_case_WalkJSONRequest_1)
{
	JSONRequestParser parser("{\"cmd\":\"walk\",\"forward\":10,\"turn\":-20}");
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0,2,
		SHIFT_PERCENTAGE(10),
		SHIFT_PERCENTAGE(-20),
	};
	GEN_COMMON_SEND_UART('t');
	
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<WalkJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());

	DO_ACK_GOOD_CHECK();
}

