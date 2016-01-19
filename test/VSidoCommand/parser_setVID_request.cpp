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
#include "VSido.hpp"
using namespace VSido;


#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test.hpp>


#include "parser_dump.hpp"




BOOST_AUTO_TEST_CASE(test_case_SetVIDRequest_0)
{
	
	/// uart data
	list<unsigned char> expected = 
	{
		0,
		2,
	};
	GEN_COMMON_SEND_UART('s');
	dumpTestData(expected);

	SetVIDRequest req;
	req.value(0,2);
	auto ack = req.execBK();
	dumpTestData(sendBuffer);
	BOOST_CHECK(expected == sendBuffer);
	
}

BOOST_AUTO_TEST_CASE(test_case_SetVIDRequest_1)
{
	
	/// uart data
	list<unsigned char> expected = 
	{
		6,
		_HBYTE(2334),7,_LBYTE(2334),
	};
	GEN_COMMON_SEND_UART('s');
	dumpTestData(expected);

	SetVIDRequest req;
	req.value2B(6,2334);
	auto ack = req.execBK();
	dumpTestData(sendBuffer);
	BOOST_CHECK(expected == sendBuffer);
	
}

#define VID_DATA_1B(x,y,z,min,max,val) (x),(val)
#define VID_DATA_2B(x,y,z,min,max,val) (x),_HBYTE(val),(x+1),_LBYTE(val)

#define VID_ADD_DATA_1B(x,y,z,min,max,val) req.value(x,val)
#define VID_ADD_DATA_2B(x,y,z,min,max,val) req.value2B(x,val)


BOOST_AUTO_TEST_CASE(test_case_SetVIDRequest_2)
{
	
	/// uart data
	list<unsigned char> expected = 
	{
		VID_DATA_1B( 0,VID_RS485_Baudrate,		1,0,3,2),
		VID_DATA_1B( 1,VID_TTL_Baudrate,			1,0,3,1),
		VID_DATA_1B( 2,VID_RS232_Baudrate,		1,0,3,0),
		VID_DATA_1B( 3,VID_IO_PA_IO_Mode,			1,4,7,5),
		VID_DATA_1B( 4,VID_IO_PA_Analog_Mode,		1,4,7,6),
		VID_DATA_1B( 5,VID_IO_PA_PWM,				1,0,1,0),
//		VID_DATA_2B( 6,VID_IO_PA_PWM_CYCLE,		2,0,16383,2261),
		VID_DATA_1B( 8,VID_Through_Port,			1,0,254,100),
		VID_DATA_1B( 9,VID_Servo_Type_RS485,		1,0,3,3),
		VID_DATA_1B(10,VID_Servo_Type_TTL,		1,0,3,0),
		VID_DATA_1B(11,VID_IMU_Type,				1,0,2,1),
		VID_DATA_1B(12,VID_Balancer_Flag,			1,0,1,0),
		VID_DATA_1B(13,VID_Theta_Th,				1,1,100,32),
		VID_DATA_1B(14,VID_Cycletime,				1,1,100,73),
		VID_DATA_1B(15,VID_Min_Cmp,				1,1,250,99),
		VID_DATA_1B(16,VID_Flag_Ack,				1,0,1,0),
		VID_DATA_1B(17,VID_Volt_Th,				1,60,90,80),
		VID_DATA_1B(18,VID_Initialize_Torque,		1,0,1,0),
		VID_DATA_1B(19,VID_Initialize_Angle,		1,0,1,1),
		VID_DATA_1B(20,VID_Inspection_Flag,		1,0,1,0),
		VID_DATA_1B(21,VID_Inspection_Type,		1,0,1,0),
		VID_DATA_1B(22,VID_Robot_Model,			1,0,2,2),
		VID_DATA_1B(23,VID_UID_Flag,				1,0,1,0),
		VID_DATA_2B( 6,VID_IO_PA_PWM_CYCLE,		2,0,16383,2261),
	};
	GEN_COMMON_SEND_UART('s');
	dumpTestData(expected);

	SetVIDRequest req;
//	req.value2B(6,2334);

	VID_ADD_DATA_1B( 0,VID_RS485_Baudrate,		1,0,3,2);
	VID_ADD_DATA_1B( 1,VID_TTL_Baudrate,		1,0,3,1);
	VID_ADD_DATA_1B( 2,VID_RS232_Baudrate,		1,0,3,0);
	VID_ADD_DATA_1B( 3,VID_IO_PA_IO_Mode,		1,4,7,5);
	VID_ADD_DATA_1B( 4,VID_IO_PA_Analog_Mode,	1,4,7,6);
	VID_ADD_DATA_1B( 5,VID_IO_PA_PWM,			1,0,1,0);
	VID_ADD_DATA_2B( 6,VID_IO_PA_PWM_CYCLE,		2,0,16383,2261);
	VID_ADD_DATA_1B( 8,VID_Through_Port,		1,0,254,100);
	VID_ADD_DATA_1B( 9,VID_Servo_Type_RS485,	1,0,3,3);
	VID_ADD_DATA_1B(10,VID_Servo_Type_TTL,		1,0,3,0);
	VID_ADD_DATA_1B(11,VID_IMU_Type,			1,0,2,1);
	VID_ADD_DATA_1B(12,VID_Balancer_Flag,		1,0,1,0);
	VID_ADD_DATA_1B(13,VID_Theta_Th,			1,1,100,32);
	VID_ADD_DATA_1B(14,VID_Cycletime,			1,1,100,73);
	VID_ADD_DATA_1B(15,VID_Min_Cmp,				1,1,250,99);
	VID_ADD_DATA_1B(16,VID_Flag_Ack,			1,0,1,0);
	VID_ADD_DATA_1B(17,VID_Volt_Th,				1,60,90,80);
	VID_ADD_DATA_1B(18,VID_Initialize_Torque,	1,0,1,0);
	VID_ADD_DATA_1B(19,VID_Initialize_Angle,	1,0,1,1);
	VID_ADD_DATA_1B(20,VID_Inspection_Flag,		1,0,1,0);
	VID_ADD_DATA_1B(21,VID_Inspection_Type,		1,0,1,0);
	VID_ADD_DATA_1B(22,VID_Robot_Model,			1,0,2,2);
	VID_ADD_DATA_1B(23,VID_UID_Flag,			1,0,1,0);



	
	auto ack = req.execBK();
	dumpTestData(sendBuffer);
	BOOST_CHECK(expected == sendBuffer);
	
}


BOOST_AUTO_TEST_CASE(test_case_SetVIDRequest_3)
{
	
	/// uart data
	list<unsigned char> expected = 
	{
	};
	GEN_COMMON_SEND_UART('s');
	dumpTestData(expected);

	SetVIDRequest req;
	req.value(40,2);
	auto ack = req.execBK();
	dumpTestData(sendBuffer);
	BOOST_CHECK(expected == sendBuffer);
	
}


