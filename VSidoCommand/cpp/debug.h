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
#ifndef __DEBUG_HPP__
#define __DEBUG_HPP__

#include <mutex>              // std::mutex, std::lock_guard
#include <chrono>
using namespace std;
extern mutex _globalLockMtx;
#include <unistd.h>
#include <sys/time.h>
#include <time.h>




#if 0
#define DUMP_UART_IO(x,msg) \
{\
	lock_guard<mutex> lock(_globalLockMtx);\
	printf("%s %s.size()=<%d><\n ",msg,#x,x.size());\
    for (const auto ch:x)\
    {\
    	printf("0x%02x,",ch);\
    }\
	printf(">\n");\
}
#else
#define DUMP_UART_IO(...)
#endif

#define DUMP_UART_IO_READ(x) DUMP_UART_IO(x,"read uart data")
#define DUMP_UART_IO_WRITE(x) DUMP_UART_IO(x,"write uart data")





#if 0
#define DUMP_SPEED_CHECK(msg,...) {\
	auto now = chrono::duration_cast<chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();\
	{\
		lock_guard<mutex> lock(_globalLockMtx);\
		printf("Time Stamp :[%0.6f] at %s\n", (double)now/(double)1000000,msg); \
	}\
}
#else
#define DUMP_SPEED_CHECK(...)
#endif

#if 0
#define DUMP_SPEED_CHECK_DETAILS(...) { \
    DUMP_SPEED_CHECK("perf details"); \
    {\
        lock_guard<mutex> lock(_globalLockMtx);\
        cout << __FILE__ ":" << __LINE__ << endl;\
    }\
}

#else
#define DUMP_SPEED_CHECK_DETAILS(...)
#endif


//#define DUMP_CMD_BINARY


#if 0
#define DUMP_QUE_SIZE FATAL_VAR 
#else
#define DUMP_QUE_SIZE(...)
#endif

#if 1
	#define FATAL_VAR(x) \
	{	\
		lock_guard<mutex> lock(_globalLockMtx);\
		cout << __FILE__ << ":" << __func__ << ":" << __LINE__ << " " << #x << "=<" << x <<  ">" << endl; \
	}
#else
	#define FATAL_VAR(x)
#endif


#if 0
	#define DUMP_VAR(x) \
	{	\
		lock_guard<mutex> lock(_globalLockMtx);\
		cout << __FILE__ << ":" << __func__ << ":" << __LINE__ << " " << #x << "=<" << x <<  ">" << endl; \
	}
#else
	#define DUMP_VAR(x)
#endif



#if 0
	#define DUMP_VAR_DETAILS(x) \
	{	\
		lock_guard<mutex> lock(_globalLockMtx);\
		cout << __FILE__ << ":" << __func__ << ":" << __LINE__ << " " << #x << "=<" << x <<  ">" << endl;\
	}
#else
	#define DUMP_VAR_DETAILS(x) 

#endif

#endif //__DEBUG_HPP__

