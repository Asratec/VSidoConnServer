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
#ifndef __DISPATCH_HPP__
#define __DISPATCH_HPP__


#include <list>
#include <tuple>
#include <memory>
using namespace std;

namespace VSido
{
class JSONRequest;
class WSResponse;
class RSResponse;

/**
 * 送受信の分配
 */
class Dispatcher
{
public:
	/** コンストラクタ
	* @param send VSidoと繋がるURATの送信
	*/
    Dispatcher();
	
	/** 要求を追加する
	* @param req 要求
	* @param res WebSocket返事先
	*/
	void addRequest(const string req,shared_ptr<WSResponse> res);

	/** 要求を追加する
	* @param req 要求
	* @param res TCPSocket返事先
	*/
	void addRequest(const string req,shared_ptr<RSResponse> res);

	/** 返事
	* @return None
	*/
	static void onResponse(int id ,const string & msg);

	/** 分配スレッド本体
	* @return None
	*/
    void operator()();

	
private:
	
	void trySendWSReq(void);
	void trySendRSReq(void);
	
private:
	list<tuple<string,shared_ptr<WSResponse>>> &_wsReq;
	list<tuple<string,shared_ptr<RSResponse>>> &_rsReq;
};
} // namespace VSido

#endif //__DISPATCH_HPP__
