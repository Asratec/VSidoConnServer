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
#include "vsido_response_ik.hpp"
using namespace VSido;
#include <string>
#include <iostream>
using namespace std;


#include "debug.h"

/** コンストラクタ
* @param[in] uart VSidoからの返事 IK設定
*/
ResponseIK::ResponseIK(const list<unsigned char> &uart)
:Response(uart)
,_cur_pos(false)
,_cur_torq(false)
,_cur_rot(false)
,_dist_pos(false)
,_dist_torq(false)
,_dist_rot(false)
,_kdts()
{
	_jsonObj["type"] = picojson::value("ik");
	DUMP_VAR(&_jsonObj);
}


/** VSidoの返事をJsonに変換する
* @param None
* @return Jsonデータ
* {
* 	"type": "ik",
* 	"ikf": {
* 	  "cur" :{"torq":true,"pos":true,"rot":true},
* 	  "dist"    :{"torq":false,"pos":false,"rot":false},
* 	},
* 	"kdts":[
* 	  {
* 	    "kid":1,
* 	    "kdt": {
* 	      "pos"     :{"x":x,"y":y,"z":0},
* 	      "rot"    :{"Rx":0,"Ry":0,"Rz":0},
* 	      "torq"      :{"Tx":0,"Ty":0,"Tz":0},
* 	    }
* 	  }
* 	]
* }	
*/
string ResponseIK::conv(void)
{
	parseIKF();
	
	spliteKDT();
	parseKDT();
	return Response::conv();
}

void ResponseIK::parseIKF(void)
{
	try 
	{
		if(_uart.empty())
		{
			throw string("data error");
		}
		unsigned char ikf = _uart.front();
		_uart.pop_front();
		picojson::object ikfObj;
		picojson::object reserve;
		picojson::object cur;
		picojson::object dist;

		if( 0x20 &ikf )
		{
			cur["torq"] = picojson::value(true);
			_cur_torq = true;
		}
		else
		{
			cur["torq"] = picojson::value(false);
		}
		if( 0x10 &ikf )
		{
			cur["rot"] = picojson::value(true);
			_cur_rot = true;
		}
		else
		{
			cur["rot"] = picojson::value(false);
		}
		if( 0x08 &ikf )
		{
			cur["pos"] = picojson::value(true);
			_cur_pos = true;
		}
		else
		{
			cur["pos"] = picojson::value(false);
		}
		ikfObj["cur"] = picojson::value(cur);

		_jsonObj["ikf"] = picojson::value(ikfObj);
	}
	catch(const string &err)
	{
		DUMP_VAR(err);
	}

}

void ResponseIK::spliteKDT(void)
{
	int kdtUnit = 1; // kid 1 byte.
	if(_cur_torq)
	{
		kdtUnit += 3; // Tx,Ty,Tz. 3byte
	}
	if(_cur_rot)
	{
		kdtUnit += 3; // Rx,Ry,Rz. 3byte
	}
	if(_cur_pos)
	{
		kdtUnit += 3; // x,y,z. 3byte
	}
	DUMP_VAR(kdtUnit);
	/// 最少4バイト
	if(4 > kdtUnit)
	{
		return;
	}
	list<unsigned char> kdt;
	int i = 0;
	for(auto ch :_uart)
	{
		kdt.push_back(ch);
		if(0 == ++i%kdtUnit)
		{
			_kdts.push_back(kdt);
		}
	}
	DUMP_VAR(_kdts.size());
}


void ResponseIK::parseKDT(void)
{
	try
	{
		picojson::array kdtsArray;
		for(auto &kdt :_kdts)
		{
			picojson::object kdtObj;
			if(kdt.empty())
			{
				throw string("data error");
			}
			kdtObj["kid"] = picojson::value((double)kdt.front());
			kdt.pop_front();
			picojson::object kdtValueObj;
			if(_cur_pos)
			{
				picojson::object pos;
				
				if(kdt.empty())
				{
					throw string("data error");
				}
				auto x = (int)kdt.front() - 100;
				pos["x"] = picojson::value((double)x);
				kdt.pop_front();
				
				if(kdt.empty())
				{
					throw string("data error");
				}
				auto y =  (int)kdt.front() - 100;
				pos["y"] = picojson::value((double)y);
				kdt.pop_front();

				if(kdt.empty())
				{
					throw string("data error");
				}
				auto z =  (int)kdt.front() - 100;
				pos["z"] = picojson::value((double)z);
				kdt.pop_front();
				kdtValueObj["pos"] = picojson::value(pos);
			}		
			if(_cur_rot)
			{
				picojson::object rot;
				
				if(kdt.empty())
				{
					throw string("data error");
				}
				auto Rx =  (int)kdt.front() - 100;
				rot["Rx"] = picojson::value((double)Rx);
				kdt.pop_front();

				if(kdt.empty())
				{
					throw string("data error");
				}
				auto Ry =  (int)kdt.front() - 100;
				rot["Ry"] = picojson::value((double)Ry);
				kdt.pop_front();

				if(kdt.empty())
				{
					throw string("data error");
				}
				auto Rz =  (int)kdt.front() - 100;
				rot["Rz"] = picojson::value((double)Rz);
				kdt.pop_front();
				kdtValueObj["rot"] = picojson::value(rot);
			}
			if(_cur_torq)
			{
				picojson::object torq;

				if(kdt.empty())
				{
					throw string("data error");
				}
				auto Tx =  (int)kdt.front() - 100;
				torq["Tx"] = picojson::value((double)Tx);
				kdt.pop_front();

				if(kdt.empty())
				{
					throw string("data error");
				}
				auto Ty =  (int)kdt.front() - 100;
				torq["Ty"] = picojson::value((double)Ty);
				kdt.pop_front();

				if(kdt.empty())
				{
					throw string("data error");
				}
				auto Tz =  (int)kdt.front() - 100;
				torq["Tz"] = picojson::value((double)Tz);
				kdt.pop_front();
				kdtValueObj["torq"] = picojson::value(torq);
			}
			
			kdtObj["kdt"] = picojson::value(kdtValueObj);
			
			kdtsArray.push_back(picojson::value(kdtObj));
		}
		_jsonObj["kdts"] = picojson::value(kdtsArray);
	}
	catch(const string &err)
	{
		DUMP_VAR(err);
	}
}

