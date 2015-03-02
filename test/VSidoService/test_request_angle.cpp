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
#include "vsido_request_acceleration.hpp"
#include "vsido_request_parser.hpp"
using namespace VSido;


#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test.hpp>


#include "test_dump.hpp"


BOOST_AUTO_TEST_CASE(test_case_RequestAngle_0)
{
	string json = 
	"  {"
	"    \"cmd\": \"servoAngle\","
	"    \"cycle\": 1,"
	"    \"servo\":["
	"      {\"sid\":2,\"angle\":100}"
	"    ]"
	"  }"
	"";
	RequestParser parser(json);
	
	/// uart data
	list<unsigned char> result = 
	{
/*		
		0xff,
		(unsigned char)'o',
		0x4,
*/
		1,

		2,
		_HBYTE(100),_LBYTE(100),
	};
	result.push_front(result.size()+3 +1);
	result.push_front((unsigned char)'o');
	result.push_front(0xff);
	
	unsigned char _sum = 0;
	for(auto data : result)
	{
		_sum ^= data;
	}
	result.push_back(_sum);
	dumpTestData(result);
	
	BOOST_CHECK(result == parser.conv());
}


BOOST_AUTO_TEST_CASE(test_case_RequestAngle_1)
{
	string json = 
	"  {"
	"    \"cmd\": \"servoAngle\","
	"    \"cycle\": 1,"
	"    \"servo\":["
	"      {\"sid\":1,\"angle\":1000}"
	"    ]"
	"  }"
	"";
	RequestParser parser(json);
	
	/// uart data
	list<unsigned char> result = 
	{
/*		
		0xff,
		(unsigned char)'o',
		0x4,
*/
		1,

		1,
		_HBYTE(1000),_LBYTE(1000),
	};
	result.push_front(result.size()+3 +1);
	result.push_front((unsigned char)'o');
	result.push_front(0xff);
	
	unsigned char _sum = 0;
	for(auto data : result)
	{
		_sum ^= data;
	}
	result.push_back(_sum);
	dumpTestData(result);
	
	BOOST_CHECK(result == parser.conv());
}



BOOST_AUTO_TEST_CASE(test_case_RequestAngle_2)
{
	string json = 
	"  {"
	"    \"cmd\": \"servoAngle\","
	"    \"cycle\": 100,"
	"    \"servo\":["
	"      {\"sid\":1,\"angle\":-1400},"
	"      {\"sid\":100,\"angle\":1400}"
	"    ]"
	"  }"
	"";
	RequestParser parser(json);
	
	/// uart data
	list<unsigned char> result = 
	{
/*		
		0xff,
		(unsigned char)'o',
		0x4,
*/
		100,

		1,_HBYTE(-1400),_LBYTE(-1400),

		100,_HBYTE(1400),_LBYTE(1400),
	};
	result.push_front(result.size()+3 +1);
	result.push_front((unsigned char)'o');
	result.push_front(0xff);
	
	unsigned char _sum = 0;
	for(auto data : result)
	{
		_sum ^= data;
	}
	result.push_back(_sum);
	dumpTestData(result);
	
	BOOST_CHECK(result == parser.conv());
}

BOOST_AUTO_TEST_CASE(test_case_RequestAngle_3)
{
	string json = 
	"  {"
	"    \"cmd\": \"servoAngle\","
	"    \"cycle\": 5,"
	"    \"servo\":["
	"      {\"sid\":5,\"angle\":-100},"
	"      {\"sid\":35,\"angle\":0},"
	"      {\"sid\":76,\"angle\":561},"
	"      {\"sid\":8,\"angle\":999}"
	"    ]"
	"  }"
	"";
	RequestParser parser(json);
	
	/// uart data
	list<unsigned char> result = 
	{
/*		
		0xff,
		(unsigned char)'o',
		0x4,
*/
		5,
		
		5,_HBYTE(-100),_LBYTE(-100),
		
		
		35,_HBYTE(0),_LBYTE(0),
		
		76,_HBYTE(561),_LBYTE(561),
		
		8,_HBYTE(999),_LBYTE(999),
		
	};
	result.push_front(result.size()+3 +1);
	result.push_front((unsigned char)'o');
	result.push_front(0xff);
	
	unsigned char _sum = 0;
	for(auto data : result)
	{
		_sum ^= data;
	}
	result.push_back(_sum);
	dumpTestData(result);
	
	BOOST_CHECK(result == parser.conv());
}



