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
#include "cmd_common.hpp"
#include "VSido.hpp"
using namespace VSido;

#include <string>
#include <iostream>
using namespace std;



#include "debug.h"


/** コンストラクタ
* @param None
*/
JSONRequest::JSONRequest()
:_res()
{
}

/** コマンドを実行する。
* @param None
* @return 返事のJSON文字列
*/
string JSONRequest::exec()
{
	return "";
}

/** ACK返事のJSON解析を実行する。
* @param ack 返事
* @return 返事のJSON文字列
*/
string JSONRequest::Ack(AckResponse &ack)
{
	if(ack.timeout())
	{
		_res["type"] = picojson::value(string("timeout"));
		picojson::value resValue(_res);
		return resValue.serialize();
	}
	if(ack)
	{
		_res["type"] = picojson::value(string("ack"));
		string raw = static_cast<string>(ack);
		_res["raw"] = picojson::value(raw);
		_res["message"] = picojson::value(string(ack.msg()));
		picojson::value val(_res);
		return val.serialize();
	}
	else
	{
		_res["type"] = picojson::value(string("error"));
		_res["detail"] = picojson::value(string(""));
		picojson::value resValue(_res);
		return resValue.serialize();
	}
}

/** 実際のACK返事のなしの場合を実行する。
* @return 返事のJSON文字列
*/
string JSONRequest::Ack()
{
	_res["type"] = picojson::value(string("accepted"));
	picojson::value val(_res);
	return val.serialize();
}

