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



BOOST_AUTO_TEST_CASE(test_case_JointResponse_0)
{

#define DATA_DIM(x,y) (x),(y)
	/// uart data
	list<unsigned char> recieved = 
	{
		DATA_DIM(2,3),
		DATA_DIM(3,30),
		DATA_DIM(4,35),
		DATA_DIM(14,235),
		DATA_DIM(154,123),
	};
	GEN_COMMON_READ_UART('j');
	DUMMY_ACK(recieved);

	JointRequest req;
	auto ack = req.execBK();

	dumpTestData(sendBuffer);
	
	auto tim = ack.tim();

	DUMP_VAR(tim.size());
	BOOST_CHECK_EQUAL(tim.size(),5);

#define CHECK_TIM(x,y) \
{\
	if(x==sid)\
	{\
		BOOST_CHECK_EQUAL(y,value);\
	}\
}

	
	for(auto t :tim)
	{
		auto sid = std::get<0>(t);
		auto value = std::get<1>(t);
		DUMP_VAR((int)sid);
		DUMP_VAR((int)value);

		CHECK_TIM(2,3);
		CHECK_TIM(3,30);
		CHECK_TIM(4,35);
		CHECK_TIM(14,235);
		CHECK_TIM(154,123);

	}
	BOOST_CHECK_EQUAL("" , ack.JSON());
}


BOOST_AUTO_TEST_CASE(test_case_JointResponse_1)
{

#define DATA_DIM(x,y) (x),(y)
	/// uart data
	list<unsigned char> recieved = 
	{
		DATA_DIM(2,3),
		DATA_DIM(3,30),
		DATA_DIM(4,35),
		DATA_DIM(14,235),
		DATA_DIM(154,123),
	};
	GEN_COMMON_READ_UART('j');
	DUMMY_ACK(recieved);

	JointRequest req;
	auto ack = req.execBK();

	dumpTestData(sendBuffer);

#define CHECK_TIM(x,y) \
{\
	if(x==sid)\
	{\
		BOOST_CHECK_EQUAL(y,value);\
	}\
}
	
	ack.forEach([](int sid,int value){
		CHECK_TIM(2,3);
		CHECK_TIM(3,30);
		CHECK_TIM(4,35);
		CHECK_TIM(14,235);
		CHECK_TIM(154,123);
	});
}

