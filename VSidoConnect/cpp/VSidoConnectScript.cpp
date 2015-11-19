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
#include "VSidoConnect.hpp"
using namespace VSido;

#include <iostream>
using namespace std;


#define log_erro_param(...)



template <typename T> string tryJsonResponse(Request &req)
{
	string ret;
	try
	{
		T & req = dynamic_cast<T &>(req);
		auto ack = req.execBK();
		return ack.JSON();
	}
	catch(...)
	{
		
	}
	return ret;
}



/**
* 要求コマンドを実行する
* @param req
* @return VSido CONNECTから返事クラス
*/
string execWithJsonResult(Request &req)
{
	string ret;
	
	// try AngleRequest Ack Response.
	{
		auto json = tryJsonResponse<AngleRequest>(req);
		if(false == json.empty())
		{
			return json;
		}
	}

	// try ComplianceRequest Ack Response.
	{
		auto json = tryJsonResponse<ComplianceRequest>(req);
		if(false == json.empty())
		{
			return json;
		}
	}

	// try MinMaxAngleRequest Ack Response.
	{
		auto json = tryJsonResponse<MinMaxAngleRequest>(req);
		if(false == json.empty())
		{
			return json;
		}
	}

	// try ServoInfoRequest Response.
	{
		auto json = tryJsonResponse<ServoInfoRequest>(req);
		if(false == json.empty())
		{
			return json;
		}
	}
	
	return ret;
}




