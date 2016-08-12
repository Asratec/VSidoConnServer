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
#include "debug.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <list>
#include <fstream>
#include <streambuf>
using namespace std;



#include <string>
#include <iostream>
#include <sstream>
#include <cstdio>

static string exec(string cmd);
static void initEnv(void);

#include "VSidoConnectUartConnect.hpp"
#include "VSidoConnectUartSend.hpp"
#include "VSidoConnectUartRead.hpp"
#include "VSidoConnect.hpp"
using namespace VSido;

#include <memory>
#include <thread>
using namespace std;

#include "picojson.h"




static unique_ptr<UARTConnect> uartConn_;
static unique_ptr<UARTSend> uartSend_;
static unique_ptr<UARTRead> uartRead_;


VSido::SystemInfo VSido::gSysInfo;
void fillSystemInformation(void);


void IOConnect(const string &device,int baudrate) throw(string)
{
    fillSystemInformation();
	if(device.empty())
	{
		initEnv();
		uartConn_ = unique_ptr<UARTConnect>(new UARTConnect("/dev/tty.vsido.link"));
	}
	else
	{
		uartConn_ = unique_ptr<UARTConnect>(new UARTConnect(device,baudrate));
	}
	uartSend_ = unique_ptr<UARTSend>(new UARTSend);
	uartRead_ = unique_ptr<UARTRead>(new UARTRead);
	uartSend_->setFD(*uartConn_);
	uartRead_->setFD(*uartConn_);

	thread threadRead(std::ref(*uartRead_));
	threadRead.detach();

}


void IODisConnect(void ) throw(string)
{
	if(nullptr != uartConn_)
	{
		uartConn_->closeSPP();
		uartConn_.reset(nullptr);
	}
}


void IOSendUart(list<unsigned char> data)
{
	if(nullptr != uartSend_)
	{
		uartSend_->send(data);
	}
}

void IOSetRecieveCallback(function<void(list<unsigned char>)> fn)
{
	if(nullptr != uartRead_)
	{
		uartRead_->callback(fn);
	}
}



static string exec(string cmd)
{
    FILE* pipe = ::popen(cmd.c_str(), "r");
    if (!pipe)
	{
		return "ERROR";
	}
    char buffer[128];
    std::string result = "";
    while(!feof(pipe))
	{
    	if(fgets(buffer, 128, pipe) != NULL)
		{
    		result += buffer;
		}
    }
    pclose(pipe);
    return result;
}


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
                string link("rm -f /dev/tty.vsido.link;ln -sf ");
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
				string link("rm -f /dev/tty.vsido.link;ln -sf /dev/ttyAMA0 /dev/tty.vsido.link");
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
        string shellBT("/home/sysroot/usr/bin/btsetup");
        system(shellBT.c_str());
		
		{
			string link("rm -f /dev/tty.vsido.link;ln -sf /dev/rfcomm75 /dev/tty.vsido.link");
			auto result = exec(link);
			FATAL_VAR(result);
		}
    }
    else
    {
        
    }
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
    auto uname = exec(systemInfo);
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

void fillSystemInformation(void)
{
    gSysInfo.linux =true;
    string systemInfo("cat /etc/vsido/name");
    auto uname = exec(systemInfo);
    FATAL_VAR(uname);
    
    string configPah;
    if("edison\n"== uname)
    {
        gSysInfo.edison =true;
    	gSysInfo.name = "edison";
    }
    else if("raspberrypi\n"== uname)
    {
        gSysInfo.rasp =true;
    	gSysInfo.name = "rasp";
    }
    else
    {
        
    }
    
}