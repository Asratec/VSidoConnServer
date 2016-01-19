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
#include "cmd_raw.hpp"
#include "VSido.hpp"
using namespace VSido;
#include <string>
#include <iostream>
using namespace std;


#include "debug.h"

/**
* Raw UART データ
*/
RawJSONRequest::RawJSONRequest(picojson::object &raw)
:JSONRequest()
,_raw(raw)
,block_(false)
{
}


/** コマンドを実行する。
* @param None
* @return 返事のJSON文字列
*/
string RawJSONRequest::exec()
{
	RawRequest req;
	try
	{
	//	DUMP_VAR(_raw);
		
		picojson::value rawV(_raw);
		if(rawV.contains("exec"))
		{
			auto execBytes = _raw["exec"].get<picojson::array>();
			for (auto it = execBytes.begin(); it != execBytes.end(); it++)
			{
				unsigned char byte = (unsigned char )it->get<double>();
				DUMP_VAR((int)byte);
				req.bin(byte);
			}
			block_ = true;
		}
		if(rawV.contains("op"))
		{
			unsigned char op = (unsigned char)(_raw["op"].get<double>());
			req.op(op);
			req.autoHead();
		}
		if(rawV.contains("bin"))
		{
			auto binBytes = _raw["bin"].get<picojson::array>();
			for (auto it = binBytes.begin(); it != binBytes.end(); it++)
			{
				unsigned char byte = (unsigned char )it->get<double>();
				req.bin(byte);
			}
		}
	}
	catch(const std::runtime_error &e)
	{
		FATAL_VAR(e.what());
	}
	if(req)
	{
		if(false ==block_)
		{
			auto fn = [this](RawResponse& resp){
				if(resp.timeout())
				{
					_res["type"] = picojson::value(string("timeout"));
					picojson::value resValue(_res);
					Dispatcher::onResponse(uid_,resValue.serialize());
					return ;
				}
				if(resp)
				{
					_res["type"] = picojson::value(string("Binary"));
					string raw = static_cast<string>(resp);
					_res["raw"] = picojson::value(raw);
					picojson::value resValue(_res);
					Dispatcher::onResponse(uid_,resValue.serialize());
					return ;
				}
				else
				{
					_res["type"] = picojson::value(string("error"));
					_res["detail"] = picojson::value(string(""));
					picojson::value resValue(_res);
					Dispatcher::onResponse(uid_,resValue.serialize());
					return ;
				}
			};
			uid_ = req.exec(fn);
			return "";
		}
		else
		{
			auto resp = req.execBK();
			if(resp.timeout())
			{
				_res["type"] = picojson::value(string("timeout"));
			}
			if(resp)
			{
				_res["type"] = picojson::value(string("Binary"));
				string raw = static_cast<string>(resp);
				_res["raw"] = picojson::value(raw);
			}
			else
			{
				_res["type"] = picojson::value(string("error"));
				_res["detail"] = picojson::value(string(""));
			}
			picojson::value resValue(_res);
			return resValue.serialize();
		}
	}
	else
	{
		return req.msg();
	}
}

