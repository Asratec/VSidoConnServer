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
#include "vsido_response_feedback.hpp"
using namespace VSido;
#include <string>
#include <iostream>
using namespace std;


#include "debug.h"


/** コンストラクタ
* @param[in] uart VSidoからの返事 フィードバック
*/
ResponseFeedback::ResponseFeedback(const list<unsigned char> &uart)
:Response(uart)
,_feedbacks()
,_fields
{
	/* name  offset */
	{ 0,"rom_model_num"},
	{ 2,"rom_servo_ID"},
	{ 3,"rom_cw_agl_lmt"},
	{ 5,"rom_ccw_agl_lmt"},
	{ 7,"rom_damper"},
	{ 8,"rom_cw_cmp_margin"},
	{ 9,"rom_ccw_cmp_margin"},
	{10,"rom_cw_cmp_slope"},
	{11,"rom_ccw_cmp_slope"},
	{12,"rom_punch"},
	{13,"rom_goal_pos"},
	{15,"rom_goal_tim"},
	{17,"rom_max_torque"},
	{18,"rom_torque_mode"},
	{19,"rom_pres_pos"},
	{21,"rom_pres_time"},
	{23,"rom_pres_spd"},
	{25,"rom_pres_curr"},
	{27,"rom_pres_temp"},
	{29,"rom_pres_volt"},
	{31,"Flags"},
	{32,"alg_ofset"},
	{34,"parents_ID"},
	{35,"connected"},
	{36,"read_time"},
	{38,"_ram_goal_pos"},
	{40,"__ram_goal_pos"},
	{42,"_ram_res_pos"},
	{44,"_send_speed"},
	{45,"_send_cmd_time"},
	{46,"flg_min_max"},
	{47,"flg_goal_pos"},
	{48,"flg_parent_inv"},
	{49,"flg_goal_slope"},
	{51,"flg_check_angle"},
	{52,"port_type"},
	{53,"servo_type"}
}
{
	_jsonObj["type"] = picojson::value("feedback");
}


/** VSidoの返事をJsonに変換する
* @param None
* @return Jsonデータ
* {
* 	"type": "feedback",
* 	"feedback":[
* 	  {
* 	    "sid":1,
* 	    "dat": {}
* 	  },
* 	  {
* 	    "sid":2,
* 	    "dat": {}
* 	  }
* 	]
* }	
*/
string ResponseFeedback::conv(void)
{
	this->splite();
	
	picojson::array feedbackArray;
	for(auto &feedback :_feedbacks)
	{
		picojson::object fbObj;
		fbObj["sid"] = picojson::value((double)feedback.front());
		feedback.pop_front();

		auto dat = parseDAT(feedback);
		fbObj["dat"] = picojson::value(dat);
		
		feedbackArray.push_back(picojson::value(fbObj));
	}
	_jsonObj["feedback"] = picojson::value(feedbackArray);
	return Response::conv();
}



void ResponseFeedback::splite(void)
{
	 auto requset = dynamic_pointer_cast<ResponseFeedback>(_expect);
	this->_dad = requset->_dad;
	this->_dln = requset->_dln;
	this->_width = this->_dln;
	
	if(_iConstMaxDataLength < this->_width  + this->_dad)
	{
		this->_width = _iConstMaxDataLength - this->_dad;
	}
	
	list<unsigned char> feedback;
	int i = 0;
	for(auto ch :_uart)
	{
		feedback.push_back(ch);
		if(0 == ++i%this->_width)
		{
			_feedbacks.push_back(feedback);
			feedback.clear();
		}
	}
}


inline short Mix2Btye(unsigned char low_byte,unsigned char high_byte)
{
	short data_low = low_byte;
	short data_high = (short)((high_byte & 0x80) | (high_byte) >> 1);
	short combined_data = (short)(((data_high << 8) & 0x0000FF00) | ((data_low) & 0x000000FF));
	return  (short)((combined_data & 0x8000) | (combined_data >> 1));
}

picojson::object ResponseFeedback::parseDAT(list<unsigned char> &data)
{
	picojson::object dat;
//	fbObj["dat"] = picojson::value((double)feedback.front());

	int i = 0;
	string name("unkown");
	list<unsigned char> elementRaw;
	map<string,list<unsigned char>> datRaw;
	for(auto ch :data)
	{
		const auto field = this->_fields.find(i++);
		if(field != this->_fields.end())
		{
			// keep old one 
			datRaw[name]= elementRaw;
			
			// for next element.
			name = field->second;
			elementRaw.clear();
			elementRaw.push_back(ch);
		}
		else
		{
			elementRaw.push_back(ch);
		}
	}
	// keep last one.
	if("unkown" != name)
	{
		datRaw[name]= elementRaw;
	}
	
	short value = 0;
	//
	for (auto& kv : datRaw)
	{
		DUMP_VAR(kv.first);
		if(1 == kv.second.size())
		{
			value = (int)kv.second.front();
		}
		else if(2 == kv.second.size())
		{
			value = Mix2Btye(kv.second.front(),kv.second.back());
		}
		else
		{
			continue;
		}
		dat[kv.first] = picojson::value((double)value);
	}
	data.clear();
	return dat;
}

