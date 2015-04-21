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
#include "uart_send.hpp"
#include "uart_read.hpp"
#include "ws_response.hpp"
#include "vsido_response_parser.hpp"
#include "vsido_response_common.hpp"
using namespace VSido;

#include <string>
#include <iostream>
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
using namespace std;
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <libwebsockets.h>


#include "debug.h"

static mutex _mtx;
static condition_variable _cv;


/// 100 mili sec
static const int iConstBusyWait = 50 *1000;


extern mutex mtxWSConnection;
extern list<struct libwebsocket *> wsCallbacks;

/** コンストラクタ
*/
WSResponse::WSResponse(void)
:_wsi(nullptr)
{
    DUMP_VAR(_wsi);
}


/** 返事先を設定する
* @param fd 
* @return None
*/
void WSResponse::setDist(struct libwebsocket *wsi)
{
	this->_wsi = wsi;
}


/** Busyを返事する
* @return None
*/
void WSResponse::ackBusy(void)
{
	string busy("{\"type\":\"busy\"}\n");
// reply error when socket is busy to receive.
//	usleep(iConstBusyWait);
//	this->ack(busy);
}
/** Timeoutを返事する
* @return None
*/
void WSResponse::ackTimeout(void)
{
	string timeout("{\"type\":\"timeout\"}\n");
	this->ack(timeout);
}

/** 返事する
* @return None
*/
void WSResponse::ack(const string &msg)
{
	DUMP_VAR(this->_wsi);
	if(nullptr != this->_wsi)
	{
		{
			lock_guard<mutex> lock(mtxWSConnection);
			const auto ret =std::find(wsCallbacks.begin(),wsCallbacks.end(),this->_wsi);
			if(wsCallbacks.end() == ret)
			{
				return;
			}
		}
        unsigned char *data = new unsigned char [LWS_SEND_BUFFER_PRE_PADDING + msg.size() + LWS_SEND_BUFFER_POST_PADDING];
        memcpy(&data[LWS_SEND_BUFFER_PRE_PADDING],msg.c_str(),msg.size());
		auto ret = libwebsocket_write(this->_wsi, &data[LWS_SEND_BUFFER_PRE_PADDING], msg.size(), LWS_WRITE_TEXT);
        if(ret < msg.size())
		{
			FATAL_VAR(this->_wsi);
		}
        delete []data;
	}
}





