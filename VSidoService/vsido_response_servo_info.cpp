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
#include "vsido_response_servo_info.hpp"
using namespace VSido;
#include <string>
#include <iostream>
using namespace std;


#include "debug.h"

/** コンストラクタ
* @param[in] uart VSidoからの返事 サーボ情報
*/
ResponseServoInfo::ResponseServoInfo(const list<unsigned char> &uart)
:Response(uart)
,_servos()
{
	_jsonObj["type"] = picojson::value("servoinfo");
}


/** VSidoの返事をJsonに変換する
* @param None
* @return Jsonデータ
* {
* 	"type": "servoinfo",
* 	"servo":[
* 	  {
* 	    "sid":1,
* 	    "dat": 100
* 	  },
* 	  {
* 	    "sid":2,
* 	    "dat": 200
* 	  }
* 	]
* }	
*/
string ResponseServoInfo::conv(void)
{
	this->splite();
	picojson::array servoskArray;
	for(auto &servo :_servos)
	{
		picojson::object servoObj;
		servoObj["sid"] = picojson::value((double)servo.front());
		servo.pop_front();

		unsigned char low_byte = servo.front();
		servo.pop_front();
		unsigned char high_byte = servo.front();
		servo.pop_front();

		
//		int iAngle1 = (int)(angle1 & 0x80);
//		int iAngle2 = (int)(angle2 & 0xff);
//		DUMP_VAR(iAngle1);
//		DUMP_VAR(iAngle2);		
//		int angle = ( (((iAngle1 >> 1) << 8) & 0xff00) + iAngle2) >> 1;
		
		short data_low = low_byte;
		short data_high = (short)((high_byte & 0x80) | (high_byte) >> 1);
		short combined_data = (short)(((data_high << 8) & 0x0000FF00) | ((data_low) & 0x000000FF));

		short converted_data = (short)((combined_data & 0x8000) | (combined_data >> 1));

		
		
		servoObj["angle"] = picojson::value(((double)converted_data)/10);
		
		servoskArray.push_back(picojson::value(servoObj));
	}
	_jsonObj["servo"] = picojson::value(servoskArray);
	return Response::conv();
}


void ResponseServoInfo::splite(void)
{
	list<unsigned char> servo;
	int i = 0;
	for(auto ch :_uart)
	{
		servo.push_back(ch);
		if(0 == ++i%3)
		{
			_servos.push_back(servo);
		}
	}
}


