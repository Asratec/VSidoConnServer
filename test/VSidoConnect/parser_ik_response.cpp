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






// ik add 100 with %
#define DATA(x,y,z) (100+x),(100+y),(100+z)

BOOST_AUTO_TEST_CASE(test_case_IKResponse_0)
{
	/// uart data
	list<unsigned char> recieved = 
	{
		0x38,
		1,DATA(77,23,30),DATA(17,52,34),DATA(26,58,44),
		2,DATA(77,23,30),DATA(26,58,44),DATA(17,52,34),
	};

	GEN_COMMON_READ_UART('k');
	DUMMY_ACK(recieved);

	IKReadRequest req;
	auto ack = req.exec();

	dumpTestData(sendBuffer);
	
	auto kdts = ack.kdt();

	DUMP_VAR(kdts.size());
	BOOST_CHECK_EQUAL(kdts.size(),2);
	
	for(auto const &kv:kdts)
	{
		DUMP_VAR((int)kv.first);
		if(1 == kv.first)
		{
			BOOST_CHECK_EQUAL((int)get<0>(kv.second),77);
			BOOST_CHECK_EQUAL((int)get<1>(kv.second),23);
			BOOST_CHECK_EQUAL((int)get<2>(kv.second),30);

			BOOST_CHECK_EQUAL((int)get<3>(kv.second),17);
			BOOST_CHECK_EQUAL((int)get<4>(kv.second),52);
			BOOST_CHECK_EQUAL((int)get<5>(kv.second),34);

			BOOST_CHECK_EQUAL((int)get<6>(kv.second),26);
			BOOST_CHECK_EQUAL((int)get<7>(kv.second),58);
			BOOST_CHECK_EQUAL((int)get<8>(kv.second),44);

		}
		if(2 == kv.first)
		{
			BOOST_CHECK_EQUAL((int)get<0>(kv.second),77);
			BOOST_CHECK_EQUAL((int)get<1>(kv.second),23);
			BOOST_CHECK_EQUAL((int)get<2>(kv.second),30);

			BOOST_CHECK_EQUAL((int)get<3>(kv.second),26);
			BOOST_CHECK_EQUAL((int)get<4>(kv.second),58);
			BOOST_CHECK_EQUAL((int)get<5>(kv.second),44);

			BOOST_CHECK_EQUAL((int)get<6>(kv.second),17);
			BOOST_CHECK_EQUAL((int)get<7>(kv.second),52);
			BOOST_CHECK_EQUAL((int)get<8>(kv.second),34);

		}
	}

}


BOOST_AUTO_TEST_CASE(test_case_IKResponse_1)
{
	/// uart data
	list<unsigned char> recieved = 
	{
		0x38,
		1,DATA(77,23,30),DATA(17,52,34),DATA(26,58,44),
		2,DATA(77,23,30),DATA(26,58,44),DATA(17,52,34),
	};

	GEN_COMMON_READ_UART('k');
	DUMMY_ACK(recieved);

	IKReadRequest req;
	auto ack = req.exec();

	BOOST_CHECK_EQUAL("" , ack.JSON());

}


BOOST_AUTO_TEST_CASE(test_case_IKResponse_2)
{
	/// uart data
	list<unsigned char> recieved = 
	{
		0x38,
		1,DATA(77,23,30),DATA(17,52,34),DATA(26,58,44),
		2,DATA(77,23,30),DATA(26,58,44),DATA(17,52,34),
	};

	GEN_COMMON_READ_UART('k');
	DUMMY_ACK(recieved);

	IKReadRequest req;
	auto ack = req.exec();

	BOOST_CHECK_EQUAL(true , ack.position());
	BOOST_CHECK_EQUAL(true , ack.torque());
	BOOST_CHECK_EQUAL(true , ack.rotation());

}


BOOST_AUTO_TEST_CASE(test_case_IKResponse_3)
{
	/// uart data
	list<unsigned char> recieved = 
	{
	};

	GEN_COMMON_READ_UART('k');
	DUMMY_ACK(recieved);

	IKReadRequest req;
	auto ack = req.exec();

	BOOST_CHECK_EQUAL(false , ack.position());
	BOOST_CHECK_EQUAL(false , ack.torque());
	BOOST_CHECK_EQUAL(false , ack.rotation());

}

BOOST_AUTO_TEST_CASE(test_case_IKResponse_4)
{
	/// uart data
	list<unsigned char> recieved = 
	{
		0x38,
	};

	GEN_COMMON_READ_UART('k');
	DUMMY_ACK(recieved);

	IKReadRequest req;
	auto ack = req.exec();

	BOOST_CHECK_EQUAL(true , true);

}


BOOST_AUTO_TEST_CASE(test_case_IKResponse_5)
{
	/// uart data
	list<unsigned char> recieved = 
	{
		0x38,
		1,DATA(77,23,30),DATA(17,52,34),DATA(26,58,44),
		2,DATA(77,23,30),DATA(26,58,44),DATA(17,52,34),
	};

	GEN_COMMON_READ_UART('k');
	DUMMY_ACK(recieved);

	IKReadRequest req;
	auto ack = req.exec();

	dumpTestData(sendBuffer);
	
	ack.forEach([](unsigned char kid,tuple<signed char,signed char,signed char,signed char,signed char,signed char,signed char,signed char,signed char> kdt){
		if(1 == kid)
		{
			BOOST_CHECK_EQUAL((int)get<0>(kdt),77);
			BOOST_CHECK_EQUAL((int)get<1>(kdt),23);
			BOOST_CHECK_EQUAL((int)get<2>(kdt),30);

			BOOST_CHECK_EQUAL((int)get<3>(kdt),17);
			BOOST_CHECK_EQUAL((int)get<4>(kdt),52);
			BOOST_CHECK_EQUAL((int)get<5>(kdt),34);

			BOOST_CHECK_EQUAL((int)get<6>(kdt),26);
			BOOST_CHECK_EQUAL((int)get<7>(kdt),58);
			BOOST_CHECK_EQUAL((int)get<8>(kdt),44);

		}
		if(2 == kid)
		{
			BOOST_CHECK_EQUAL((int)get<0>(kdt),77);
			BOOST_CHECK_EQUAL((int)get<1>(kdt),23);
			BOOST_CHECK_EQUAL((int)get<2>(kdt),30);

			BOOST_CHECK_EQUAL((int)get<3>(kdt),26);
			BOOST_CHECK_EQUAL((int)get<4>(kdt),58);
			BOOST_CHECK_EQUAL((int)get<5>(kdt),44);

			BOOST_CHECK_EQUAL((int)get<6>(kdt),17);
			BOOST_CHECK_EQUAL((int)get<7>(kdt),52);
			BOOST_CHECK_EQUAL((int)get<8>(kdt),34);

		}
		
	});
}


