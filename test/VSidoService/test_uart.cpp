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
#include "test_uart.hpp"
#include <iostream>

#include <list>
#include <map>
#include <chrono>
#include <tuple>
using namespace std;


map<int,tuple<list<unsigned char>,chrono::milliseconds>> ut_uart_read;
list<unsigned char> ut_uart_send;



namespace VSido
{




/** 送信する
* @param data 送信データ
* @return None
*/
void sendUart(list<unsigned char> data)
{
	ut_uart_send = data;
}
/** UARTを受信する。 
* @return 受信データ
*/
map<int,tuple<list<unsigned char>,chrono::milliseconds>> readUart()
{
	return ut_uart_read;
}

/** 関心のコマンドを削除する。 
* @param i 番号
* @return None
*/
void deleteUartAck(int i)
{
	
}

	
/** 新規コマンド発行したことを受信スレッドに通知する
* @return None
*/
void notifyNewUart(void)
{
	
}
} // namespace VSido
