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
#ifndef __VISIDO_REQUEST_VAR_GET_HPP__
#define __VISIDO_REQUEST_VAR_GET_HPP__


#include <list>
#include <string>
using namespace std;
#include "cmd_common.hpp"
#include "picojson.h"

namespace VSido
{
/**
* 各種変数設定
*/
class VarSetJSONRequest : public JSONRequest
{
public:
	/** コンストラクタ
	* @param[in] raw httpサーバーからのJson要求
	*  {
	*	 "cmd" : "SetVIDValue",
	*	"var":[
	*	    {"RS485_Baudrate": "9600" ~ "57600" ~ "115200" ~ "1000000" or 0~255},
	*	    {"TTL_Baudrate":"9600" ~ "57600" ~ "115200" ~ "1000000" or 0~255},
	*	    {"RS232_Baudrate":"9600" ~ "57600" ~ "115200" ~ "1000000" or 0~255},
	*	    {"IO_PA_IO_Mode":4~7},
	*	    {"IO_PA_Analog_Mode":4~7},
	*	    {"IO_PA_PWM":0~1},
	*	    {"IO_PA_PWM_CYCLE":0~16383},
	*	    {"Through_Port":0~255},
	*	    {"Servo_Type_RS485":"None" ~ "FUTABA" ~ "ROBOTIS_v1.0" ~ ":ROBOTIS_v2.0" or 0~255},
	*	    {"Servo_Type_TTL":"None" ~ "FUTABA" ~ "ROBOTIS_v1.0" ~ ":ROBOTIS_v2.0" or 0~255},
	*	    {"IMU_Type": "None" or 0~255},
	*	    {"Balancer_Flag":0~1},
	*	    {"Theta_Th":0.1~10.0},
	*	    {"Cycletime":1~100},
	*	    {"Min_Cmp":1~250},
	*	    {"Flag_Ack":0~1},
	*	    {"Volt_Th":6.0~9.0},
	*	    {"Initialize_Torque":0~1},
	*	    {"Initialize_Angle":0~1},
	*	    {"Inspection_Flag":0~1},
	*	    {"Inspection_Type":0~1},
	*	    {"Robot_Model":"None" ~ "GR-001" ~ "DAWIN-MINI" or 0~255},
	*	    {"UID_Flag":0~1}
	*	]
	* }
	*/
    VarSetJSONRequest(picojson::object &raw);
	
	/** コマンドを実行する。
	* @param None
	* @return 返事のJSON文字列
	*/
    virtual string exec();
private:
    VarSetJSONRequest(void);
private:
    picojson::object &_raw;
	const map<string,tuple<int,int,int,int,bool>> _fields;
	const map<string,unsigned char> _builtinVars;
};

/**
* 各種変数要求
*/
class VarGetJSONRequest : public JSONRequest
{
public:
	/** コンストラクタ
	* @param[in] raw httpサーバーからのJson要求
	*  {
	*	"cmd" : "GetVIDValue",
	*	"var":[
	*	 "all",
	*    "RS485_Baudrate",
	*    "TTL_Baudrate",
	*    "RS232_Baudrate",
	*    "IO_PA_IO_Mode",
	*    "IO_PA_Analog_Mode",
	*    "IO_PA_PWM",
	*    "IO_PA_PWM_CYCLE",
	*    "Through_Port",
	*    "Servo_Type_RS485",
	*    "Servo_Type_TTL",
	*    "IMU_Type",
	*    "Balancer_Flag",
	*    "Theta_Th",
	*    "Cycletime",
	*    "Min_Cmp",
	*    "Flag_Ack",
	*    "Volt_Th",
	*    "Initialize_Torque",
	*    "Initialize_Angle",
	*    "Inspection_Flag",
	*    "Inspection_Type",
	*    "Robot_Model",
	*    "UID_Flag"
	*   ]
	*  }
	*/
    VarGetJSONRequest(picojson::object &raw);
	
	/** コマンドを実行する。
	* @param None
	* @return 返事のJSON文字列
	*/
    virtual string exec();
private:
    VarGetJSONRequest(void);
private:
    picojson::object &_raw;
	const map<string,tuple<int,int,int,int,bool>> _fields;
	const map<string,unsigned char> _builtinVars;
};
} // namespace VSido

#endif //__VISIDO_REQUEST_VAR_GET_HPP__
