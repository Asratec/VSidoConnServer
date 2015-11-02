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
#ifndef __VISIDO_VID_TABLE_HPP__
#define __VISIDO_VID_TABLE_HPP__

#include <string>
#include <map>
#include <tuple>
using namespace std;

#define VID_TABLE_ITEM_1S(x,y,min,max,val) { x,make_tuple(#y,1,min,max,false)}
#define VID_TABLE_ITEM_1U(x,y,min,max,val) { x,make_tuple(#y,1,min,max,true)}
#define VID_TABLE_ITEM_2S(x,y,min,max,val) { x,make_tuple(#y,2,min,max,false)}
#define VID_TABLE_ITEM_2U(x,y,min,max,val) { x,make_tuple(#y,2,min,max,true)}


static const map<int,tuple<string,int,int,int,bool>> vid_table =
{
	/* offset name byte min max test */
	VID_TABLE_ITEM_1U( 0,RS485_Baudrate,		0,3,2),
	VID_TABLE_ITEM_1U( 1,TTL_Baudrate,			0,3,1),
	VID_TABLE_ITEM_1U( 2,RS232_Baudrate,		0,3,0),
	VID_TABLE_ITEM_1U( 3,IO_PA_IO_Mode,			0,0x78,5),
	VID_TABLE_ITEM_1U( 4,IO_PA_Analog_Mode,		0,0x78,6),
	VID_TABLE_ITEM_1U( 5,IO_PA_PWM,				0,1,0),
	VID_TABLE_ITEM_2U( 6,IO_PA_PWM_CYCLE,		0,16383,2261),
	VID_TABLE_ITEM_1U( 8,Through_Port,			0,254,100),
	VID_TABLE_ITEM_1U( 9,Servo_Type_RS485,		0,3,3),
	VID_TABLE_ITEM_1U(10,Servo_Type_TTL,		0,3,0),
	VID_TABLE_ITEM_1U(11,IMU_Type,				0,2,1),
	VID_TABLE_ITEM_1U(12,Balancer_Flag,			0,1,0),
	VID_TABLE_ITEM_1U(13,Theta_Th,				1,100,32),
	VID_TABLE_ITEM_1U(14,Cycletime,				1,100,73),
	VID_TABLE_ITEM_1U(15,Min_Cmp,				1,250,99),
	VID_TABLE_ITEM_1U(16,Flag_Ack,				0,1,0),
	VID_TABLE_ITEM_1U(17,Volt_Th,				60,90,80),
	VID_TABLE_ITEM_1U(18,Initialize_Torque,		0,1,0),
	VID_TABLE_ITEM_1U(19,Initialize_Angle,		0,1,1),
	VID_TABLE_ITEM_1U(20,Inspection_Flag,		0,1,0),
	VID_TABLE_ITEM_1U(21,Inspection_Type,		0,1,0),
	VID_TABLE_ITEM_1U(22,Robot_Model,			0,2,2),
	VID_TABLE_ITEM_1U(23,UID_Flag,				0,1,0),
};


#define VID_LABLE_TABLE_ITEM_1S(x,y,min,max,val) { #y,make_tuple(x,1,min,max,false)}
#define VID_LABLE_TABLE_ITEM_1U(x,y,min,max,val) { #y,make_tuple(x,1,min,max,true)}
#define VID_LABLE_TABLE_ITEM_2S(x,y,min,max,val) { #y,make_tuple(x,2,min,max,false)}
#define VID_LABLE_TABLE_ITEM_2U(x,y,min,max,val) { #y,make_tuple(x,2,min,max,true)}


static const map<string,tuple<int,int,int,int,bool>> vid_label_table =
{
	/* offset name byte min max test */
	VID_LABLE_TABLE_ITEM_1U( 0,RS485_Baudrate,		0,3,2),
	VID_LABLE_TABLE_ITEM_1U( 1,TTL_Baudrate,		0,3,1),
	VID_LABLE_TABLE_ITEM_1U( 2,RS232_Baudrate,		0,3,0),
	VID_LABLE_TABLE_ITEM_1U( 3,IO_PA_IO_Mode,		4,7,5),
	VID_LABLE_TABLE_ITEM_1U( 4,IO_PA_Analog_Mode,	4,7,6),
	VID_LABLE_TABLE_ITEM_1U( 5,IO_PA_PWM,			0,1,0),
	VID_LABLE_TABLE_ITEM_2U( 6,IO_PA_PWM_CYCLE,		0,16383,2261),
	VID_LABLE_TABLE_ITEM_1U( 8,Through_Port,		0,254,100),
	VID_LABLE_TABLE_ITEM_1U( 9,Servo_Type_RS485,	0,3,3),
	VID_LABLE_TABLE_ITEM_1U(10,Servo_Type_TTL,		0,3,0),
	VID_LABLE_TABLE_ITEM_1U(11,IMU_Type,			0,2,1),
	VID_LABLE_TABLE_ITEM_1U(12,Balancer_Flag,		0,1,0),
	VID_LABLE_TABLE_ITEM_1U(13,Theta_Th,			1,100,32),
	VID_LABLE_TABLE_ITEM_1U(14,Cycletime,			1,100,73),
	VID_LABLE_TABLE_ITEM_1U(15,Min_Cmp,				1,250,99),
	VID_LABLE_TABLE_ITEM_1U(16,Flag_Ack,			0,1,0),
	VID_LABLE_TABLE_ITEM_1U(17,Volt_Th,				60,90,80),
	VID_LABLE_TABLE_ITEM_1U(18,Initialize_Torque,	0,1,0),
	VID_LABLE_TABLE_ITEM_1U(19,Initialize_Angle,	0,1,1),
	VID_LABLE_TABLE_ITEM_1U(20,Inspection_Flag,		0,1,0),
	VID_LABLE_TABLE_ITEM_1U(21,Inspection_Type,		0,1,0),
	VID_LABLE_TABLE_ITEM_1U(22,Robot_Model,			0,2,2),
	VID_LABLE_TABLE_ITEM_1U(23,UID_Flag,			0,1,0),
};


#endif //__VISIDO_VID_TABLE_HPP__

