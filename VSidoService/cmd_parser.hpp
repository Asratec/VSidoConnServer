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
#ifndef __VISIDO_REQUEST_PARSER_HPP__
#define __VISIDO_REQUEST_PARSER_HPP__


#include <list>
#include <string>
#include <memory>
using namespace std;
#include "picojson.h"

namespace VSido
{
class JSONRequest;

/**
* HTTP(Json)の要求解析
*/
class JSONRequestParser
{
public:
	/** コンストラクタ
	* @param[in] raw httpサーバーからのJson要求
	*  {
	*	 "cmd" : コマンド,
	*     コマンドに応じて変わる内容
	*  }
	*/
    JSONRequestParser(const string &json);
	
	/** 要求のインスタンス作成する。
	* @param None
	* @return インスタンス
	*/
	shared_ptr<JSONRequest> create(void);
	
	/** エラーメッセージ送信を取得する。
	* @param None
	* @return エラーメッセージ
	*/
	string errorMsg()
	{
		return err_;
	}

private:
    JSONRequestParser(void);
private:
    picojson::value _raw;
	string err_;
};
} // namespace VSido
#endif //__VISIDO_REQUEST_PARSER_HPP__

