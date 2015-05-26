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
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <iostream>
#include <sstream>
using namespace std;

#include "uart_send.hpp"
#include "uart_read.hpp"
#include "dispatch.hpp"
#include "bt_watchdog.hpp"


#include "rs_request.hpp"
#include "rs_response.hpp"
#include "ws_request.hpp"
#include "ws_response.hpp"

using namespace VSido;
#include "debug.h"



 mutex _globalLockMtx;


/*
 mac usb serial command

 screen /dev/tty.usbserial-AJ035QBI 115200
 */

/*
 git clone 
 git clone ssh://maeikei@192.168.11.4/Users/maeikei
 /workspace/workspace/git.repso/Edison2VSido


 git clone ssh://ma@192.168.11.6:22222/media/sf_vms-shared/git.repso/VSidoWithEdison
 git clone file:///media/sf_vms-shared/git.repso/VSidoWithEdison

 */


extern string exec(string cmd);


/** 実行時必要なファイル領域を作る
* @param None
* @return None
*/
static void initEnv(void)
{
    string systemType("uname");
    auto uType = exec(systemType);
    FATAL_VAR(uType);
    if("Darwin\n"== uType)
    {
        /// using bluetooth
        {
            string shell("find  /dev/tty* | grep tty.SBDBT");
            auto result = exec(shell);
            FATAL_VAR(result);
            stringstream ss(result);
            std::string tty;
            getline(ss, tty);
            FATAL_VAR(tty);
            if(tty.size() > 5)
            {
                string link("ln -sf ");
                link += tty;
                link += " /dev/tty.vsido.link";
                auto result = exec(link);
                FATAL_VAR(result);
            }
        }
    }
    else if("Linux\n"== uType)
    {
        /// add a action log folder
        {
            string shell("umount /mnt/vsido/log");
            int ret = ::system(shell.c_str());
        }
        {
            string shell("mkdir -p /mnt/vsido/log");
            int ret = ::system(shell.c_str());
        }
        {
            string shell("mount -t tmpfs -o size=2M tmpfs /mnt/vsido/log");
            int ret = ::system(shell.c_str());
        }
        {
            string shell("sync");
            int ret = ::system(shell.c_str());
        }
    }

	
	
	string systemInfo("uname -n");
    auto uname = exec(systemInfo);
	FATAL_VAR(uname);
	if("raspberrypi\n"== uname)
	{
		/// using AMA0
		{
			string shell("find  /dev/tty* | grep ttyAMA0");
	        auto result = exec(shell);
	    	FATAL_VAR(result);
			if(result.size() > 5)
			{
				string link("ln -sf /dev/ttyAMA0 /dev/tty.vsido.link");
				auto result = exec(link);
				FATAL_VAR(result);
			}
		}
		/// check BT ,if has a HCI Mode dongle will using BLUE Tooth 
		{
			/// wait 5 secend for usb dongle.
			{
				string shell("sleep 5;lsusb;hciconfig;");
		        auto result = exec(shell);
		    	FATAL_VAR(result);
			}
			string shell("hciconfig | grep \"BD Address:\"");
	        auto result = exec(shell);
	    	FATAL_VAR(result);
			if(result.size() > 5)
			{
		        string shellBT("/opt/vsido/usr/bin/btsetup");
				system(shellBT.c_str());
				/// blocked at read pipe us system. do not see result.
//		        auto result = exec(shellBT);
//		    	FATAL_VAR(result);
				{
					string link("ln -sf /dev/rfcomm75 /dev/tty.vsido.link");
					auto result = exec(link);
					FATAL_VAR(result);
				}
			}
		}
	}
	else if("edison\n"== uname)
	{
        string shell("/home/sysroot/usr/bin/btsetup");
        auto result = exec(shell);
    	FATAL_VAR(result);
		{
			string link("ln -sf /dev/rfcomm75 /dev/tty.vsido.link");
			auto result = exec(link);
			FATAL_VAR(result);
		}
    }
    else
    {
        
    }
}



/** 入口
* @param コマンドの引数なし
* @return -1異常終了、0正常終了
*/
int main(int argc ,char *argv[])
{
	/// 実行環境の準備
	if(2 < argc )
	{
		string link("ln -sf ");
		link += string(argv[1]);
		link += "/dev/tty.vsido.link";
		auto result = exec(link);
		FATAL_VAR(result);
	}
	else
	{
    	initEnv();
	}
    UARTSend send;
	Dispatcher dispath(send);
	/// ユーザー操作からVSidoConnectorまで情報伝達経路
	/// VSidoCmd --> VSidoService --> UART --> VSidoConnector
	/// ユーザー操作の操作待ちスレッド
    RSRequest request(dispath);

	/// ユーザー操作からVSidoConnectorまで情報伝達経路
	/// Web --> WebSocket --> VSidoService --> UART --> VSidoConnector
	/// ユーザー操作の操作待ちスレッド
    WSRequest reqWS(dispath);
	
	/// VSidoConnectorからユーザーフィードバックまで情報伝達経路
	/// Web <-- Apache2 <-- VSidoCmd <-- VSidoService <-- UART <-- VSidoConnector
	/// VSidoコネクターのACK待ちスレッド
    UARTRead read(dispath);
    
    
	BTWatchDog dog(send,read);
	
	thread threadDisp(std::ref(dispath));
    thread threadReq(std::ref(request));
    thread threadReqWS(std::ref(reqWS));
    thread threadRead(std::ref(read));
	thread threadDog(std::ref(dog));
    
    
    threadDisp.join();
    threadReq.join();    
    threadReqWS.join();
    threadRead.join();
    threadDog.join();
 
    return 0;
}
