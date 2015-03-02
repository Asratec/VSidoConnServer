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
#include "vsido_response_acceleration.hpp"
using namespace VSido;
#include <string>
#include <iostream>
using namespace std;


#include "debug.h"

/** コンストラクタ
*/
ResponseAcceleration::ResponseAcceleration(void)
{
	_jsonObj["type"] = picojson::value("acceleration");
}

/** コンストラクタ
* @param[in] uart VSidoからの返事 加速度センサ情報
*/
ResponseAcceleration::ResponseAcceleration(const list<unsigned char> &uart)
:Response(uart)
{
	_jsonObj["type"] = picojson::value("acceleration");
}


/** VSidoの返事をJsonに変換する
* @param None
* @return Jsonデータ
* {
* 	"type": "acceleration",
* 	"ax":10.0000,
* 	"ay":10.0000,
* 	"az":10.0000
* }	
*/
string ResponseAcceleration::conv(void)
{
	picojson::array vdtArray;
	_jsonObj["ax"] = picojson::value((double)_uart.front());
	_uart.pop_front();
	_jsonObj["ay"] = picojson::value((double)_uart.front());
	_uart.pop_front();
	_jsonObj["az"] = picojson::value((double)_uart.front());
	_uart.pop_front();
	return Response::conv();
}


