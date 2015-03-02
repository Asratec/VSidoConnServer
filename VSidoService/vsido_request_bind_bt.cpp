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
#include "vsido_request_bind_bt.hpp"
#include "bt_watchdog.hpp"
using namespace VSido;
#include <string>
#include <iostream>
using namespace std;


#include "debug.h"

/**
* Bluetooth Bind要求
*/
RequestBindBT::RequestBindBT(picojson::object &raw)
:Request()
,_raw(raw)
{
}


/** Json要求からVSidoパケットへ変換する
* @param None
* @return VSidoパケット
*/
list<unsigned char> RequestBindBT::conv(void)
{
	list<unsigned char> ret = {};
	return ret;
}



extern string exec(string cmd);


/** Json要求で、実行する。
* @param None
* @return VSidoパケット
*/
string RequestBindBT::runTask(void)
{
	BTWatchDog::notifyBTBindBegin();

	string cmd("/home/sysroot/usr/bin/btsetup ");
	try
	{
	//	DUMP_VAR(_raw);
		auto device = _raw["device"].get<picojson::object>();
		auto mac = device["mac"].get<string>();
		DUMP_VAR(mac);
		cmd += mac;
	}
	catch(const std::runtime_error &e)
	{
		DUMP_VAR(e.what());
	}

	DUMP_VAR(cmd);
	auto result = exec(cmd);
	DUMP_VAR(result);
	
	BTWatchDog::notifyBTBindEnd();
	
	picojson::object jsonObj;
	jsonObj["type"] = picojson::value("bindBT");
	picojson::value json(jsonObj);
	DUMP_VAR(json.serialize());
	return json.serialize();
}

