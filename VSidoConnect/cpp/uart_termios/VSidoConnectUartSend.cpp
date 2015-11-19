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
#include "VSidoConnectUartSend.hpp"
using namespace VSido;
#include <unistd.h>
#include <termios.h>

#include <iostream>
#include <thread>
#include <condition_variable>
using namespace std;

#include "debug.h"

#include <sys/time.h>

long long globalSendUartTime = 0;

static int const iConstAvarageOnceWrite = 16;
static int const iConstWriteTryMergin = 10;


/** コンストラクタ
*/
UARTSend::UARTSend()
:_fd(-1)
,_cmds({})
{
}




static mutex gUartWriteMutex;
/** VSidoと繋がるURATにコマンド送信
* @param data VSido制御コマンド
* @return None
*/
void UARTSend::send(const list<unsigned char> &data)
{
	//UARTRead::send(data);
	lock_guard<mutex> lock(gUartWriteMutex);
    write(data);
}


void UARTSend::write(const list<unsigned char> &data)
{
    DUMP_VAR(_fd);
	if(0 >_fd)
	{
		return;
	}
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

	DUMP_SPEED_CHECK("writes start");
	auto tcret = tcflush(_fd,TCIOFLUSH);
	if (0 > tcret) 
	{
        perror("tcflush");
		tryReconnect();
	}
	
    auto writeRet = ::write(_fd,buf,data.size());
	/// when write buffer is full,send and wait
	/// retry to write.
	int counter = iConstWriteTryMergin + (data.size()/iConstAvarageOnceWrite);
	while(writeRet < data.size() && 0 < counter-- )
	{
		FATAL_VAR(writeRet);
		FATAL_VAR(data.size());
		auto tcRet = tcdrain(_fd);
		if(0 > tcRet)
		{
			FATAL_VAR(tcRet);
			perror("tcdrain");
		}
		auto rWRet = ::write(_fd,buf+writeRet,data.size() - writeRet);
		if(0 < rWRet)
		{
			writeRet += rWRet;
		}
		else
		{
        	perror("write");
			FATAL_VAR(rWRet);
			break;
		}
		if(writeRet == data.size())
		{
			FATAL_VAR(writeRet);
			FATAL_VAR(data.size());
			FATAL_VAR(counter);
		}
	}
	DUMP_VAR(writeRet);
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
	DUMP_SPEED_CHECK("writes end");

}

void UARTSend::tryReconnect(void)
{
}

