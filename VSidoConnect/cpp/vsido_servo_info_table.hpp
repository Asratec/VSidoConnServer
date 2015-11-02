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
#ifndef __VISIDO_SERVO_INFO_TABLE_HPP__
#define __VISIDO_SERVO_INFO_TABLE_HPP__

#include <string>
#include <map>
#include <vector>
#include <tuple>
using namespace std;

#define SERVO_ITEM_1S(x,y,z) { x ,make_tuple( #y ,false)}
#define SERVO_ITEM_1U(x,y,z) { x ,make_tuple( #y ,true)}
#define SERVO_ITEM_2S(x,y,z) { x ,make_tuple( #y ,false)}
#define SERVO_ITEM_2U(x,y,z) { x ,make_tuple( #y ,true)}

static const map<int,tuple<string,bool>> servo_table =
{
	SERVO_ITEM_2S( 0,rom_model_num,			0x1234),
	SERVO_ITEM_1U( 2,rom_servo_ID,			0x1f),
	SERVO_ITEM_2S( 3,rom_cw_agl_lmt,		0x1234),
	SERVO_ITEM_2S( 5,rom_ccw_agl_lmt,		0x1234),
	SERVO_ITEM_1U( 7,rom_damper,			0x1f),
	SERVO_ITEM_1U( 8,rom_cw_cmp_margin,		0x1f),
	SERVO_ITEM_1U( 9,rom_ccw_cmp_margin,	0x1f),
	SERVO_ITEM_1U(10,rom_cw_cmp_slope,		0x1f),
	SERVO_ITEM_1U(11,rom_ccw_cmp_slope,		0x1f),
	SERVO_ITEM_1U(12,rom_punch,				0x1f),
	SERVO_ITEM_2S(13,ram_goal_pos,			0x1234),
	SERVO_ITEM_2S(15,ram_goal_tim,			0x1234),
	SERVO_ITEM_1U(17,ram_max_torque,		0x1f),
	SERVO_ITEM_1U(18,ram_torque_mode,		0x1f),
	SERVO_ITEM_2S(19,ram_pres_pos,			0x1234),
	SERVO_ITEM_2S(21,ram_pres_time,			0x1234),
	SERVO_ITEM_2S(23,ram_pres_spd,			0x1234),
	SERVO_ITEM_2S(25,ram_pres_curr,			0x1234),
	SERVO_ITEM_2S(27,ram_pres_temp,			0x1234),
	SERVO_ITEM_2S(29,ram_pres_volt,			0x1234),
	SERVO_ITEM_1U(31,Flags,					0x1f),
	SERVO_ITEM_2S(32,alg_ofset,				0x1234),
	SERVO_ITEM_1U(34,parents_ID,			0x1f),
	SERVO_ITEM_1U(35,connected,				0x1f),
	SERVO_ITEM_2S(36,read_time,				0x1234),
	SERVO_ITEM_2S(38,_ram_goal_pos,			0x1234),
	SERVO_ITEM_2S(40,__ram_goal_pos,		0x1234),
	SERVO_ITEM_2S(42,_ram_res_pos,			0x1234),
	SERVO_ITEM_1U(44,_send_speed,			0x1f),
	SERVO_ITEM_1U(45,_send_cmd_time,		0x1f),
	SERVO_ITEM_1U(46,flg_min_max,			0x1f),
	SERVO_ITEM_1U(47,flg_goal_pos,			0x1f),
	SERVO_ITEM_1U(48,flg_parent_inv,		0x1f),
	SERVO_ITEM_1U(49,flg_cmp_slope,			0x1f),
	SERVO_ITEM_1U(50,flg_check_angle,		0x1f),
	SERVO_ITEM_1U(51,port_type,				0x1f),
	SERVO_ITEM_1U(52,servo_type,			0x1f),

};


#define SERVO_ITEM_LABEL_1S(x,y,v) { #y,make_tuple(x,1,true)}
#define SERVO_ITEM_LABEL_1U(x,y,v) { #y,make_tuple(x,1,true)}

#define SERVO_ITEM_LABEL_2S(x,y,v) { #y,make_tuple(x,2,false)}
#define SERVO_ITEM_LABEL_2U(x,y,v) { #y,make_tuple(x,2,false)}


static const map<string,tuple<int,int,bool>> servo_label_table =
{
	SERVO_ITEM_LABEL_2S( 0,rom_model_num,			0x1234),
	SERVO_ITEM_LABEL_1U( 2,rom_servo_ID,			0x1f),
	SERVO_ITEM_LABEL_2S( 3,rom_cw_agl_lmt,			0x1234),
	SERVO_ITEM_LABEL_2S( 5,rom_ccw_agl_lmt,			0x1234),
	SERVO_ITEM_LABEL_1U( 7,rom_damper,				0x1f),
	SERVO_ITEM_LABEL_1U( 8,rom_cw_cmp_margin,		0x1f),
	SERVO_ITEM_LABEL_1U( 9,rom_ccw_cmp_margin,		0x1f),
	SERVO_ITEM_LABEL_1U(10,rom_cw_cmp_slope,		0x1f),
	SERVO_ITEM_LABEL_1U(11,rom_ccw_cmp_slope,		0x1f),
	SERVO_ITEM_LABEL_1U(12,rom_punch,				0x1f),
	SERVO_ITEM_LABEL_2S(13,ram_goal_pos,			0x1234),
	SERVO_ITEM_LABEL_2S(15,ram_goal_tim,			0x1234),
	SERVO_ITEM_LABEL_1U(17,ram_max_torque,			0x1f),
	SERVO_ITEM_LABEL_1U(18,ram_torque_mode,			0x1f),
	SERVO_ITEM_LABEL_2S(19,ram_pres_pos,			0x1234),
	SERVO_ITEM_LABEL_2S(21,ram_pres_time,			0x1234),
	SERVO_ITEM_LABEL_2S(23,ram_pres_spd,			0x1234),
	SERVO_ITEM_LABEL_2S(25,ram_pres_curr,			0x1234),
	SERVO_ITEM_LABEL_2S(27,ram_pres_temp,			0x1234),
	SERVO_ITEM_LABEL_2S(29,ram_pres_volt,			0x1234),
	SERVO_ITEM_LABEL_1U(31,Flags,					0x1f),
	SERVO_ITEM_LABEL_2S(32,alg_ofset,				0x1234),
	SERVO_ITEM_LABEL_1U(34,parents_ID,				0x1f),
	SERVO_ITEM_LABEL_1U(35,connected,				0x1f),
	SERVO_ITEM_LABEL_2S(36,read_time,				0x1234),
	SERVO_ITEM_LABEL_2S(38,_ram_goal_pos,			0x1234),
	SERVO_ITEM_LABEL_2S(40,__ram_goal_pos,			0x1234),
	SERVO_ITEM_LABEL_2S(42,_ram_res_pos,			0x1234),
	SERVO_ITEM_LABEL_1U(44,_send_speed,				0x1f),
	SERVO_ITEM_LABEL_1U(45,_send_cmd_time,			0x1f),
	SERVO_ITEM_LABEL_1U(46,flg_min_max,				0x1f),
	SERVO_ITEM_LABEL_1U(47,flg_goal_pos,			0x1f),
	SERVO_ITEM_LABEL_1U(48,flg_parent_inv,			0x1f),
	SERVO_ITEM_LABEL_1U(49,flg_cmp_slope,			0x1f),
	SERVO_ITEM_LABEL_1U(50,flg_check_angle,			0x1f),
	SERVO_ITEM_LABEL_1U(51,port_type,				0x1f),
	SERVO_ITEM_LABEL_1U(52,servo_type,				0x1f),
};


static const vector<string> angle_type_list =
{
	"rom_cw_agl_lmt",
	"rom_ccw_agl_lmt",
	"ram_goal_pos",
	"ram_pres_pos",
	"alg_ofset",
	"_ram_goal_pos",
	"__ram_goal_pos",
	"_ram_res_pos"
};


#endif //__VISIDO_SERVO_INFO_TABLE_HPP__
	
