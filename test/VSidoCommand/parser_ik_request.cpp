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



#define DATA(x,y,z) (x),(y),(z)

BOOST_AUTO_TEST_CASE(test_case_IKSetRequest_0)
{
	
	/// uart data
	list<unsigned char> expected = 
	{
		0x7,
		1,
		DATA(77,23,30),
		DATA(17,52,34),
		DATA(26,58,44),
	};
	GEN_COMMON_SEND_UART('k');
	dumpTestData(expected);

	IKSetRequest req;
	req.position();
	req.rotation();
	req.torque();

	req.position(1,77,23,30);
	req.rotation(1,17,52,34);
	req.torque(1,26,58,44);
	
	auto ack = req.execBK();
	dumpTestData(sendBuffer);
	BOOST_CHECK(expected == sendBuffer);
	
}




BOOST_AUTO_TEST_CASE(test_case_IKSetRequest_1)
{
	
	/// uart data
	list<unsigned char> expected = 
	{
		0x7,
		1,
		DATA(77,23,30),
		DATA(17,52,34),
		DATA(26,58,44),
	};
	GEN_COMMON_SEND_UART('k');
	dumpTestData(expected);

	IKSetRequest req;
	req.rotation();
	req.position();
	req.torque();

	req.rotation(1,17,52,34);
	req.position(1,77,23,30);
	req.torque(1,26,58,44);
	
	auto ack = req.execBK();
	dumpTestData(sendBuffer);
	BOOST_CHECK(expected == sendBuffer);
	
}


BOOST_AUTO_TEST_CASE(test_case_IKSetRequest_2)
{
	
	/// uart data
	list<unsigned char> expected = 
	{
		0x7,
		1,
		DATA(77,23,30),
		DATA(17,52,34),
		DATA(26,58,44),
	};
	GEN_COMMON_SEND_UART('k');
	dumpTestData(expected);

	IKSetRequest req;
	req.torque();
	req.rotation();
	req.position();

	req.torque(1,26,58,44);
	req.rotation(1,17,52,34);
	req.position(1,77,23,30);
	
	auto ack = req.execBK();
	dumpTestData(sendBuffer);
	BOOST_CHECK(expected == sendBuffer);
	
}


BOOST_AUTO_TEST_CASE(test_case_IKSetRequest_3)
{
	
	/// uart data
	list<unsigned char> expected = 
	{
		0x0,
	};
	GEN_COMMON_SEND_UART('k');
	dumpTestData(expected);

	IKSetRequest req;

	req.position(17,26,58,44);
	req.position(17,17,52,34);
	req.position(17,77,23,30);

	
	req.position(1,-101,0,0);
	req.position(1,101,0,0);
	req.position(1,0,-101,0);
	req.position(1,0,101,0);
	req.position(1,0,0,-101);
	req.position(1,0,0,101);
	
	auto ack = req.execBK();
	dumpTestData(sendBuffer);
	BOOST_CHECK(expected == sendBuffer);
	
}



BOOST_AUTO_TEST_CASE(test_case_IKReadRequest_0)
{
	
	/// uart data
	list<unsigned char> expected = 
	{
		0x38,
		1,
		2,
	};
	GEN_COMMON_SEND_UART('k');
	dumpTestData(expected);

	IKReadRequest req;
	req.position();
	req.rotation();
	req.torque();

	req.kid(1);
	req.kid(2);
	
	auto ack = req.execBK();
	dumpTestData(sendBuffer);
	BOOST_CHECK(expected == sendBuffer);
	
}



