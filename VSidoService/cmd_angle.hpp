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
#ifndef __VISIDO_REQUEST_ANGLE_HPP__
#define __VISIDO_REQUEST_ANGLE_HPP__


#include <list>
#include <string>
using namespace std;
#include "cmd_common.hpp"
#include "picojson.h"

namespace VSido
{
/**
* 目標角度設定
*/
class AngleJSONRequest : public JSONRequest
{
public:
	/** コンストラクタ
	* @param[in] raw httpサーバーからのJson要求
	*  {
    *    "cmd": "servoAngle",
    *    "cycle": 1~100,
    *    "servo":[
    *      {"sid":1~127,"angle":-180.0~180.0}
    *    ]
    *  }
	*/
    AngleJSONRequest(picojson::object &raw);

	/** コマンドを実行する。
	* @param None
	* @return 返事のJSON文字列
	*/
    virtual string exec();
private:
    AngleJSONRequest(void);
private:
    picojson::object &_raw;
};
	
/**
* サーボ角度情報
*/
class AngleGetJSONRequest : public JSONRequest
{
public:
	/** コンストラクタ
	* @param[in] raw httpサーバーからのJson要求
	*  {
	*	 "cmd" : "GetServoAngle",
	*	 "servo"  : [
	*	   {"sid":1~254}
	*	 ]
	*  }
	*/
    AngleGetJSONRequest(picojson::object &raw);
	
	/** コマンドを実行する。
	* @param None
	* @return 返事のJSON文字列
	*/
    virtual string exec();
private:
    AngleGetJSONRequest(void);
private:
    picojson::object &_raw;
};

/**
* 最大・最少角度設定
*/
class MinMaxAngleJSONRequest : public JSONRequest
{
public:
	/** コンストラクタ
	* @param[in] raw httpサーバーからのJson要求
	*  {
	*	  "cmd" : "SetServoMinMaxAngle",
	*	  "angle"   : [
		   {"sid":1~254,"min":-180.0~180.0,"max":-180.0~180.0}
	*	  ]
	*  }
	*/
    MinMaxAngleJSONRequest(picojson::object &raw);

	/** コマンドを実行する。
	* @param None
	* @return 返事のJSON文字列
	*/
    virtual string exec();
private:
    MinMaxAngleJSONRequest(void);
private:
    picojson::object &_raw;
};

} // namespace VSido

#endif //__VISIDO_REQUEST_ANGLE_HPP__
