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
#include "vsido_request_ik.hpp"
#include "vsido_response_ik.hpp"
using namespace VSido;
#include <string>
#include <iostream>
using namespace std;


#include "debug.h"


/** コンストラクタ
* @param[in] raw httpサーバーからのJson要求
*  {
*	 "cmd" : "ik",
*    "ikf": {
*      "cur" :{"torq":false,"pos":false,"rot":false},
*      "dist"    :{"torq":false,"pos":true,"rot":false},
*    },
*    "kdts":[
*      {
*        "kid":0~15,
*        "kdt": {
*          "pos"    :{"x"  :-100~100,"y"  :-100~100,"z" :-100~100},
*          "rot"    :{"Rx" :-100~100,"Ry" :-100~100,"Rz":-100~100},
*          "torq"      :{"Tx" :-100~100,"Ty" :-100~100,"Tz":-100~100},
*        }
*      }
*    ]
*    "kids":[
*        0~15,// kid
*    ]
*  }
*/

RequestIK::RequestIK(picojson::object &raw)
:Request()
,_raw(raw)
,_cur_pos(false)
,_cur_torq(false)
,_cur_rot(false)
,_dist_pos(false)
,_dist_torq(false)
,_dist_rot(false)
{
	_cmd = (unsigned char)'k';
//	_expect = shared_ptr<Response>(new ResponseIK({0xff}));
	const list<unsigned char> uart = {};
	_expect = make_shared<ResponseIK>(uart);
}

/** Json要求からVSidoパケットへ変換する
* @param None
* @return VSidoパケット
*/
list<unsigned char> RequestIK::conv(void)
{
	if(this->parseIKF())
	{
		this->parseKDT();
	}
	else
	{
		this->parseKID();
	}
	putCommon();
	return _uart;
}

bool RequestIK::parseIKF()
{
	bool ret = false;
	try
	{
		unsigned char ikflag = 0;
		{
			auto ikfV = _raw["ikf"];
			auto ikf = _raw["ikf"].get<picojson::object>();
			DUMP_VAR(&ikf);

/*			
			auto reserve = ikf["reserve"].get<picojson::object>();
			DUMP_VAR(&reserve);
			
			auto res1 = reserve["res1"].get<bool>();
			DUMP_VAR(res1);
			if(res1)
			{
				ikflag |=  0x80;
			}
			auto res2 = reserve["res2"].get<bool>();
			DUMP_VAR(res2);
			if(res2)
			{
				ikflag |=  0x40;
			}
*/
			if(ikfV.contains("cur"))
			{
				auto cur = ikf["cur"].get<picojson::object>();
				DUMP_VAR(&cur);
				auto torq = cur["torq"].get<bool>();
				DUMP_VAR(torq);
				if(torq)
				{
					ikflag |=  0x20;
					_cur_torq = true;
				}
				auto rot = cur["rot"].get<bool>();
				DUMP_VAR(rot);
				if(rot)
				{
					ikflag |=  0x10;
					_cur_rot = true;
				}
				auto pos = cur["pos"].get<bool>();
				DUMP_VAR(pos);
				if(pos)
				{
					ikflag |=  0x08;
					_cur_pos = true;
				}
				ret =  false;
			}

			if(ikfV.contains("dist"))
			{
				auto dist = ikf["dist"].get<picojson::object>();
				DUMP_VAR(&dist);			
				auto torq = dist["torq"].get<bool>();
				DUMP_VAR(torq);
				if(torq)
				{
					ikflag |=  0x04;
					_dist_pos = true;
				}
				auto rot = dist["rot"].get<bool>();
				DUMP_VAR(rot);
				if(rot)
				{
					ikflag |=  0x02;
					_dist_rot = true;
				}
				auto pos = dist["pos"].get<bool>();
				DUMP_VAR(pos);
				if(pos)
				{
					ikflag |=  0x01;
					_dist_pos = true;
				}
				ret =  true;
			}
			
		
		}
		DUMP_VAR(ikflag);
		_uart.push_back(ikflag);
		_length++;
	}
	catch(const std::runtime_error &e)
	{
		DUMP_VAR(e.what());
	}	
	return ret;
}

void RequestIK::parseKDT(void)
{
	try
	{
		auto kdt_s = _raw["kdts"].get<picojson::array>();
		DUMP_VAR(&kdt_s);
		for (auto it = kdt_s.begin(); it != kdt_s.end(); it++)
		{
			auto &kdt_object = it->get<picojson::object>();
			unsigned char kid = (unsigned char)kdt_object["kid"].get<double>();
			DUMP_VAR(kid);
			_uart.push_back(kid);
			_length++;
			
			auto kdt = kdt_object["kdt"].get<picojson::object>();
			DUMP_VAR(&kdt);
			
			auto pos = kdt["pos"].get<picojson::object>();
			DUMP_VAR(&pos);
			if(_dist_pos)
			{
				int x = (int) pos["x"].get<double>()  + 100;
				_uart.push_back((unsigned char)x);
				_length++;
				int y = (int) pos["y"].get<double>() + 100;
				_uart.push_back((unsigned char)y);
				_length++;
				int z = (int) pos["z"].get<double>() + 100;
				_uart.push_back((unsigned char)z);
				_length++;
			}
			auto rot = kdt["rot"].get<picojson::object>();
			DUMP_VAR(&rot);
			if(_dist_rot)
			{
				int Rx = (int) rot["Rx"].get<double>() + 100;
				_uart.push_back((unsigned char)Rx);
				_length++;
				int Ry = (int) rot["Ry"].get<double>() + 100;
				_uart.push_back((unsigned char)Ry);
				_length++;
				int Rz = (int) rot["Rz"].get<double>() + 100;
				_uart.push_back((unsigned char)Rz);
				_length++;
			}

			auto torq = kdt["torq"].get<picojson::object>();
			DUMP_VAR(&torq);
			if(_dist_torq)
			{
				int Tx = (int) torq["Tx"].get<double>() + 100;
				_uart.push_back((unsigned char)Tx);
				_length++;
				int Ty = (int) torq["Ty"].get<double>() + 100;
				_uart.push_back((unsigned char)Ty);
				_length++;
				int Tz = (int) torq["Tz"].get<double>() + 100;
				_uart.push_back((unsigned char)Tz);
				_length++;
			}
		}	
	}
	catch(const std::runtime_error &e)
	{
		DUMP_VAR(e.what());
	}
}

void RequestIK::parseKID(void)
{
	try
	{
		auto kid_s = _raw["kids"].get<picojson::array>();
		DUMP_VAR(&kid_s);
		for (auto it = kid_s.begin(); it != kid_s.end(); it++)
		{
			auto kid = it->get<double>();
			DUMP_VAR(kid);
			_uart.push_back((unsigned char)kid);
			_length++;
		}
	}
	catch(const std::runtime_error &e)
	{
		DUMP_VAR(e.what());
	}
}

