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
#include <list>
#include <map>
#include <chrono>
#include <tuple>
using namespace std;


#define _HBYTE(x) (unsigned char)(((int)x <<1) &0xff)
#define _LBYTE(x) (unsigned char)(((((int)x <<1) >>8 ) &0xff)<< 1)

#define dumpTestData(x) dumpTestDataReal(__FILE__,__LINE__, "@"  #x,x)
void dumpTestDataReal(const string &file,int line,const string &msg,const list<unsigned char>&data);


extern list<unsigned char> sendBuffer;
//extern list<unsigned char> readBuffer;

extern unsigned int testUIDCounter;


#define GEN_COMMON_SEND_UART(x) \
{ \
	expected.push_front(expected.size() + 3 +1); \
	expected.push_front((unsigned char)x); \
	/*expected.push_front((Request::uidCounter_)%200);*/ \
	/*expected.push_front((Request::uidCounter_+1)%200);*/ \
	expected.push_front(0xff); \
	unsigned char _sum = 0; \
	for(auto data : expected) \
	{ \
		_sum ^= data; \
	} \
	expected.push_back(_sum); \
}


#define GEN_COMMON_READ_UART(x) \
{ \
	recieved.push_front(recieved.size() + 3 +1); \
	recieved.push_front((unsigned char)x); \
	/*recieved.push_front((Request::uidCounter_)%200);*/ \
	/*recieved.push_front((Request::uidCounter_+1)%200);*/ \
	recieved.push_front(0xff); \
	unsigned char _sum = 0; \
	for(auto data : recieved) \
	{ \
		_sum ^= data; \
	} \
	recieved.push_back(_sum); \
}

#define DUMP_VAR(x) \
{\
	cout << __FILE__ << "@" <<#x << ":=<"<< x << ">" << endl;\
}





extern map<int,tuple<list<unsigned char>,chrono::milliseconds>> ut_uart_read;

#define DUMMY_ACK(x) {\
	ut_uart_read.clear(); \
	chrono::milliseconds dummy(0);\
	static int i = 0;\
	ut_uart_read[i++] = make_tuple(x,dummy);\
}


#endif // __TEST_DUMP_HPP__

