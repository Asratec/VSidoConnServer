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
#ifndef __VISIDO_REQUEST_COMMON_HPP__
#define __VISIDO_REQUEST_COMMON_HPP__


#include <string>
using namespace std;

#include "picojson.h"

namespace VSido
{

class AckResponse;
/**
* ウェブ要求
*/

class JSONRequest
{
public:
	/** コマンドを実行する。
	* @param None
	* @return 返事のJSON文字列
	*/
    virtual string exec();

	/** デストラクタ
	* @param None
	*/
	virtual ~JSONRequest(){};
protected:
	/** コンストラクタ
	* @param None
	*/
    JSONRequest();

	/** ACK返事のJSON解析を実行する。
	* @param ack 返事
	* @return 返事のJSON文字列
	*/
    string Ack(AckResponse &ack);
protected:
    picojson::object _res;
};
} // namespace VSido

#endif //__VISIDO_REQUEST_COMMON_HPP__
