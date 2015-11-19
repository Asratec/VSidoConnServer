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
#include "VSidoConnectUartRead.hpp"
#include "VSidoConnect.hpp"
using namespace VSido;

#include <unistd.h>
#include <stdio.h>
#include <termios.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>              // std::mutex
#include <condition_variable>
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
UARTRead::UARTRead()
:_fd(-1)
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
		this_thread::yield();
    	
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
    if(_constKST == data)
    {
       DUMP_SPEED_CHECK("recieve begin msg");
#ifdef DUMP_UART_IO
       printf("read uart dump begin < \n");
       printf("0x%02x,",data);
#endif
        reset();
	    _counter++;
	    checkSum(data);
		DUMP_VAR_READ(_length);
		DUMP_VAR_READ(_counter);
		DUMP_VAR_READ(_sumValue);
        _ack.push_back(data);
        return;
    }
#ifdef DUMP_UART_IO
       printf("0x%02x,",data);
#endif
	_counter++;
    checkSum(data);
	int CounterIndex = 0x3;
	if(Request::useuid_)
	{
		CounterIndex = 0x5;
	}
    if(CounterIndex == _counter)
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
#ifdef DUMP_UART_IO
       printf("\n");
       printf("> read uart dump end\n");
#endif
    	_ack.push_back(data);
    	// a whole command is done.
		DUMP_SPEED_CHECK("recieve whole msg");
    	DUMP_VAR(typeid(fn_).name());
    	fn_(_ack);
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


/** 受信通知を設定する
* @param fn 通知関数
* @return None
*/
void UARTRead::callback(function<void(list<unsigned char>)> fn)
{
	fn_ = fn;
    DUMP_VAR(typeid(fn_).name());
}


