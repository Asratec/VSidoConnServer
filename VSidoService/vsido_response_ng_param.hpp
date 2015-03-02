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
#ifndef __VISIDO_RESPONSE_NG_PARAM_HPP__
#define __VISIDO_RESPONSE_NG_PARAM_HPP__


#include <list>
#include <string>
using namespace std;
#include "vsido_response_common.hpp"

namespace VSido
{
/**
* 問題発生時の返事
*/
class ResponseNGParam : public Response
{
public:
	/** コンストラクタ
	* @param[in] uart VSidoからの返事 問題発生時
	*/
    ResponseNGParam(const list<unsigned char> &uart);

	/** VSidoの返事をJsonに変換する
	* @param None
	* @return Jsonデータ
	* {
	* 	"type": "error",
	* 	"message": ""
	* }	
	*/
	virtual string conv(void);
private:
    ResponseNGParam(void);
private:
};
} // namespace VSido

#endif //__VISIDO_RESPONSE_NG_PARAM_HPP__
