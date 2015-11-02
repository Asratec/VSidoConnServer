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
#ifndef __VISIDO_REQUEST_IK_HPP__
#define __VISIDO_REQUEST_IK_HPP__


#include <list>
#include <string>
using namespace std;
#include "cmd_common.hpp"
#include "picojson.h"

namespace VSido
{
class IKSetRequest;
class IKReadRequest;


/**
* IK設定
*/
class IKSetJSONRequest : public JSONRequest
{
public:
	/** コンストラクタ
	* @param[in] raw httpサーバーからのJson要求
	*  {
	*	 "command" : "SetIK",
    *    "ikflag": {
    *      "torque":false,"position":true,"rotation":false
    *    },
    *    "kdt":[
    *      {
    *        "kid":"body" ~ "head" ~ "right_hand" ~ "left_hand" ~ "right_foot" ~ "left_foot" or 0 ~ 15,
    *        "position"    :{"x" :-100~100, "y" :-100~100, "z":-100~100},
    *        "rotation"    :{"x" :-100~100, "y" :-100~100, "z":-100~100},
    *        "torque"   :{"x" :-100~100, "y" :-100~100, "z":-100~100}
    *      }
    *    ]
	*  }
	*/
    IKSetJSONRequest(picojson::object &raw);
	
	/** コマンドを実行する。
	* @param None
	* @return 返事のJSON文字列
	*/
    virtual string exec();
private:
    IKSetJSONRequest(void);
	void parseIKF(IKSetRequest &req);
	void parseKDT(IKSetRequest &req);
private:
    picojson::object &_raw;
	bool _dist_pos;
	bool _dist_torq;
	bool _dist_rot;
};

/**
* IK読み
*/
class IKGetJSONRequest : public JSONRequest
{
public:
	/** コンストラクタ
	* @param[in] raw httpサーバーからのJson要求
	*  {
	*	 "command" : "GetIK",
    *    "ikflag": {
    *      "torque":false,"position":false,"rotation":false
    *    },
    *    "kid":[
    *        "body" ~ "head" ~ "right_hand" ~ "left_hand" ~ "right_foot" ~ "left_foot" or 0 ~ 15,
    *    ]
	*  }
	*/
    IKGetJSONRequest(picojson::object &raw);
	
	/** コマンドを実行する。
	* @param None
	* @return 返事のJSON文字列
	*/
    virtual string exec();
private:
    IKGetJSONRequest();
	void parseIKF(IKReadRequest &req);
	void parseKID(IKReadRequest &req);
private:
    picojson::object &_raw;
	bool _cur_pos;
	bool _cur_torq;
	bool _cur_rot;
};

/**
* IK設定 
* This method is deprecated. Use IKSetJSONRequest IKGetJSONRequest
*/
class IKJSONRequest : public JSONRequest
{
public:
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
    IKJSONRequest(picojson::object &raw);
	
	/** コマンドを実行する。
	* @param None
	* @return 返事のJSON文字列
	*/
    virtual string exec();
private:
    IKJSONRequest(void);
	bool parseIKF();
	void parseKDT(IKSetRequest &req);
	void parseKID(IKReadRequest &req);
private:
    picojson::object &_raw;
	bool _cur_pos;
	bool _cur_torq;
	bool _cur_rot;
	bool _dist_pos;
	bool _dist_torq;
	bool _dist_rot;
};


} // namespace VSido

#endif //__VISIDO_REQUEST_IK_HPP__
