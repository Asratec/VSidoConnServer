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
#include "vsido_request_walk.hpp"
using namespace VSido;
#include <string>
#include <iostream>
using namespace std;


#include "debug.h"

/** コンストラクタ
* @param[in] raw httpサーバーからのJson要求
*  {
* 	 "command" : "walk",
* 	 "walk": {
* 	   "address":0~128,
* 	   "length":1~3,
* 	   "data":[
* 	     0~200,0~200,0~200
* 	   ]
* 	 }
*  }
*/
RequestWalk::RequestWalk(picojson::object &raw)
:Request()
,_raw(raw)
{
	_cmd = (unsigned char)'t';
}


/** Json要求からVSidoパケットへ変換する
* @param None
* @return VSidoパケット
*/
list<unsigned char> RequestWalk::conv(void)
{
	try
	{
	//	DUMP_VAR(_raw);
/*		
		auto  add = _raw["address"].get<double>();
		DUMP_VAR(add);
		_uart.push_back((unsigned char)add);
		_length++;
*/
		_uart.push_back((unsigned char)0x0);
		_length++;

/*		
		auto  length = _raw["length"].get<double>();
		DUMP_VAR(length);
		_uart.push_back((unsigned char)length);
		_length++;
*/
		_uart.push_back((unsigned char)0x2);
		_length++;
		
		int  forward = (int)_raw["forward"].get<double>() + 100;
		DUMP_VAR(forward);
		_uart.push_back((unsigned char)forward);
		_length++;

		int  turn = (int)_raw["turn"].get<double>() + 100;
		DUMP_VAR(turn);
		_uart.push_back((unsigned char)turn);
		_length++;
		
	}
	catch(const std::runtime_error &e)
	{
		DUMP_VAR(e.what());
	}
	putCommon();
	return _uart;
}

