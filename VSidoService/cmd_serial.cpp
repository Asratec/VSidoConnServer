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
#include "cmd_serial.hpp"
#include "VSido.hpp"
using namespace VSido;
#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
using namespace std;


#include "debug.h"

/**
* シリアルポートList 要求
*/
ListSerialJSONRequest::ListSerialJSONRequest(picojson::object &raw)
:JSONRequest()
,_raw(raw)
{
}





extern string execShell(string cmd);


/** コマンドを実行する。
* @param None
* @return 返事のJSON文字列
*/
string ListSerialJSONRequest::exec()
{

	picojson::object jsonObj;
	jsonObj["type"] = picojson::value("ListSerialPort");

	string systemInfo("cat /etc/vsido/name");
    auto uname = execShell(systemInfo);
	FATAL_VAR(uname);
	
	picojson::array portArray;
	
	if("edison\n"== uname)
	{
		portArray.push_back(picojson::value("/dev/ttyMFD0"));
		portArray.push_back(picojson::value("/dev/ttyMFD1"));
		portArray.push_back(picojson::value("/dev/ttyMFD2"));
	}
	else if("raspberrypi\n"== uname)
    {
		portArray.push_back(picojson::value("/dev/ttyAMA0"));
    }
	else
	{
		
	}
	
	jsonObj["port"] = picojson::value(portArray);
	picojson::value json(jsonObj);
	
	DUMP_VAR(json.serialize());
	return json.serialize();
}



/**
* シリアルポート選択 要求
*/
SelectSerialJSONRequest::SelectSerialJSONRequest(picojson::object &raw)
:JSONRequest()
,_raw(raw)
{
}







/** コマンドを実行する。
* @param None
* @return 返事のJSON文字列
*/
string SelectSerialJSONRequest::exec()
{
	try
	{
	//	DUMP_VAR(_raw);
		auto port = _raw["port"].get<string>();
		
		
		picojson::object jsonConfig;
		jsonConfig["port"] = picojson::value(port);
		DUMP_VAR(port);
		
		auto baudrate = _raw["baudrate"].get<string>();
		DUMP_VAR(baudrate);
		jsonConfig["baudrate"] = picojson::value(baudrate);


		picojson::value json(jsonConfig);
		DUMP_VAR(json.serialize());

		string path;
		if(gSysInfo.edison)
		{
	        path = "/home/sysroot/usr/etc/serial";
		}
		else if(gSysInfo.rasp)
		{
	        path = "/opt/vsido/usr/etc/serial";
		}
	    else if(gSysInfo.osx)
	    {
            path = "../etc/serial";
        }
        else
        {
            // use default.
        }
        FATAL_VAR(path);
		if(false==path.empty())
		{
			ofstream t(path);
			t << json.serialize();
		}
	}
	catch(const std::runtime_error &e)
	{
		DUMP_VAR(e.what());
	}
	::execShell("sync");
	
	picojson::object jsonObj;
	jsonObj["type"] = picojson::value("SelectSerialPort");
	picojson::value json(jsonObj);
	DUMP_VAR(json.serialize());
	return json.serialize();
}

