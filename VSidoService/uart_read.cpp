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
#include "uart_read.hpp"
#include "dispatch.hpp"
using namespace VSido;

#include <unistd.h>
#include <stdio.h>
#include <termios.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>              // std::mutex
using namespace std;

#include <sys/time.h>

#include "debug.h"


#ifdef DEBUB_READ
	#define DUMP_VAR_READ(x)\
	printf("%s:%s:%d %s=<0x%02x>\n",__FILE__ , __func__ , __LINE__ ,#x ,x);
#else
	#define DUMP_VAR_READ(x) 
#endif


extern long long globalSendUartTime;






/** コンストラクタ
* @param dis 返事の分配器
*/
UARTRead::UARTRead(Dispatcher &dis)
:_fd(-1)
,_dis(dis)
,_sumValue(0)
,_ack({})
,_counter(0)
,_length(0)
{
}




unsigned char dummy_ack[] = 
{
	0xff,(unsigned char)'v',0x5,0x1f,
	0x00^0xff^(unsigned char)'v'^0x5^0x1f
};


/** URATの読み込みスレッド本体
* @return None
*/
void UARTRead::operator()()
{
    while(true)
    {
		if(0 >_fd)
		{
			usleep(100*1000);
		}    	
#if 1 // read from uart
        unsigned char data = 0;
    	auto readSize = read(_fd,&data,sizeof(data));
        if(0 < readSize)
        {
	        put(data);
        }
    	else if(0 == readSize)
    	{
//    		printf("no data\n");
    	}
        else
        {
//            perror("read ");
        }
#endif
//    	this->trySendBuffer();

    	
//// debug dummy
#if 0
    	{
	    	for(const auto data:dummy_ack)
	    	{
	    		put(data);
	    	}
    		chrono::milliseconds dura( 10000 );
    		this_thread::sleep_for( dura );
    	}
#endif
    }
}




void UARTRead::checkSum(unsigned char data)
{
    _sumValue ^= data;
}

void UARTRead::put(unsigned char data)
{
	DUMP_VAR_READ(data);
#ifdef DUMP_UART_IO
       printf("0x%02x,",data);
#endif
    if(_constKST == data)
    {
        reset();
	    _counter++;
	    checkSum(data);
		DUMP_VAR_READ(_length);
		DUMP_VAR_READ(_counter);
		DUMP_VAR_READ(_sumValue);
        _ack.push_back(data);
        return;
    }
	_counter++;
    checkSum(data);
    if(3 == _counter)
    {
        _length = data;
        _ack.push_back(data);
		DUMP_VAR_READ(_length);
		DUMP_VAR_READ(_counter);
		DUMP_VAR_READ(_sumValue);
        return;
    }
    ///
    if(_length == _counter && 0 == _sumValue)
    {
    	struct timeval tv;
		gettimeofday(&tv,NULL);
		long long now = tv.tv_sec *1000 + tv.tv_usec /1000;
		long long uartTimeLag = now - globalSendUartTime;
		DUMP_VAR(uartTimeLag);

    	_ack.push_back(data);
    	DUMP_VAR(&_dis);
    	_dis.ack(_ack);
#ifdef DUMP_UART_IO
       printf("\n");
#endif
    	return;
    }
	DUMP_VAR_READ(_length);
	DUMP_VAR_READ(_counter);
	DUMP_VAR_READ(_sumValue);
    _ack.push_back(data);
}

void UARTRead::reset(void)
{
    _ack.clear();
    _sumValue = 0;
    _length = 0;
    _counter = 0;
}


#if 0 //// no used 
static mutex  gSendBufferMtx;
static list<list<unsigned char> > gSendBuffer;


void UARTRead::send(const list<unsigned char> &data)
{
	lock_guard<mutex> lock(gSendBufferMtx);
	gSendBuffer.push_back(data);
}

void UARTRead::trySendBuffer(void)
{
	lock_guard<mutex> lock(gSendBufferMtx);
	for(const auto &data :gSendBuffer)
	{
		this->write(data);
	}
	gSendBuffer.clear();
}

void UARTRead::write(const list<unsigned char> &data)
{
    DUMP_VAR(_fd);
#ifdef DUMP_UART_IO
	printf("send data data.size()=<%d><\n",data.size());
#endif
	unsigned char buf[data.size()];
	int i = 0;
    for (const auto ch:data)
    {
    	buf[i++] = ch;
#ifdef DUMP_UART_IO
    	printf("0x%02x,",ch);
#endif
    }
#ifdef DUMP_UART_IO
	printf(">\n");
#endif

	struct timeval pred_tv;
	gettimeofday(&pred_tv,NULL);
	auto preMiliSec = pred_tv.tv_sec *1000 + pred_tv.tv_usec /1000;

	auto tcret = tcflush(_fd,TCIOFLUSH);
	if (tcret == -1) 
	{
        perror("tcflush");
	}
	
    const auto writeRet = ::write(_fd,buf,data.size());
	if(writeRet != data.size())
	{
		FATAL_VAR(writeRet);
		FATAL_VAR(data.size());
	}
	auto tcRet = tcdrain(_fd);
	if(0 > tcRet)
	{
		FATAL_VAR(tcRet);
		perror("tcdrain");
	}
	DUMP_VAR(tcRet);

	struct timeval tv;
	gettimeofday(&tv,NULL);
	globalSendUartTime = tv.tv_sec *1000 + tv.tv_usec /1000;
	
	auto uartWriteTime = globalSendUartTime -  preMiliSec;
	
	DUMP_VAR(uartWriteTime);

}

#endif

