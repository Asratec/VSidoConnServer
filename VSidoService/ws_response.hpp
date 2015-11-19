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
#ifndef __WS_RESPONSE_HPP__
#define __WS_RESPONSE_HPP__


#include <list>
#include <string>
#include <memory>
using namespace std;

struct libwebsocket;

namespace VSido
{
class Response;

/**
 * WebSocketの返信
 */
class WSResponse
{
public:
	/** コンストラクタ
	*/
    WSResponse(void);

	/** 返事先を設定する
	* @param wsi 
	* @return None
	*/
    void setDist(struct libwebsocket *wsi);
	
	/** Busyを返事する
	* @return None
	*/
    void ackBusy(void);

	/** Timeoutを返事する
	* @return None
	*/
    void ackTimeout(void);

	/** Miss エラーを返事する
	* @return None
	*/
    void ackMiss(void);

	/** 返事する
	* @return None
	*/
    void ack(const string &msg);

	/** 予期の返事を取得する
	* @param None
	* @return 返事ポインタ
	*/
	shared_ptr<Response> getExpect(void)
	{
		return _expect;
	}

	/** 予期の返事を設定する
	* @param expect 予期の返事
	* @return 返事ポインタ
	*/
	void setExpect(shared_ptr<Response> expect)
	{
		_expect = expect;
	}

private:
private:
    struct libwebsocket *_wsi;
	shared_ptr<Response> _expect;
};
} // namespace VSido

#endif //__WS_RESPONSE_HPP__
