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


#define ACCE(x) (unsigned char)( (double)32*(double)(x) + 128 )

BOOST_AUTO_TEST_CASE(test_case_AccelerationResponse_0)
{
	
	/// uart data
	list<unsigned char> recieved = 
	{
		ACCE(2.5),ACCE(1.2),ACCE(-0.3),
	};
	GEN_COMMON_READ_UART('a');
	DUMMY_ACK(recieved);

	AccelerationRequest req;
	auto ack = req.execBK();
	dumpTestData(sendBuffer);

	DUMP_VAR(ack.x());
	DUMP_VAR(ack.y());
	DUMP_VAR(ack.z());
	
	BOOST_CHECK_CLOSE(ack.x(),2.5,5);
	BOOST_CHECK_CLOSE(ack.y(),1.2,5);
	BOOST_CHECK_CLOSE(ack.z(),-0.3,5);
}



BOOST_AUTO_TEST_CASE(test_case_AccelerationResponse_1)
{
	
	/// uart data
	list<unsigned char> recieved = 
	{
		ACCE(2.5),ACCE(1.2),ACCE(-0.3),
	};
	GEN_COMMON_READ_UART('a');
	DUMMY_ACK(recieved);

	AccelerationRequest req;
	auto ack = req.execBK();

	BOOST_CHECK_EQUAL("" , ack.JSON());
}


