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
#ifndef __UART_READ__
#define __UART_READ__


#include <list>
#include <map>
#include <tuple>
#include <functional>
using namespace std;

namespace VSido
{

/**
 * VSidoと繋がるURATの読み込み
 */
class UARTRead
{
public:
	/** コンストラクタ
	* @param dis 返事の分配器
	*/
    UARTRead();

	/** ファイルデスクリプタを設定する
	* @param fd VSidoと繋がるURATファイルデスクリプタ
	* @return None
	*/
	void setFD(int fd){_fd = fd;}
	
	/** 受信通知を設定する
	* @param fn 通知関数
	* @return None
	*/
	void callback(function<void(list<unsigned char>)> fn);

	/** URATの読み込みスレッド本体
	* @return None
	*/
    void operator()();
    
private:
    void put(unsigned char data);
    void checkSum(unsigned char data);
    void reset(void);
private:
    const unsigned char _constKST = 0xff;
    int _fd;
    list<unsigned char> _ack;
	function<void(list<unsigned char>)> fn_;
	int _counter;
    int _length;
    unsigned char _sumValue;
};
} // namespace VSido

#endif //__UART_READ__
