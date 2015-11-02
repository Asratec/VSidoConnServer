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
#ifndef __TEST_DUMP_HPP__
#define __TEST_DUMP_HPP__
#include <string>
#include <list>
using namespace std;


#define _HBYTE(x) (unsigned char)(((int)x <<1) &0xff)
#define _LBYTE(x) (unsigned char)(((((int)x <<1) >>8 ) &0xff)<< 1)

#define dumpTestData(x) dumpTestDataReal(#x,x)
void dumpTestDataReal(const string &msg,const list<unsigned char>&data);



#define DUMP_VAR(x) cout << __FILE__ " " << __LINE__ << ":" << #x << "=<" << x << ">" <<endl;



#define GEN_COMMON_SEND_UART(x) \
{ \
	uartExpectedSend.push_front(uartExpectedSend.size() + 3 +1); \
	uartExpectedSend.push_front((unsigned char)x); \
	/*uartExpectedSend.push_front((Request::uidCounter_)%200);*/ \
	/*uartExpectedSend.push_front((Request::uidCounter_+1)%200);*/ \
	uartExpectedSend.push_front(0xff); \
	unsigned char _sum = 0; \
	for(auto data : uartExpectedSend) \
	{ \
		_sum ^= data; \
	} \
	uartExpectedSend.push_back(_sum); \
}


#define GEN_COMMON_READ_UART(x) \
{ \
	uartAck.push_front(uartAck.size() + 3 +1); \
	uartAck.push_front((unsigned char)x); \
	/*uartAck.push_front((Request::uidCounter_)%200);*/ \
	/*uartAck.push_front((Request::uidCounter_+1)%200);*/ \
	uartAck.push_front(0xff); \
	unsigned char _sum = 0; \
	for(auto data : uartAck) \
	{ \
		_sum ^= data; \
	} \
	uartAck.push_back(_sum); \
}





#endif // __TEST_DUMP_HPP__

