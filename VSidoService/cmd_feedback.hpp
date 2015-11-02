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
#ifndef __VISIDO_REQUEST_FEEDBACK_HPP__
#define __VISIDO_REQUEST_FEEDBACK_HPP__


#include <list>
#include <string>
using namespace std;
#include "cmd_common.hpp"
#include "picojson.h"

namespace VSido
{

/**
* フィードバックID設定
*/
class IDFeedbackJSONRequest : public JSONRequest
{
public:
	/** コンストラクタ
	* @param[in] raw httpサーバーからのJson要求
	*  {
	*	  "cmd" : "SetFeedbackID",
	*	  "servo"   : [
		   sid(1~254)
	*	  ]
	*  }
	*/
    IDFeedbackJSONRequest(picojson::object &raw);

	/** コマンドを実行する。
	* @param None
	* @return 返事のJSON文字列
	*/
    virtual string exec();
private:
    IDFeedbackJSONRequest(void);
private:
    picojson::object &_raw;
};


/**
* フィードバック要求
*/
class FeedbackJSONRequest : public JSONRequest
{
public:
	/** コンストラクタ
	* @param[in] raw httpサーバーからのJson要求
	*  {
	*	"cmd" : "GetServoFeedback",
	*	"item":[
	*		"rom_model_num",
	*		"rom_servo_ID",
	*		"rom_cw_agl_lmt",
	*		"rom_ccw_agl_lmt",
	*		"rom_damper",
	*		"rom_cw_cmp_margin",
	*		"rom_ccw_cmp_margin",
	*		"rom_cw_cmp_slope",
	*		"rom_ccw_cmp_slope",
	*		"rom_punch",
	*		"ram_goal_pos",
	*		"ram_goal_tim",
	*		"ram_max_torque",
	*		"ram_torque_mode",
	*		"ram_pres_pos",
	*		"ram_pres_time",
	*		"ram_pres_spd",
	*		"ram_pres_curr",
	*		"ram_pres_temp",
	*		"ram_pres_volt",
	*		"Flags",
	*		"alg_ofset",
	*		"parents_ID",
	*		"connected",
	*		"read_time",
	*		"_ram_goal_pos",
	*		"__ram_goal_pos",
	*		"_ram_res_pos",
	*		"_send_speed",
	*		"_send_cmd_time",
	*		"flg_min_max",
	*		"flg_goal_pos",
	*		"flg_parent_inv",
	*		"flg_cmp_slope",
	*		"flg_check_angle",
	*		"port_type",
	*		"servo_type"
	*	]
	*  }
	*/
    FeedbackJSONRequest(picojson::object &raw);

	/** コマンドを実行する。
	* @param None
	* @return 返事のJSON文字列
	*/
    virtual string exec();
private:
    FeedbackJSONRequest(void);
private:
    picojson::object &_raw;
	const map<int,tuple<string,bool>> _fields;
	vector<int> _offsets;
	vector<string> _angleType;
};
} // namespace VSido

#endif //__VISIDO_REQUEST_FEEDBACK_HPP__
