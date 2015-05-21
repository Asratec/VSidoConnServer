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
#include "vsido_request_scan_bt.hpp"
using namespace VSido;
#include <string>
#include <iostream>
using namespace std;


#include "debug.h"

/**
* Bluetooth Scan要求
*/
RequestScanBT::RequestScanBT(picojson::object &raw)
:Request()
,_raw(raw)
{
}


/** Json要求からVSidoパケットへ変換する
* @param None
* @return VSidoパケット
*/
list<unsigned char> RequestScanBT::conv(void)
{
	list<unsigned char> ret = {};
	return ret;
}


#include <string>
#include <iostream>
#include <cstdio>

string exec(string cmd)
{
    FILE* pipe = ::popen(cmd.c_str(), "r");
    if (!pipe)
	{
		return "ERROR";
	}
    char buffer[128];
    std::string result = "";
    while(!feof(pipe))
	{
    	if(fgets(buffer, 128, pipe) != NULL)
		{
    		result += buffer;
		}
    }
    pclose(pipe);
    return result;
}



#include <string>
#include <sstream>
#include <algorithm>
#include <vector>


static vector<string> split(const string &str, char delim)
{
	istringstream iss(str); 
	string tmp; vector<string> res;
	while(getline(iss, tmp, delim))
	{ 
		res.push_back(tmp);
	}
	return res;
}

/** Json要求で、実行する。
* @param None
* @return VSidoパケット
*/
string RequestScanBT::runTask(void)
{
	string systemInfo("uname -n");
    auto uname = exec(systemInfo);
	FATAL_VAR(uname);
	string result;
	if("edison\n"== uname)
	{
		result = exec("/home/sysroot/usr/bin/btscan");
	}
    else
    {
        result = exec("/opt/vsido/usr/bin/btscan");
    }
	DUMP_VAR(result);
	
	auto devices = split(result,'\n');
	picojson::array deviceArray;
	for(auto &dev :devices)
	{
		DUMP_VAR(dev);
		auto parts = split(dev,'\t');
		for(auto &part :parts)
		{
			DUMP_VAR(part);
		}
		if(2 < parts.size())
		{
			picojson::object deviceObj;
			deviceObj["mac"] = picojson::value(parts[1]);
			deviceObj["name"] = picojson::value(parts[2]);
			deviceArray.push_back(picojson::value(deviceObj));
		}
	}
	picojson::object jsonObj;

	jsonObj["devices"] = picojson::value(deviceArray);
	jsonObj["type"] = picojson::value("scanBT");

	picojson::value json(jsonObj);
	DUMP_VAR(json.serialize());
	return json.serialize();
}



