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
#include "dispatch_perf.hpp"
#include "VSido.hpp"
using namespace VSido;
#include <string>
#include <iostream>
#include <random>
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <thread>

using namespace std;


#include "debug.h"

/** コンストラクタ
*/
PerformanceChecker::PerformanceChecker()
{
	
}

static const int iConstPerfTestWaitTimeout = 25;
static mutex reqMtx;
static condition_variable reqCv;

extern bool gBoolPerfChecker;

void PerformanceChecker::operator()()
{
	int forward = 0;
	std::random_device rnd;
	int turn = rnd();
	while(gBoolPerfChecker)
	{
		WalkRequest req;
		req.forward( (forward++)%200 -100);
		req.turn( (turn++)%200 -100);
//		req.forward( 0);
//		req.turn( 0);
		if(req)
		{
			req.execNA();
		}
		else
		{
		}
#if 0
		unique_lock<mutex> lck(reqMtx);
		auto waitRet = reqCv.wait_for(lck,std::chrono::milliseconds(iConstPerfTestWaitTimeout));
		if(cv_status::timeout==waitRet)
		{
			//printf("%s,%d timeout\n",__FILE__,__LINE__);
		}
#endif
	}
}

