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
#include "VSido.hpp"
using namespace VSido;


#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test.hpp>


#include "parser_dump.hpp"



BOOST_AUTO_TEST_CASE(test_case_AckResponse_0)
{

	/// uart data
	list<unsigned char> recieved = 
	{
		'A','N','G','L','E'
	};
	GEN_COMMON_READ_UART('!');
	DUMMY_ACK(recieved);
	
	
	AngleRequest req;
	req.cycle(10);
	req.angle(2,100);
	auto ack = req.execBK();

	dumpTestData(sendBuffer);
	
	DUMP_VAR(ack.msg());
	BOOST_CHECK("ANGLE" == ack.msg());
}

BOOST_AUTO_TEST_CASE(test_case_AckResponse_2)
{

	/// uart data
	list<unsigned char> recieved = 
	{
		'C','O','M','P','L','I','A','N','C','E'
	};
	GEN_COMMON_READ_UART('!');
	DUMMY_ACK(recieved);
	
	
	ComplianceRequest req;
	req.comp(2,10,20);
	auto ack = req.execBK();

	dumpTestData(sendBuffer);
	
	DUMP_VAR(ack.msg());
	BOOST_CHECK("COMPLIANCE" == ack.msg());
}

BOOST_AUTO_TEST_CASE(test_case_AckResponse_3)
{
	/// uart data
	list<unsigned char> recieved = 
	{
		'S','E','T','V','I','D'
	};
	GEN_COMMON_READ_UART('!');
	DUMMY_ACK(recieved);
	
	
	SetVIDRequest req;
	req.value(0,10);
	auto ack = req.execBK();

	dumpTestData(sendBuffer);
	
	DUMP_VAR(ack.msg());
	BOOST_CHECK("SETVID" == ack.msg());
}

BOOST_AUTO_TEST_CASE(test_case_AckResponse_4)
{
	/// uart data
	list<unsigned char> recieved = 
	{
		'S','E','T','V','I','D'
	};
	GEN_COMMON_READ_UART('!');
	DUMMY_ACK(recieved);
	
	
	SetVIDRequest req;
	req.value(0,10);
	auto ack = req.execBK();

	dumpTestData(sendBuffer);
	
	BOOST_CHECK_EQUAL("" , ack.JSON());
}





