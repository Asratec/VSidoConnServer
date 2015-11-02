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
#include "VSidoConnect.hpp"
#include "vsido_servo_info_table.hpp"
#include "vsido_vid_table.hpp"
#include "VSidoConnectInner.hpp"
using namespace VSido;
#include "debug.h"


#include <iostream>
using namespace std;


#include <mutex>
using namespace std;
mutex _globalLockMtx;


#define log_erro_param(...)


extern void IOConnect(const string &device,int baudrate) throw(string);
extern void IODisConnect(void ) throw(string);
extern void IOSendUart(list<unsigned char> data);
extern map<int,tuple<list<unsigned char>,chrono::milliseconds>> IOReadUart();
extern void IODeleteUartAck(int i);
extern void IONotifyNewUart(void);

/** V-Sido CONNECTと接続する
* @param device V-Sido CONNECTと接続しているデバイス名。
*                例えば、"/dev/ttyAMA0","/dev/tty.USB0"。
*                何も入れない場合、事前に用意したデバイスリストから探し出す。
* @return None
*/
void VSido::connect(const string &device,int baudrate) throw(string)
{
	IOConnect(device,baudrate);
}

/** V-Sido CONNECTと切断する
* @return None
*/
void VSido::disConnect(void ) throw(string)
{
	IODisConnect();
}




namespace VSido
{
/** 送信する
* @param data 送信データ
* @return None
*/
void sendUart(list<unsigned char> data)
{
#ifdef DUMP_CMD_BINARY
	cout << "data = <" << endl;
	for(auto ch:data)
	{
		cout << "0x" << std::hex << (int)ch << "," << std::dec;
	}
	cout << ">" << endl;
#endif
	IOSendUart(data);
}

/** UARTを受信する。 
* @return 受信データ
*/
map<int,tuple<list<unsigned char>,chrono::milliseconds>> readUart()
{
	auto ret = IOReadUart();
#ifdef DUMP_CMD_BINARY
	cout <<__FILE__ << "@" << __func__ << ":ret=<";
	for(auto ch:ret)
	{
		cout << "0x" << std::hex << (int)ch << "," << std::dec;
	}
	cout << ">" << endl;
#endif
	return ret;
}

/** CONNECTからの返事候補をを削除する
* @param i 番号
* @return None
*/
void deleteUartAck(int i)
{
	IODeleteUartAck(i);
}



	
/** 新規コマンド発行したことを受信スレッドに通知する
* @return None
*/
void notifyNewUart()
{
	IONotifyNewUart();
}


} // namespace VSido





