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

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <iostream>
using namespace std;

#include "bt_watchdog.hpp"
using namespace VSido;

#include "debug.h"


#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
using namespace std;


static mutex bindMtxBegin;
static condition_variable bindCvBegin;
static mutex bindMtxEnd;
static condition_variable bindCvEnd;




void dumpUartFlags(termios term);



/** Bindを通知する
* @return None
*/
void BTWatchDog::notifyBTBindBegin(void)
{
	bindCvBegin.notify_all();
}


/** Bindを通知する
* @return None
*/
void BTWatchDog::notifyBTBindEnd(void)
{
	bindCvEnd.notify_all();
}


/** コンストラクタ
*/
BTWatchDog::BTWatchDog()
{
#if 0
	_conn.openSPP();
    DUMP_VAR(_conn);
	if(0 < _conn)
	{
		_send.setFD(_conn);
		_read.setFD(_conn);
	}
#endif
}


/** 分配スレッド本体
* @return None
*/
void BTWatchDog::operator()()
{
	while(true)
	{
		unique_lock<mutex> lckBegin(bindMtxBegin);
		bindCvBegin.wait(lckBegin);
		
		/// close uart.
#if 0
		_conn.closeSPP();
#endif		
		
		unique_lock<mutex> lckEnd(bindMtxEnd);
		bindCvEnd.wait(lckEnd);
		
		/// open uart.
		usleep(1000*1000*5);
#if 0
		_conn.openSPP();
	    DUMP_VAR(_conn);
		if(0 < _conn)
		{
			_send.setFD(_conn);
			_read.setFD(_conn);
		}
#endif
	}
}





