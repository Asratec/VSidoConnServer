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
#include <fstream>
#include <streambuf>
using namespace std;

#include "dispatch.hpp"
#include "bt_watchdog.hpp"
#include "rs_request.hpp"
#include "rs_response.hpp"
#include "ws_request.hpp"
#include "ws_response.hpp"
#include "cmd_common.hpp"

#include "VSido.hpp"
#include "VSidoMotion.hpp"
using namespace VSido;
#include "debug.h"



string forceStrRoot;

SystemInfo gSysInfo;


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



#include <signal.h>
static void sig_int_catch(int);
static void sig_abrt_catch(int);
static void sig_pipe_catch(int);
static void readConfig(string &port ,int &baudrate);

extern string execShell(string cmd);

#include "picojson.h"



/** 入口
* @param コマンドの引数なし
* @return -1異常終了、0正常終了
*/
int main(int argc ,char *argv[])
{
	if(SIG_ERR == signal(SIGINT, sig_int_catch))
	{
		perror("failed to set signal handler\n");
		return -1;
	}
	
	if(SIG_ERR == signal(SIGABRT, sig_abrt_catch))
	{
		perror("failed to set signal handler\n");
		return -1;
	}
	if(SIG_ERR == signal(SIGPIPE, sig_pipe_catch))
	{
		perror("failed to set signal handler\n");
		return -1;
	}

	string port;
	int baudrate = B115200;
	/// read serial setting.
	readConfig(port,baudrate);
	FATAL_VAR(port);
	FATAL_VAR(baudrate);
	if(port.empty())
	{
		VSido::connect();
	}
	else
	{
		VSido::connect(port,baudrate);
	}
	
	Dispatcher dispath;
	/// ユーザー操作からVSidoConnectorまで情報伝達経路
	/// VSidoCmd --> VSidoService --> UART --> VSidoConnector
	/// ユーザー操作の操作待ちスレッド
    RSRequest reqRS(dispath);

	/// ユーザー操作からVSidoConnectorまで情報伝達経路
	/// Web --> WebSocket --> VSidoService --> UART --> VSidoConnector
	/// ユーザー操作の操作待ちスレッド
    WSRequest reqWS(dispath);
	

	Motion motion;
	motion.loadXmlAll();
	
	thread threadDisp(std::ref(dispath));
    thread threadReqRS(std::ref(reqRS));
    thread threadReqWS(std::ref(reqWS));
	thread threadMotion(std::ref(motion));
    
    
    threadDisp.join();
    threadReqRS.join();    
    threadReqWS.join();
	threadMotion.join();
 
	VSido::disConnect();
    return 0;
}

void sig_int_catch(int sig)
{
	printf("catch signal %d\n", sig);
	VSido::disConnect();
	exit(-1);
}

void sig_abrt_catch(int sig)
{
	printf("catch signal %d\n", sig);
}
void sig_pipe_catch(int sig)
{
	printf("catch signal %d\n", sig);
}



#define BAURATE(x) {"B_"#x,B##x}

static map<string,int> baudrateTable =
{
	BAURATE(50),
	BAURATE(75),
	BAURATE(110),
	BAURATE(134),
	BAURATE(150),
	BAURATE(200),
	BAURATE(300),
	BAURATE(600),
	BAURATE(1200),
	BAURATE(1800),
	BAURATE(2400),
	BAURATE(4800),
	BAURATE(9600),
	BAURATE(19200),
	BAURATE(38400),
	BAURATE(57600),
	BAURATE(115200),
	BAURATE(230400),
	BAURATE(460800),
	BAURATE(500000),
	BAURATE(576000),
	BAURATE(921600),
	BAURATE(1000000),
	BAURATE(1152000),
	BAURATE(1500000),
	BAURATE(2000000),
	BAURATE(2500000),
	BAURATE(3000000),
	BAURATE(3500000),
	BAURATE(4000000)
};

void readConfig(string &port ,int &baudrate)
{
	string systemInfo("uname -n");
    auto uname = execShell(systemInfo);
	FATAL_VAR(uname);
	
	string configPah;
	if("edison\n"== uname)
	{
		configPah = "/home/sysroot/usr/etc/serial";
	}
	else if("raspberrypi\n"== uname)
    {
    	configPah = "/opt/vsido/usr/etc/serial";
    }
	else
	{
		
	}
	if(false==configPah.empty())
	{
		picojson::value conf;
		ifstream t(configPah);
		string err;
		picojson::parse(conf, istream_iterator<char>(t),istream_iterator<char>(),&err);
		
		if(err.empty())
		{
			auto& confObj = conf.get<picojson::object>();

			if(conf.contains("port"))
			{
				port = confObj["port"].get<string>();
			}
			if(conf.contains("baudrate"))
			{
				auto baudrateS = confObj["baudrate"].get<string>();
				auto it = baudrateTable.find(baudrateS);
				if(it != baudrateTable.end())
				{
					baudrate = it->second;
				}
			}
		}
		else
		{
			FATAL_VAR(err);
		}
	}
}
