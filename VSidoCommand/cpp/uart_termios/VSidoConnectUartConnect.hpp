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
#ifndef __UART_CONNECT_HPP__
#define __UART_CONNECT_HPP__


#include <list>
#include <tuple>
#include <memory>
#include <string>
using namespace std;

namespace VSido
{

/**
 * UART接続管理
 */
class UARTConnect
{
public:
	/** コンストラクタ
	* @param dev VSidoコネクトと繋ぐデバイス
	*/
	UARTConnect(const string &dev);

	/** コンストラクタ
	* @param dev VSidoコネクトと繋ぐデバイス
	* @param baudrate ボーレート
	*/
	UARTConnect(const string &dev,int baudrate);
	
	/** SPPを開く
	* @return None
	*/
	void openSPP();

	/** SPPを閉じる
	* @return None
	*/
	void closeSPP();
	
	/** URATを変更する。
	* @return UART
	*/
	operator int() const;

private:
	/** コンストラクタ
	* @return None
	*/
	UARTConnect();

	
private:
	const string _dev;
	int _uart;
	int _baudrate;
};
} // namespace VSido

#endif //__UART_CONNECT_HPP__
