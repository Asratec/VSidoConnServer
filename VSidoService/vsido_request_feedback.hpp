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
#ifndef __VISIDO_REQUEST_FEEDBACK_HPP__
#define __VISIDO_REQUEST_FEEDBACK_HPP__


#include <list>
#include <string>
using namespace std;
#include "vsido_request_common.hpp"
#include "picojson.h"

namespace VSido
{

class ResponseFeedback;
/**
* フィードバック要求
*/
class RequestFeedback : public Request
{
public:
	/** コンストラクタ
	* @param[in] raw httpサーバーからのJson要求
	*  {
	*	  "cmd" : "feedback",
	*	  "address" : 1~128, //サーボ情報の先頭アドレス
	*	  "length"  : 1~54,  //サーボ情報の読出し長さ
	*  }
	*/
    RequestFeedback(picojson::object &raw);

	/** Json要求からVSidoパケットへ変換する
	* @param None
	* @return VSidoパケット
	*/
    virtual list<unsigned char> conv(void);
private:
    RequestFeedback(void);
private:
    picojson::object &_raw;
	shared_ptr<ResponseFeedback> _feed;
};
} // namespace VSido

#endif //__VISIDO_REQUEST_FEEDBACK_HPP__
