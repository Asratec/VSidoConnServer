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
#include "VSido.hpp"
using namespace VSido;
#include <iostream>
#include <atomic>
using namespace std;

int main()
{
	VSido::connect();
	
	
	{
		AngleRequest req;
		req.cycle(10);
		req.angle(2,100);
		// Not care ack
		req.execNA();
	}

	{
		MinMaxAngleRequest req;
		req.minMax(2,20,35);
		// block exec until Ack is return.
		auto ack = req.execBK();
		cout << ack.msg() << endl;
	}
	
	{
		ComplianceRequest req;
		req.comp(2,20,10);
		// Not care response
		req.exec();
	}
	
	{
		ServoInfoRequest req;
		req.info(2,9,54);
		auto ack = req.execBK();
		ack.forEachServo([](unsigned char sid,vector<tuple<string,int>> &info){
			
		});
	}

	// get setting with no block (multithread).
	{
		ServoInfoRequest req;
		req.info(2,9,54);
		atomic<bool> waitMsg(true);
		auto fn = [&waitMsg](ServoInfoResponse &resp) {
			resp.forEachServo([](unsigned char sid,vector<tuple<string,int>> &info){
				
			});
			waitMsg = false;
		};
		req.exec(fn);
		// keep object(req) util call back;
		while(waitMsg){}
	}
	
	
	VSido::disConnect();
	return 0;
}

