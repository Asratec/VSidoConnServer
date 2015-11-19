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
#include "cmd_ik.hpp"
#include "VSido.hpp"
using namespace VSido;
#include <string>
#include <iostream>
#include <map>
using namespace std;


#include "debug.h"


static const map<string,int> gConstKidMap(
{
	{"body",0},
	{"head",1},
	{"right_hand",2},
	{"left_hand",3},
	{"right_foot",4},
	{"left_foot",5}
});




/** コンストラクタ
* @param[in] raw httpサーバーからのJson要求
*  {
*	 "command" : "SetIK",
*    "ikflag": {
*      		"torque":false,"position":true,"rotation":false
*    },
*    "kdt":[
*      {
*        "kid":"body" ~ "head" ~ "right_hand" ~ "left_hand" ~ "right_foot" ~ "left_foot" or 0 ~ 15,
*        "position"    :{"x"  :-100~100,"y"  :-100~100,"z" :-100~100},
*        "rotation"    :{"Rx" :-100~100,"Ry" :-100~100,"Rz":-100~100},
*        "torque"   :{"Tx" :-100~100,"Ty" :-100~100,"Tz":-100~100}
*      }
*    ]
*  }
*/
IKSetJSONRequest::IKSetJSONRequest(picojson::object &raw)
:JSONRequest()
,_raw(raw)
,_dist_pos(false)
,_dist_torq(false)
,_dist_rot(false)
{
}

/** コマンドを実行する。
* @param None
* @return 返事のJSON文字列
*/
string IKSetJSONRequest::exec()
{
	IKSetRequest req;
	this->parseIKF(req);
	this->parseKDT(req);
	if(req)
	{
		req.execNA();
		return Ack();
	}
	else
	{
		return req.msg();
	}
}

void IKSetJSONRequest::parseIKF(IKSetRequest &req)
{
	try
	{
		auto ikfV = _raw["ikflag"];
		auto ikf = _raw["ikflag"].get<picojson::object>();
		DUMP_VAR(&ikf);
		if(ikfV.contains("torque"))
		{
			auto torq = ikf["torque"].get<bool>();
			DUMP_VAR(torq);
			if(torq)
			{
				_dist_torq = true;
				req.torque();
			}
		}
		if(ikfV.contains("rotation"))
		{
			auto rot = ikf["rotation"].get<bool>();
			DUMP_VAR(rot);
			if(rot)
			{
				_dist_rot = true;
				req.rotation();
			}
		}
		if(ikfV.contains("position"))
		{
			auto pos = ikf["position"].get<bool>();
			DUMP_VAR(pos);
			if(pos)
			{
				_dist_pos = true;
				req.position();
			}
		}
	}
	catch(const std::runtime_error &e)
	{
		FATAL_VAR(e.what());
	}	
}

void IKSetJSONRequest::parseKDT(IKSetRequest &req)
{
	try
	{
		auto kdt_s = _raw["kdt"].get<picojson::array>();
		DUMP_VAR(&kdt_s);
		for (auto it = kdt_s.begin(); it != kdt_s.end(); it++)
		{
			auto &kdt_object = it->get<picojson::object>();
			if(false == it->contains("kid"))
			{
				break;
			}
			unsigned char kid = 255;
			if(kdt_object["kid"].is<string>())
			{
				auto valueStr = kdt_object["kid"].get<string>();
				auto itBI = gConstKidMap.find(valueStr);
				if(itBI != gConstKidMap.end())
				{
					kid = itBI->second;
				}
			}
			else
			{
				kid = (unsigned char)kdt_object["kid"].get<double>();
			}
			DUMP_VAR(kid);
			if(255 == kid)
			{
				break;
			}
			
			if(it->contains("position"))
			{
				auto pos = kdt_object["position"].get<picojson::object>();
				DUMP_VAR(&pos);
				if(_dist_pos)
				{
					signed char x = (signed char) pos["x"].get<double>();
					signed char y = (signed char) pos["y"].get<double>();
					signed char z = (signed char) pos["z"].get<double>();
					req.position(kid,x,y,z);
					DUMP_VAR(int(x));
					DUMP_VAR(int(y));
					DUMP_VAR(int(z));
				}
			}
			if(it->contains("rotation"))
			{
				auto rot = kdt_object["rotation"].get<picojson::object>();
				DUMP_VAR(&rot);
				if(_dist_rot)
				{
					signed char x = (signed char) rot["x"].get<double>();
					signed char y = (signed char) rot["y"].get<double>();
					signed char z = (signed char) rot["z"].get<double>();
					req.rotation(kid,x,y,z);
					DUMP_VAR(int(x));
					DUMP_VAR(int(y));
					DUMP_VAR(int(z));
				}
			}

			if(it->contains("torque"))
			{
				auto torq = kdt_object["torque"].get<picojson::object>();
				DUMP_VAR(&torq);
				if(_dist_torq)
				{
					signed char x = (signed char) torq["x"].get<double>();
					signed char y = (signed char) torq["y"].get<double>();
					signed char z = (signed char) torq["z"].get<double>();
					req.torque(kid,x,y,z);
					DUMP_VAR(int(x));
					DUMP_VAR(int(y));
					DUMP_VAR(int(z));
				}
			}
		}	
	}
	catch(const std::runtime_error &e)
	{
		FATAL_VAR(e.what());
	}
}




/** コンストラクタ
* @param[in] raw httpサーバーからのJson要求
*  {
*	 "cmd" : "ik",
*    "ikflag": {
*      "torque":false,"position":false,"rotation":false
*    },
*    "kid":[
*        "body" ~ "head" ~ "right_hand" ~ "left_hand" ~ "right_foot" ~ "left_foot" or 0 ~ 15,
*    ]
*  }
*/
IKGetJSONRequest::IKGetJSONRequest(picojson::object &raw)
:JSONRequest()
,_raw(raw)
,_cur_pos(false)
,_cur_torq(false)
,_cur_rot(false)
{
}

/** コマンドを実行する。
* @param None
* @return 返事のJSON文字列
*/
string IKGetJSONRequest::exec()
{
	IKReadRequest req;
	this->parseIKF(req);
	this->parseKID(req);
	if(req)
	{
		auto fn = [this](IKResponse &resp) {
			if(resp.timeout())
			{
				_res["type"] = picojson::value(string("timeout"));
				picojson::value resValue(_res);
				Dispatcher::onResponse(uid_,resValue.serialize());
				return ;
			}
			if(resp)
			{
				_res["type"] = picojson::value(string("GetIK"));
				string raw = static_cast<string>(resp);
				_res["raw"] = picojson::value(raw);
				
				picojson::object ikf;
				if(resp.position())
				{
					ikf["position"] = picojson::value(true);
				}
				else
				{
					ikf["position"] = picojson::value(false);
				}
				if(resp.rotation())
				{
					ikf["rotation"] = picojson::value(true);
				}
				else
				{
					ikf["rotation"] = picojson::value(false);
				}
				if(resp.torque())
				{
					ikf["torque"] = picojson::value(true);
				}
				else
				{
					ikf["torque"] = picojson::value(false);
				}
				_res["ikflag"] = picojson::value(ikf);
				
				picojson::array kdtsArray;

				resp.forEach( [this,&kdtsArray,&resp](unsigned char kid,tuple<signed char,signed char,signed char,signed char,signed char,signed char,signed char,signed char,signed char>kdt){
					picojson::object ikData;
					
					string strKid;
					for(auto pairKid : gConstKidMap)
					{
						if(pairKid.second == kid)
						{
							strKid = pairKid.first;
						}
					}
					if(strKid.empty())
					{
						ikData["kid"] =  picojson::value(double(kid));
					}
					else
					{
						ikData["kid"] =  picojson::value(strKid);
					}
					
					picojson::object kdtObj;
					
					if(resp.position())
					{
						picojson::object obj;
						obj["x"] = picojson::value((double)std::get<0>(kdt));
						obj["y"] = picojson::value((double)std::get<1>(kdt));
						obj["z"] = picojson::value((double)std::get<2>(kdt));
						ikData["position"] = picojson::value(obj);
					}
					if(resp.rotation())
					{
						picojson::object obj;
						obj["x"] = picojson::value((double)std::get<3>(kdt));
						obj["y"] = picojson::value((double)std::get<4>(kdt));
						obj["z"] = picojson::value((double)std::get<5>(kdt));
						ikData["rotation"] = picojson::value(obj);
					}
					if(resp.torque())
					{
						picojson::object obj;
						obj["x"] = picojson::value((double)std::get<6>(kdt));
						obj["y"] = picojson::value((double)std::get<7>(kdt));
						obj["z"] = picojson::value((double)std::get<8>(kdt));
						ikData["torque"] = picojson::value(obj);
					}
					
					kdtsArray.push_back(picojson::value(ikData));
				}
				);
				_res["kdt"] = picojson::value(kdtsArray);
				picojson::value resValue(_res);
				Dispatcher::onResponse(uid_,resValue.serialize());
				return ;
			}
			else
			{
				_res["type"] = picojson::value(string("error"));
				_res["detail"] = picojson::value(string(""));
				picojson::value resValue(_res);
				Dispatcher::onResponse(uid_,resValue.serialize());
				return ;
			}
		};
		uid_ = req.exec(fn);
		return "";
	}
	else
	{
		return req.msg();
	}
}


void IKGetJSONRequest::parseIKF(IKReadRequest &req)
{
	try
	{
		auto ikfV = _raw["ikflag"];
		auto ikf = _raw["ikflag"].get<picojson::object>();
		DUMP_VAR(&ikf);
		if(ikfV.contains("torque"))
		{
			auto torq = ikf["torque"].get<bool>();
			DUMP_VAR(torq);
			if(torq)
			{
				_cur_torq = true;
				req.torque();
			}
		}
		if(ikfV.contains("rotation"))
		{
			auto rot = ikf["rotation"].get<bool>();
			DUMP_VAR(rot);
			if(rot)
			{
				_cur_rot = true;
				req.rotation();
			}
		}
		if(ikfV.contains("position"))
		{
			auto pos = ikf["position"].get<bool>();
			DUMP_VAR(pos);
			if(pos)
			{
				_cur_pos = true;
				req.position();
			}
		}
	}
	catch(const std::runtime_error &e)
	{
		FATAL_VAR(e.what());
	}	
}



void IKGetJSONRequest::parseKID(IKReadRequest &req)
{
	try
	{
		auto kid_s = _raw["kid"].get<picojson::array>();
		DUMP_VAR(&kid_s);
		for (auto it = kid_s.begin(); it != kid_s.end(); it++)
		{
			unsigned char kid = 255;
			if(it->is<string>())
			{
				auto valueStr = it->get<string>();
				auto itBI = gConstKidMap.find(valueStr);
				if(itBI != gConstKidMap.end())
				{
					kid = itBI->second;
				}
			}
			else
			{
				kid = (unsigned char)it->get<double>();
			}
			DUMP_VAR(kid);
			if(255 == kid)
			{
				break;
			}
			
			DUMP_VAR(kid);
			req.kid(kid);
		}
	}
	catch(const std::runtime_error &e)
	{
		DUMP_VAR(e.what());
	}
}



/** コンストラクタ
* @param[in] raw httpサーバーからのJson要求
*  {
*	 "cmd" : "ik",
*    "ikf": {
*      "cur" :{"torq":false,"pos":false,"rot":false},
*      "dist"    :{"torq":false,"pos":false,"rot":false},
*    },
*    "kids":[
*        "body" ~ "head" ~ "right_hand" ~ "left_hand" ~ "right_foot" ~ "left_foot" or 0 ~ 15,
*    ]
*    "kdts":[
*      {
*        "kid":0~15,
*        "kdt": {
*          [
*              "pos"    :{"x"  :-100~100,"y"  :-100~100,"z" :-100~100},
*              "rot"    :{"Rx" :-100~100,"Ry" :-100~100,"Rz":-100~100},
*              "torq"   :{"Tx" :-100~100,"Ty" :-100~100,"Tz":-100~100}
*          ]
*        }
*      }
*    ]
*  }
*/
IKJSONRequest::IKJSONRequest(picojson::object &raw)
:JSONRequest()
,_raw(raw)
,_dist_pos(false)
,_dist_torq(false)
,_dist_rot(false)
,_cur_pos(false)
,_cur_torq(false)
,_cur_rot(false)
{
}

/** コマンドを実行する。
* @param None
* @return 返事のJSON文字列
*/
string IKJSONRequest::exec()
{
	if(this->parseIKF())
	{
		IKSetRequest req;
		if(_dist_pos)
		{
			req.position();
		}
		if(_dist_torq)
		{
			req.torque();
		}
		if(_dist_rot)
		{
			req.rotation();
		}
		this->parseKDT(req);
		if(req)
		{
			req.execNA();
			return Ack();
		}
		else
		{
			return req.msg();
		}
	}
	else
	{
		IKReadRequest req;
		if(_cur_pos)
		{
			req.position();
		}
		if(_cur_torq)
		{
			req.torque();
		}
		if(_cur_rot)
		{
			req.rotation();
		}
		this->parseKID(req);
		if(req)
		{
			auto fn = [this](IKResponse &resp) {
				Dispatcher::onResponse(uid_,"{}");
				return ;
			};
			uid_ = req.exec(fn);
			return "";
		}
		else
		{
			return req.msg();
		}
	}
}

bool IKJSONRequest::parseIKF()
{
	bool ret = false;
	try
	{
		auto ikfV = _raw["ikf"];
		auto ikf = _raw["ikf"].get<picojson::object>();
		DUMP_VAR(&ikf);
		if(ikfV.contains("dist"))
		{
			auto dist = ikf["dist"].get<picojson::object>();
			DUMP_VAR(&dist);			
			auto torq = dist["torq"].get<bool>();
			DUMP_VAR(torq);
			if(torq)
			{
				_dist_torq = true;
			}
			auto rot = dist["rot"].get<bool>();
			DUMP_VAR(rot);
			if(rot)
			{
				_dist_rot = true;
			}
			auto pos = dist["pos"].get<bool>();
			DUMP_VAR(pos);
			if(pos)
			{
				_dist_pos = true;
			}
			ret = true;
		}
		if(ikfV.contains("cur"))
		{
			auto cur = ikf["cur"].get<picojson::object>();
			DUMP_VAR(&cur);
			auto torq = cur["torq"].get<bool>();
			DUMP_VAR(torq);
			if(torq)
			{
				_cur_torq = true;
			}
			auto rot = cur["rot"].get<bool>();
			DUMP_VAR(rot);
			if(rot)
			{
				_cur_rot = true;
			}
			auto pos = cur["pos"].get<bool>();
			DUMP_VAR(pos);
			if(pos)
			{
				_cur_pos = true;
			}
		}
	}
	catch(const std::runtime_error &e)
	{
		FATAL_VAR(e.what());
	}
	return ret;
}


void IKJSONRequest::parseKDT(IKSetRequest &req)
{
	try
	{
		auto kdt_s = _raw["kdts"].get<picojson::array>();
		DUMP_VAR(&kdt_s);
		for (auto it = kdt_s.begin(); it != kdt_s.end(); it++)
		{
			auto &kdt_object = it->get<picojson::object>();
			unsigned char kid = (unsigned char)kdt_object["kid"].get<double>();
			DUMP_VAR((int)kid);
			
			auto kdt = kdt_object["kdt"].get<picojson::object>();
			DUMP_VAR(&kdt);
			
			if(_dist_pos)
			{
				auto pos = kdt["pos"].get<picojson::object>();
				DUMP_VAR(&pos);
				char x = (char) pos["x"].get<double>();
				char y = (char) pos["y"].get<double>();
				char z = (char) pos["z"].get<double>();
				req.position(kid,x,y,z);
			}
			if(_dist_rot)
			{
				auto rot = kdt["rot"].get<picojson::object>();
				DUMP_VAR(&rot);
				char Rx = (char) rot["Rx"].get<double>();
				char Ry = (char) rot["Ry"].get<double>();
				char Rz = (char) rot["Rz"].get<double>();
				req.rotation(kid,Rx,Ry,Rz);
			}

			if(_dist_torq)
			{
				auto torq = kdt["torq"].get<picojson::object>();
				DUMP_VAR(&torq);
				char Tx = (char) torq["Tx"].get<double>();
				char Ty = (char) torq["Ty"].get<double>();
				char Tz = (char) torq["Tz"].get<double>();
				req.torque(kid,Tx,Ty,Tz);
			}
		}	
	}
	catch(const std::runtime_error &e)
	{
		FATAL_VAR(e.what());
	}
}


void IKJSONRequest::parseKID(IKReadRequest &req)
{
	try
	{
		auto kid_s = _raw["kids"].get<picojson::array>();
		DUMP_VAR(&kid_s);
		for (auto it = kid_s.begin(); it != kid_s.end(); it++)
		{
			unsigned char  kid = (unsigned char )(it->get<double>());
			DUMP_VAR(kid);
			req.kid(kid);
		}
	}
	catch(const std::runtime_error &e)
	{
		FATAL_VAR(e.what());
	}
}


