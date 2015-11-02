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
#include "rs_response.hpp"
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

#include "debug.h"

static mutex _mtx;
static condition_variable _cv;



/** コンストラクタ
*/
RSResponse::RSResponse(void)
:_fd(-1)
{
    DUMP_VAR(_fd);
}


/** 返事先を設定する
* @param fd 
* @return None
*/
void RSResponse::setDist(int fd)
{
	this->_fd = fd;
}

/** Busyを返事する
* @return None
*/
void RSResponse::ackBusy(void)
{
	string busy("{\"type\":\"busy\"}\n");
// reply error when socket is busy to receive.
//	this->ack(busy);
}
/** Timeoutを返事する
* @return None
*/
void RSResponse::ackTimeout(void)
{
	string timeout("{\"type\":\"timeout\"}\n");
	this->ack(timeout);
}

/** 返事する
* @return None
*/
void RSResponse::ack(const string &msg)
{
	DUMP_VAR(this->_fd);
	if(0 < this->_fd)
	{
		::write(this->_fd, (unsigned char*)msg.c_str(), msg.size());
	}
}

