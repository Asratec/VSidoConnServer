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
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <unistd.h>
#include <termios.h>
#include <poll.h>


#include <iostream>
#include <thread>
#include <condition_variable>
using namespace std;

#include "debug.h"


long long globalSendUartTime = 0;

static int const iConstAvarageOnceWrite = 16;
static int const iConstWriteTryMergin = 10;

static int const iConstUartIntervalMilliSecond = 25;
static int const iConstUartReadWaitMilliSecond = 1;



/** コンストラクタ
*/
UARTSend::UARTSend()
:_fd(-1)
,_cmds({})
{
}




mutex gUartWriteMutex;
//condition_variable gUartWriteCV;
/** VSidoと繋がるURATにコマンド送信
* @param data VSido制御コマンド
* @return None
*/
void UARTSend::send(const list<unsigned char> &data)
{

	// give a interval to uart write.
	auto now = chrono::duration_cast<chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	static auto prev = now;
	auto elapsed_tp = now - prev;
	FATAL_VAR(elapsed_tp);
	int remainMiliSec = iConstUartIntervalMilliSecond - elapsed_tp;
	FATAL_VAR(remainMiliSec);
	static bool firstTime = true;
	if(false == firstTime && 0 < remainMiliSec)
	{
		DUMP_SPEED_CHECK("sleep_for start");
		this_thread::sleep_for(chrono::milliseconds(remainMiliSec));
		DUMP_SPEED_CHECK("sleep_for end");
		prev = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();
	}
	else
	{
		prev = now;
	}
    write(data);
	firstTime = false;
}


void UARTSend::write(const list<unsigned char> &data)
{
	DUMP_UART_IO_WRITE(data);
	DUMP_SPEED_CHECK("writes start");

	
	DUMP_VAR(_fd);
	if(0 >_fd)
	{
		return;
	}

	unsigned char buf[data.size()];
	int i = 0;
    for (const auto ch:data)
    {
    	buf[i++] = ch;
    }
    DUMP_SPEED_CHECK_DETAILS();

	struct timeval pred_tv;
	gettimeofday(&pred_tv,NULL);
	auto preMiliSec = pred_tv.tv_sec *1000 + pred_tv.tv_usec /1000;

    DUMP_SPEED_CHECK_DETAILS();
#if 0 // do not flush.
	auto tcret = tcflush(_fd,TCIOFLUSH);
	if (0 > tcret) 
	{
        perror("tcflush");
		tryReconnect();
	}
#endif

#if 0 // test read first.
	int byteInTTY = 1;
	ioctl(_fd,FIONREAD,&byteInTTY);
	while(byteInTTY > 0)
	{
		gUartWriteCV.notify_all();
		this_thread::sleep_for(chrono::milliseconds(iConstUartReadWaitMilliSecond));
		ioctl(_fd,FIONREAD,&byteInTTY);
		if(byteInTTY > 0)
		{
			FATAL_VAR(byteInTTY);
		}
	}
#endif
	
	lock_guard<mutex> lock(gUartWriteMutex);
    DUMP_SPEED_CHECK_DETAILS();
	fd_set writefds;
	FD_ZERO(&writefds);
	FD_SET(_fd,&writefds);
    DUMP_SPEED_CHECK_DETAILS();
	if(select(_fd+1,NULL,&writefds,NULL,NULL))
	{
        DUMP_SPEED_CHECK_DETAILS();
	    auto writeRet = ::write(_fd,buf,data.size());
		/// when write buffer is full,send and wait
		/// retry to write.
		int counter = iConstWriteTryMergin + (data.size()/iConstAvarageOnceWrite);
		while(writeRet < data.size() && 0 < counter-- )
		{
			FATAL_VAR(writeRet);
			FATAL_VAR(data.size());
            DUMP_SPEED_CHECK_DETAILS();
			auto tcRet = tcdrain(_fd);
			if(0 > tcRet)
			{
				FATAL_VAR(tcRet);
				perror("tcdrain");
			}
			FD_ZERO(&writefds);
			FD_SET(_fd,&writefds);
            DUMP_SPEED_CHECK_DETAILS();
			if(select(_fd+1,NULL,&writefds,NULL,NULL))
			{
                DUMP_SPEED_CHECK_DETAILS();
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
			else
			{
				///
			}
		}
		DUMP_VAR(writeRet);
	}
	else 
	{
		///
	}
    DUMP_SPEED_CHECK_DETAILS();
	auto tcRet = tcdrain(_fd);
	if(0 > tcRet)
	{
		FATAL_VAR(tcRet);
		perror("tcdrain");
	}
	DUMP_VAR(tcRet);
    DUMP_SPEED_CHECK_DETAILS();

	
	struct timeval tv;
	gettimeofday(&tv,NULL);
	globalSendUartTime = tv.tv_sec *1000 + tv.tv_usec /1000;
	
	auto uartWriteTime = globalSendUartTime -  preMiliSec;
	
//	gUartWriteCV.notify_all();
	DUMP_VAR(uartWriteTime);
	DUMP_SPEED_CHECK("writes end");

}

void UARTSend::tryReconnect(void)
{
}

