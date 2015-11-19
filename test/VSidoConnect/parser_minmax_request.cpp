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




BOOST_AUTO_TEST_CASE(test_case_MinMaxAngleRequest_0)
{
	
	/// uart data
	list<unsigned char> expected = 
	{
		2,
		_HBYTE((int16_t)(169.1*10)),_LBYTE((int16_t)(169.1*10)),
		_HBYTE((int16_t)(10.5*10)),_LBYTE((int16_t)(10.5*10)),
	};
	GEN_COMMON_SEND_UART('m');
	dumpTestData(expected);

	MinMaxAngleRequest req;
	req.minMax(2,10.5,169.1);
	auto ack = req.execBK();
	dumpTestData(sendBuffer);
	BOOST_CHECK(expected == sendBuffer);
	
}


BOOST_AUTO_TEST_CASE(test_case_MinMaxAngleRequest_1)
{
	
	/// uart data
	list<unsigned char> expected = 
	{
		2,
		_HBYTE((int16_t)(180.0*10)),_LBYTE((int16_t)(180.0*10)),
		_HBYTE((int16_t)(-180.0*10)),_LBYTE((int16_t)(-180.0*10)),
	};
	GEN_COMMON_SEND_UART('m');
	dumpTestData(expected);

	MinMaxAngleRequest req;
	req.minMax(2,-180.0,180.0);
	auto ack = req.execBK();
	dumpTestData(sendBuffer);
	BOOST_CHECK(expected == sendBuffer);
	
}

BOOST_AUTO_TEST_CASE(test_case_MinMaxAngleRequest_2)
{
	
	/// uart data
	list<unsigned char> expected = 
	{
		2,
		_HBYTE((int16_t)(-180.0*10)),_LBYTE((int16_t)(-180.0*10)),
		_HBYTE((int16_t)(180.0*10)),_LBYTE((int16_t)(180.0*10)),
	};
	GEN_COMMON_SEND_UART('m');
	dumpTestData(expected);

	MinMaxAngleRequest req;
	req.minMax(2,180.0,-180.0);
	auto ack = req.execBK();
	dumpTestData(sendBuffer);
	BOOST_CHECK(expected == sendBuffer);
	
}


BOOST_AUTO_TEST_CASE(test_case_MinMaxAngleRequest_3)
{
	
	/// uart data
	list<unsigned char> expected = 
	{
		1,
		_HBYTE((int16_t)(0*10)),_LBYTE((int16_t)(0*10)),
		_HBYTE((int16_t)(0*10)),_LBYTE((int16_t)(0*10)),
		254,
		_HBYTE((int16_t)(0*10)),_LBYTE((int16_t)(0*10)),
		_HBYTE((int16_t)(0*10)),_LBYTE((int16_t)(0*10)),
	};
	GEN_COMMON_SEND_UART('m');
	dumpTestData(expected);

	MinMaxAngleRequest req;
	req.minMax(1,0,0);
	req.minMax(254,0,0);
	auto ack = req.execBK();
	dumpTestData(sendBuffer);
	BOOST_CHECK(expected == sendBuffer);
	
}


BOOST_AUTO_TEST_CASE(test_case_MinMaxAngleRequest_4)
{
	
	/// uart data
	list<unsigned char> expected = 
	{
	};
	GEN_COMMON_SEND_UART('m');
	dumpTestData(expected);

	MinMaxAngleRequest req;
	req.minMax(0,0,0);
	req.minMax(255,0,0);
	auto ack = req.execBK();
	dumpTestData(sendBuffer);
	BOOST_CHECK(expected == sendBuffer);
	
}


BOOST_AUTO_TEST_CASE(test_case_MinMaxAngleRequest_5)
{
	
	/// uart data
	list<unsigned char> expected = 
	{
	};
	GEN_COMMON_SEND_UART('m');
	dumpTestData(expected);

	MinMaxAngleRequest req;
	req.minMax(1,-180.2,0.);
	req.minMax(1,180.2,0.);
	req.minMax(1,0.,180.2);
	req.minMax(1,0.,-180.2);
	auto ack = req.execBK();
	dumpTestData(sendBuffer);
	BOOST_CHECK(expected == sendBuffer);
	
}


