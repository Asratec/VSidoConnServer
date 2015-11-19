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





#define VID_DATA_1B(x,y,z,min,max,val) (val)
#define VID_DATA_2B(x,y,z,min,max,val) _HBYTE(val),_LBYTE(val)


#define VID_ADD_DATA_1B(x,y,z,min,max,val) req.vid(x)
#define VID_ADD_DATA_2B VID_ADD_DATA_1B


#define VID_CHECK_DATA_1B(x,y,z,min,max,val)\
{\
	if(#y==name)\
	{\
		BOOST_CHECK_EQUAL(value,val);\
	}\
}

#define VID_CHECK_DATA_2B VID_CHECK_DATA_1B



BOOST_AUTO_TEST_CASE(test_case_GetVIDResponse_0)
{
	
	/// uart data
	list<unsigned char> recieved = 
	{
		VID_DATA_1B( 0,RS485_Baudrate,		1,0,3,2),
		VID_DATA_1B( 1,TTL_Baudrate,		1,0,3,1),
		VID_DATA_1B( 2,RS232_Baudrate,		1,0,3,0),
		VID_DATA_1B( 3,IO_PA_IO_Mode,		1,4,7,5),
		VID_DATA_1B( 4,IO_PA_Analog_Mode,	1,4,7,6),
		VID_DATA_1B( 5,IO_PA_PWM,			1,0,1,0),
		VID_DATA_2B( 6,IO_PA_PWM_CYCLE,		2,0,16383,2261),
		VID_DATA_1B( 8,Through_Port,		1,0,254,100),
		VID_DATA_1B( 9,Servo_Type_RS485,	1,0,3,3),
		VID_DATA_1B(10,Servo_Type_TTL,		1,0,3,0),
		VID_DATA_1B(11,IMU_Type,			1,0,2,1),
		VID_DATA_1B(12,Balancer_Flag,		1,0,1,0),
		VID_DATA_1B(13,Theta_Th,			1,1,100,32),
		VID_DATA_1B(14,Cycletime,			1,1,100,73),
		VID_DATA_1B(15,Min_Cmp,				1,1,250,99),
		VID_DATA_1B(16,Flag_Ack,			1,0,1,0),
		VID_DATA_1B(17,Volt_Th,				1,60,90,80),
		VID_DATA_1B(18,Initialize_Torque,	1,0,1,0),
		VID_DATA_1B(19,Initialize_Angle,	1,0,1,1),
		VID_DATA_1B(20,Inspection_Flag,		1,0,1,0),
		VID_DATA_1B(21,Inspection_Type,		1,0,1,0),
		VID_DATA_1B(22,Robot_Model,			1,0,2,2),
		VID_DATA_1B(23,UID_Flag,			1,0,1,0),
	};
	GEN_COMMON_READ_UART('g');
	DUMMY_ACK(recieved);

	GetVIDRequest req;

	VID_ADD_DATA_1B( 0,RS485_Baudrate,		1,0,3,2);
	VID_ADD_DATA_1B( 1,TTL_Baudrate,		1,0,3,1);
	VID_ADD_DATA_1B( 2,RS232_Baudrate,		1,0,3,0);
	VID_ADD_DATA_1B( 3,IO_PA_IO_Mode,		1,4,7,5);
	VID_ADD_DATA_1B( 4,IO_PA_Analog_Mode,	1,4,7,6);
	VID_ADD_DATA_1B( 5,IO_PA_PWM,			1,0,1,0);
	VID_ADD_DATA_2B( 6,IO_PA_PWM_CYCLE,		2,0,16383,2261);
	VID_ADD_DATA_1B( 8,Through_Port,		1,0,254,100);
	VID_ADD_DATA_1B( 9,Servo_Type_RS485,	1,0,3,3);
	VID_ADD_DATA_1B(10,Servo_Type_TTL,		1,0,3,0);
	VID_ADD_DATA_1B(11,IMU_Type,			1,0,2,1);
	VID_ADD_DATA_1B(12,Balancer_Flag,		1,0,1,0);
	VID_ADD_DATA_1B(13,Theta_Th,			1,1,100,32);
	VID_ADD_DATA_1B(14,Cycletime,			1,1,100,73);
	VID_ADD_DATA_1B(15,Min_Cmp,				1,1,250,99);
	VID_ADD_DATA_1B(16,Flag_Ack,			1,0,1,0);
	VID_ADD_DATA_1B(17,Volt_Th,				1,60,90,80);
	VID_ADD_DATA_1B(18,Initialize_Torque,	1,0,1,0);
	VID_ADD_DATA_1B(19,Initialize_Angle,	1,0,1,1);
	VID_ADD_DATA_1B(20,Inspection_Flag,		1,0,1,0);
	VID_ADD_DATA_1B(21,Inspection_Type,		1,0,1,0);
	VID_ADD_DATA_1B(22,Robot_Model,			1,0,2,2);
	VID_ADD_DATA_1B(23,UID_Flag,			1,0,1,0);



	auto ack = req.execBK();
	
	auto vars = ack.vars();
	DUMP_VAR(vars.size());
	BOOST_CHECK_EQUAL(vars.size(),23);
	
	for(auto const & var:vars)
	{
		auto name = var.first;
		auto value = var.second;
	
		DUMP_VAR(name);
		DUMP_VAR(value);

		VID_CHECK_DATA_1B( 0,RS485_Baudrate,	1,0,3,2);
		VID_CHECK_DATA_1B( 1,TTL_Baudrate,		1,0,3,1);
		VID_CHECK_DATA_1B( 2,RS232_Baudrate,	1,0,3,0);
		VID_CHECK_DATA_1B( 3,IO_PA_IO_Mode,		1,4,7,5);
		VID_CHECK_DATA_1B( 4,IO_PA_Analog_Mode,	1,4,7,6);
		VID_CHECK_DATA_1B( 5,IO_PA_PWM,			1,0,1,0);
		VID_CHECK_DATA_2B( 6,IO_PA_PWM_CYCLE,	2,0,16383,2261);
		VID_CHECK_DATA_1B( 8,Through_Port,		1,0,254,100);
		VID_CHECK_DATA_1B( 9,Servo_Type_RS485,	1,0,3,3);
		VID_CHECK_DATA_1B(10,Servo_Type_TTL,	1,0,3,0);
		VID_CHECK_DATA_1B(11,IMU_Type,			1,0,2,1);
		VID_CHECK_DATA_1B(12,Balancer_Flag,		1,0,1,0);
		VID_CHECK_DATA_1B(13,Theta_Th,			1,1,100,32);
		VID_CHECK_DATA_1B(14,Cycletime,			1,1,100,73);
		VID_CHECK_DATA_1B(15,Min_Cmp,			1,1,250,99);
		VID_CHECK_DATA_1B(16,Flag_Ack,			1,0,1,0);
		VID_CHECK_DATA_1B(17,Volt_Th,			1,60,90,80);
		VID_CHECK_DATA_1B(18,Initialize_Torque,	1,0,1,0);
		VID_CHECK_DATA_1B(19,Initialize_Angle,	1,0,1,1);
		VID_CHECK_DATA_1B(20,Inspection_Flag,	1,0,1,0);
		VID_CHECK_DATA_1B(21,Inspection_Type,	1,0,1,0);
		VID_CHECK_DATA_1B(22,Robot_Model,		1,0,2,2);
		VID_CHECK_DATA_1B(23,UID_Flag,			1,0,1,0);
	}
}



BOOST_AUTO_TEST_CASE(test_case_GetVIDResponse_1)
{
	
	/// uart data
	list<unsigned char> recieved = 
	{
		VID_DATA_1B( 0,RS485_Baudrate,		1,0,3,2),
		VID_DATA_1B( 1,TTL_Baudrate,		1,0,3,1),
		VID_DATA_1B( 2,RS232_Baudrate,		1,0,3,0),
		VID_DATA_1B( 3,IO_PA_IO_Mode,		1,4,7,5),
		VID_DATA_1B( 4,IO_PA_Analog_Mode,	1,4,7,6),
		VID_DATA_1B( 5,IO_PA_PWM,			1,0,1,0),
		VID_DATA_2B( 6,IO_PA_PWM_CYCLE,		2,0,16383,2261),
		VID_DATA_1B( 8,Through_Port,		1,0,254,100),
		VID_DATA_1B( 9,Servo_Type_RS485,	1,0,3,3),
		VID_DATA_1B(10,Servo_Type_TTL,		1,0,3,0),
		VID_DATA_1B(11,IMU_Type,			1,0,2,1),
		VID_DATA_1B(12,Balancer_Flag,		1,0,1,0),
		VID_DATA_1B(13,Theta_Th,			1,1,100,32),
		VID_DATA_1B(14,Cycletime,			1,1,100,73),
		VID_DATA_1B(15,Min_Cmp,				1,1,250,99),
		VID_DATA_1B(16,Flag_Ack,			1,0,1,0),
		VID_DATA_1B(17,Volt_Th,				1,60,90,80),
		VID_DATA_1B(18,Initialize_Torque,	1,0,1,0),
		VID_DATA_1B(19,Initialize_Angle,	1,0,1,1),
		VID_DATA_1B(20,Inspection_Flag,		1,0,1,0),
		VID_DATA_1B(21,Inspection_Type,		1,0,1,0),
		VID_DATA_1B(22,Robot_Model,			1,0,2,2),
		VID_DATA_1B(23,UID_Flag,			1,0,1,0),
	};
	GEN_COMMON_READ_UART('g');
	DUMMY_ACK(recieved);

	GetVIDRequest req;

	VID_ADD_DATA_1B( 0,RS485_Baudrate,		1,0,3,2);
	VID_ADD_DATA_1B( 1,TTL_Baudrate,		1,0,3,1);
	VID_ADD_DATA_1B( 2,RS232_Baudrate,		1,0,3,0);
	VID_ADD_DATA_1B( 3,IO_PA_IO_Mode,		1,4,7,5);
	VID_ADD_DATA_1B( 4,IO_PA_Analog_Mode,	1,4,7,6);
	VID_ADD_DATA_1B( 5,IO_PA_PWM,			1,0,1,0);
	VID_ADD_DATA_2B( 6,IO_PA_PWM_CYCLE,		2,0,16383,2261);
	VID_ADD_DATA_1B( 8,Through_Port,		1,0,254,100);
	VID_ADD_DATA_1B( 9,Servo_Type_RS485,	1,0,3,3);
	VID_ADD_DATA_1B(10,Servo_Type_TTL,		1,0,3,0);
	VID_ADD_DATA_1B(11,IMU_Type,			1,0,2,1);
	VID_ADD_DATA_1B(12,Balancer_Flag,		1,0,1,0);
	VID_ADD_DATA_1B(13,Theta_Th,			1,1,100,32);
	VID_ADD_DATA_1B(14,Cycletime,			1,1,100,73);
	VID_ADD_DATA_1B(15,Min_Cmp,				1,1,250,99);
	VID_ADD_DATA_1B(16,Flag_Ack,			1,0,1,0);
	VID_ADD_DATA_1B(17,Volt_Th,				1,60,90,80);
	VID_ADD_DATA_1B(18,Initialize_Torque,	1,0,1,0);
	VID_ADD_DATA_1B(19,Initialize_Angle,	1,0,1,1);
	VID_ADD_DATA_1B(20,Inspection_Flag,		1,0,1,0);
	VID_ADD_DATA_1B(21,Inspection_Type,		1,0,1,0);
	VID_ADD_DATA_1B(22,Robot_Model,			1,0,2,2);
	VID_ADD_DATA_1B(23,UID_Flag,			1,0,1,0);



	auto ack = req.execBK();
	
	BOOST_CHECK_EQUAL("" ,ack.JSON());
}



BOOST_AUTO_TEST_CASE(test_case_GetVIDResponse_2)
{
	
	/// uart data
	list<unsigned char> recieved = 
	{
	};
	GEN_COMMON_READ_UART('G');
	DUMMY_ACK(recieved);

	GetVIDRequest req;

	auto ack = req.execBK();
	
	BOOST_CHECK_EQUAL("" ,ack.JSON());
}



BOOST_AUTO_TEST_CASE(test_case_GetVIDResponse_3)
{
	
	/// uart data
	list<unsigned char> recieved = 
	{
		VID_DATA_1B( 0,RS485_Baudrate,		1,0,3,2),
		VID_DATA_1B( 1,TTL_Baudrate,		1,0,3,1),
		VID_DATA_1B( 2,RS232_Baudrate,		1,0,3,0),
		VID_DATA_1B( 3,IO_PA_IO_Mode,		1,4,7,5),
		VID_DATA_1B( 4,IO_PA_Analog_Mode,	1,4,7,6),
		VID_DATA_1B( 5,IO_PA_PWM,			1,0,1,0),
		VID_DATA_2B( 6,IO_PA_PWM_CYCLE,		2,0,16383,2261),
		VID_DATA_1B( 8,Through_Port,		1,0,254,100),
		VID_DATA_1B( 9,Servo_Type_RS485,	1,0,3,3),
		VID_DATA_1B(10,Servo_Type_TTL,		1,0,3,0),
		VID_DATA_1B(11,IMU_Type,			1,0,2,1),
		VID_DATA_1B(12,Balancer_Flag,		1,0,1,0),
		VID_DATA_1B(13,Theta_Th,			1,1,100,32),
		VID_DATA_1B(14,Cycletime,			1,1,100,73),
		VID_DATA_1B(15,Min_Cmp,				1,1,250,99),
		VID_DATA_1B(16,Flag_Ack,			1,0,1,0),
		VID_DATA_1B(17,Volt_Th,				1,60,90,80),
		VID_DATA_1B(18,Initialize_Torque,	1,0,1,0),
		VID_DATA_1B(19,Initialize_Angle,	1,0,1,1),
		VID_DATA_1B(20,Inspection_Flag,		1,0,1,0),
		VID_DATA_1B(21,Inspection_Type,		1,0,1,0),
		VID_DATA_1B(22,Robot_Model,			1,0,2,2),
		VID_DATA_1B(23,UID_Flag,			1,0,1,0),
		VID_DATA_1B(24,UID_Flag,			1,0,1,0),
	};
	GEN_COMMON_READ_UART('g');
	DUMMY_ACK(recieved);
	dumpTestData(recieved);
	

	GetVIDRequest req;

	VID_ADD_DATA_1B( 0,RS485_Baudrate,		1,0,3,2);
	VID_ADD_DATA_1B( 1,TTL_Baudrate,		1,0,3,1);
	VID_ADD_DATA_1B( 2,RS232_Baudrate,		1,0,3,0);
	VID_ADD_DATA_1B( 3,IO_PA_IO_Mode,		1,4,7,5);
	VID_ADD_DATA_1B( 4,IO_PA_Analog_Mode,	1,4,7,6);
	VID_ADD_DATA_1B( 5,IO_PA_PWM,			1,0,1,0);
	VID_ADD_DATA_2B( 6,IO_PA_PWM_CYCLE,		2,0,16383,2261);
	VID_ADD_DATA_1B( 8,Through_Port,		1,0,254,100);
	VID_ADD_DATA_1B( 9,Servo_Type_RS485,	1,0,3,3);
	VID_ADD_DATA_1B(10,Servo_Type_TTL,		1,0,3,0);
	VID_ADD_DATA_1B(11,IMU_Type,			1,0,2,1);
	VID_ADD_DATA_1B(12,Balancer_Flag,		1,0,1,0);
	VID_ADD_DATA_1B(13,Theta_Th,			1,1,100,32);
	VID_ADD_DATA_1B(14,Cycletime,			1,1,100,73);
	VID_ADD_DATA_1B(15,Min_Cmp,				1,1,250,99);
	VID_ADD_DATA_1B(16,Flag_Ack,			1,0,1,0);
	VID_ADD_DATA_1B(17,Volt_Th,				1,60,90,80);
	VID_ADD_DATA_1B(18,Initialize_Torque,	1,0,1,0);
	VID_ADD_DATA_1B(19,Initialize_Angle,	1,0,1,1);
	VID_ADD_DATA_1B(20,Inspection_Flag,		1,0,1,0);
	VID_ADD_DATA_1B(21,Inspection_Type,		1,0,1,0);
	VID_ADD_DATA_1B(22,Robot_Model,			1,0,2,2);
	VID_ADD_DATA_1B(23,UID_Flag,			1,0,1,0);
	VID_ADD_DATA_1B(24,UID_Flag,			1,0,1,0);


	auto ack = req.execBK();
	
	auto vars = ack.vars();
	DUMP_VAR(vars.size());
	BOOST_CHECK_EQUAL(vars.size(),23);
	
	for(auto const & var:vars)
	{
		auto name = var.first;
		auto value = var.second;
	
		DUMP_VAR(name);
		DUMP_VAR(value);

		VID_CHECK_DATA_1B( 0,RS485_Baudrate,	1,0,3,2);
		VID_CHECK_DATA_1B( 1,TTL_Baudrate,		1,0,3,1);
		VID_CHECK_DATA_1B( 2,RS232_Baudrate,	1,0,3,0);
		VID_CHECK_DATA_1B( 3,IO_PA_IO_Mode,		1,4,7,5);
		VID_CHECK_DATA_1B( 4,IO_PA_Analog_Mode,	1,4,7,6);
		VID_CHECK_DATA_1B( 5,IO_PA_PWM,			1,0,1,0);
		VID_CHECK_DATA_2B( 6,IO_PA_PWM_CYCLE,	2,0,16383,2261);
		VID_CHECK_DATA_1B( 8,Through_Port,		1,0,254,100);
		VID_CHECK_DATA_1B( 9,Servo_Type_RS485,	1,0,3,3);
		VID_CHECK_DATA_1B(10,Servo_Type_TTL,	1,0,3,0);
		VID_CHECK_DATA_1B(11,IMU_Type,			1,0,2,1);
		VID_CHECK_DATA_1B(12,Balancer_Flag,		1,0,1,0);
		VID_CHECK_DATA_1B(13,Theta_Th,			1,1,100,32);
		VID_CHECK_DATA_1B(14,Cycletime,			1,1,100,73);
		VID_CHECK_DATA_1B(15,Min_Cmp,			1,1,250,99);
		VID_CHECK_DATA_1B(16,Flag_Ack,			1,0,1,0);
		VID_CHECK_DATA_1B(17,Volt_Th,			1,60,90,80);
		VID_CHECK_DATA_1B(18,Initialize_Torque,	1,0,1,0);
		VID_CHECK_DATA_1B(19,Initialize_Angle,	1,0,1,1);
		VID_CHECK_DATA_1B(20,Inspection_Flag,	1,0,1,0);
		VID_CHECK_DATA_1B(21,Inspection_Type,	1,0,1,0);
		VID_CHECK_DATA_1B(22,Robot_Model,		1,0,2,2);
		VID_CHECK_DATA_1B(23,UID_Flag,			1,0,1,0);
	}
}


BOOST_AUTO_TEST_CASE(test_case_GetVIDResponse_4)
{
	
	/// uart data
	list<unsigned char> recieved = 
	{
		VID_DATA_1B( 0,RS485_Baudrate,		1,0,3,2),
		VID_DATA_1B( 1,TTL_Baudrate,		1,0,3,1),
		VID_DATA_1B( 2,RS232_Baudrate,		1,0,3,0),
		VID_DATA_1B( 3,IO_PA_IO_Mode,		1,4,7,5),
		VID_DATA_1B( 4,IO_PA_Analog_Mode,	1,4,7,6),
		VID_DATA_1B( 5,IO_PA_PWM,			1,0,1,0),
		VID_DATA_2B( 6,IO_PA_PWM_CYCLE,		2,0,16383,2261),
		VID_DATA_1B( 8,Through_Port,		1,0,254,100),
		VID_DATA_1B( 9,Servo_Type_RS485,	1,0,3,3),
		VID_DATA_1B(10,Servo_Type_TTL,		1,0,3,0),
		VID_DATA_1B(11,IMU_Type,			1,0,2,1),
		VID_DATA_1B(12,Balancer_Flag,		1,0,1,0),
		VID_DATA_1B(13,Theta_Th,			1,1,100,32),
		VID_DATA_1B(14,Cycletime,			1,1,100,73),
		VID_DATA_1B(15,Min_Cmp,				1,1,250,99),
		VID_DATA_1B(16,Flag_Ack,			1,0,1,0),
		VID_DATA_1B(17,Volt_Th,				1,60,90,80),
		VID_DATA_1B(18,Initialize_Torque,	1,0,1,0),
		VID_DATA_1B(19,Initialize_Angle,	1,0,1,1),
		VID_DATA_1B(20,Inspection_Flag,		1,0,1,0),
		VID_DATA_1B(21,Inspection_Type,		1,0,1,0),
		VID_DATA_1B(22,Robot_Model,			1,0,2,2),
		VID_DATA_1B(23,UID_Flag,			1,0,1,0),
	};
	GEN_COMMON_READ_UART('g');
	DUMMY_ACK(recieved);

	GetVIDRequest req;

	VID_ADD_DATA_1B( 0,RS485_Baudrate,		1,0,3,2);
	VID_ADD_DATA_1B( 1,TTL_Baudrate,		1,0,3,1);
	VID_ADD_DATA_1B( 2,RS232_Baudrate,		1,0,3,0);
	VID_ADD_DATA_1B( 3,IO_PA_IO_Mode,		1,4,7,5);
	VID_ADD_DATA_1B( 4,IO_PA_Analog_Mode,	1,4,7,6);
	VID_ADD_DATA_1B( 5,IO_PA_PWM,			1,0,1,0);
	VID_ADD_DATA_2B( 6,IO_PA_PWM_CYCLE,		2,0,16383,2261);
	VID_ADD_DATA_1B( 8,Through_Port,		1,0,254,100);
	VID_ADD_DATA_1B( 9,Servo_Type_RS485,	1,0,3,3);
	VID_ADD_DATA_1B(10,Servo_Type_TTL,		1,0,3,0);
	VID_ADD_DATA_1B(11,IMU_Type,			1,0,2,1);
	VID_ADD_DATA_1B(12,Balancer_Flag,		1,0,1,0);
	VID_ADD_DATA_1B(13,Theta_Th,			1,1,100,32);
	VID_ADD_DATA_1B(14,Cycletime,			1,1,100,73);
	VID_ADD_DATA_1B(15,Min_Cmp,				1,1,250,99);
	VID_ADD_DATA_1B(16,Flag_Ack,			1,0,1,0);
	VID_ADD_DATA_1B(17,Volt_Th,				1,60,90,80);
	VID_ADD_DATA_1B(18,Initialize_Torque,	1,0,1,0);
	VID_ADD_DATA_1B(19,Initialize_Angle,	1,0,1,1);
	VID_ADD_DATA_1B(20,Inspection_Flag,		1,0,1,0);
	VID_ADD_DATA_1B(21,Inspection_Type,		1,0,1,0);
	VID_ADD_DATA_1B(22,Robot_Model,			1,0,2,2);
	VID_ADD_DATA_1B(23,UID_Flag,			1,0,1,0);



	auto ack = req.execBK();
	
	
	ack.forEachVar([](const string &name ,int value){
		VID_CHECK_DATA_1B( 0,RS485_Baudrate,	1,0,3,2);
		VID_CHECK_DATA_1B( 1,TTL_Baudrate,		1,0,3,1);
		VID_CHECK_DATA_1B( 2,RS232_Baudrate,	1,0,3,0);
		VID_CHECK_DATA_1B( 3,IO_PA_IO_Mode,		1,4,7,5);
		VID_CHECK_DATA_1B( 4,IO_PA_Analog_Mode,	1,4,7,6);
		VID_CHECK_DATA_1B( 5,IO_PA_PWM,			1,0,1,0);
		VID_CHECK_DATA_2B( 6,IO_PA_PWM_CYCLE,	2,0,16383,2261);
		VID_CHECK_DATA_1B( 8,Through_Port,		1,0,254,100);
		VID_CHECK_DATA_1B( 9,Servo_Type_RS485,	1,0,3,3);
		VID_CHECK_DATA_1B(10,Servo_Type_TTL,	1,0,3,0);
		VID_CHECK_DATA_1B(11,IMU_Type,			1,0,2,1);
		VID_CHECK_DATA_1B(12,Balancer_Flag,		1,0,1,0);
		VID_CHECK_DATA_1B(13,Theta_Th,			1,1,100,32);
		VID_CHECK_DATA_1B(14,Cycletime,			1,1,100,73);
		VID_CHECK_DATA_1B(15,Min_Cmp,			1,1,250,99);
		VID_CHECK_DATA_1B(16,Flag_Ack,			1,0,1,0);
		VID_CHECK_DATA_1B(17,Volt_Th,			1,60,90,80);
		VID_CHECK_DATA_1B(18,Initialize_Torque,	1,0,1,0);
		VID_CHECK_DATA_1B(19,Initialize_Angle,	1,0,1,1);
		VID_CHECK_DATA_1B(20,Inspection_Flag,	1,0,1,0);
		VID_CHECK_DATA_1B(21,Inspection_Type,	1,0,1,0);
		VID_CHECK_DATA_1B(22,Robot_Model,		1,0,2,2);
		VID_CHECK_DATA_1B(23,UID_Flag,			1,0,1,0);
	});
	
}


BOOST_AUTO_TEST_CASE(test_case_GetVIDResponse_5)
{
	

	GetVIDRequest req;

	VID_ADD_DATA_1B( 0,RS485_Baudrate,		1,0,3,2);
	VID_ADD_DATA_1B( 1,TTL_Baudrate,		1,0,3,2);
	VID_ADD_DATA_1B( 2,RS232_Baudrate,		1,0,3,2);
	VID_ADD_DATA_1B( 3,IO_PA_IO_Mode,		1,4,7,0);
	VID_ADD_DATA_1B( 4,IO_PA_Analog_Mode,	1,4,7,0);
	VID_ADD_DATA_1B( 5,IO_PA_PWM,			1,0,1,1);
	VID_ADD_DATA_2B( 6,IO_PA_PWM_CYCLE,		2,0,16383,12548);
	VID_ADD_DATA_1B( 8,Through_Port,		1,0,254,0);
	VID_ADD_DATA_1B( 9,Servo_Type_RS485,	1,0,3,1);
	VID_ADD_DATA_1B(10,Servo_Type_TTL,		1,0,3,0);
	VID_ADD_DATA_1B(11,IMU_Type,			1,0,2,3);
	VID_ADD_DATA_1B(12,Balancer_Flag,		1,0,1,0);
	VID_ADD_DATA_1B(13,Theta_Th,			1,1,100,0xa);
	VID_ADD_DATA_1B(14,Cycletime,			1,1,100,1);
	VID_ADD_DATA_1B(15,Min_Cmp,				1,1,250,0xa);
	VID_ADD_DATA_1B(16,Flag_Ack,			1,0,1,1);
	VID_ADD_DATA_1B(17,Volt_Th,				1,60,90,0);
	VID_ADD_DATA_1B(18,Initialize_Torque,	1,0,1,1);
	VID_ADD_DATA_1B(19,Initialize_Angle,	1,0,1,1);
	VID_ADD_DATA_1B(20,Inspection_Flag,		1,0,1,1);
	VID_ADD_DATA_1B(21,Inspection_Type,		1,0,1,0);
	VID_ADD_DATA_1B(22,Robot_Model,			1,0,2,0xe);
	VID_ADD_DATA_1B(23,UID_Flag,			1,0,1,0x1f);


	/// uart data
	list<unsigned char> recieved = 
	{
		0xff,0x67 ,0x1c ,0x02 ,0x02 ,0x02 ,0x00 ,0x00,
		0x01 ,0x09 ,0xc4 ,0x00 ,0x01 ,0x00 ,0x03 ,0x00,
		0x0a ,0x01 ,0x0a ,0x01 ,0x00 ,0x01 ,0x01 ,0x01,
		0x00 ,0x0e ,0x1f ,0x58
	};
	DUMMY_ACK(recieved);

	auto ack = req.execBK();
	
	
	ack.forEachVar([](const string &name ,int value){
		VID_CHECK_DATA_1B( 0,RS485_Baudrate,		1,0,3,2);
		VID_CHECK_DATA_1B( 1,TTL_Baudrate,		1,0,3,2);
		VID_CHECK_DATA_1B( 2,RS232_Baudrate,		1,0,3,2);
		VID_CHECK_DATA_1B( 3,IO_PA_IO_Mode,		1,4,7,0);
		VID_CHECK_DATA_1B( 4,IO_PA_Analog_Mode,	1,4,7,0);
		VID_CHECK_DATA_1B( 5,IO_PA_PWM,			1,0,1,1);
		VID_CHECK_DATA_2B( 6,IO_PA_PWM_CYCLE,		2,0,16383,12548);
		VID_CHECK_DATA_1B( 8,Through_Port,		1,0,254,0);
		VID_CHECK_DATA_1B( 9,Servo_Type_RS485,	1,0,3,1);
		VID_CHECK_DATA_1B(10,Servo_Type_TTL,		1,0,3,0);
		VID_CHECK_DATA_1B(11,IMU_Type,			1,0,2,3);
		VID_CHECK_DATA_1B(12,Balancer_Flag,		1,0,1,0);
		VID_CHECK_DATA_1B(13,Theta_Th,			1,1,100,0xa);
		VID_CHECK_DATA_1B(14,Cycletime,			1,1,100,1);
		VID_CHECK_DATA_1B(15,Min_Cmp,				1,1,250,0xa);
		VID_CHECK_DATA_1B(16,Flag_Ack,			1,0,1,1);
		VID_CHECK_DATA_1B(17,Volt_Th,				1,60,90,0);
		VID_CHECK_DATA_1B(18,Initialize_Torque,	1,0,1,1);
		VID_CHECK_DATA_1B(19,Initialize_Angle,	1,0,1,1);
		VID_CHECK_DATA_1B(20,Inspection_Flag,		1,0,1,1);
		VID_CHECK_DATA_1B(21,Inspection_Type,		1,0,1,0);
		VID_CHECK_DATA_1B(22,Robot_Model,			1,0,2,0xe);
		VID_CHECK_DATA_1B(23,UID_Flag,			1,0,1,0x1f);
	});
	
}


BOOST_AUTO_TEST_CASE(test_case_GetVIDResponse_6)
{
	

	GetVIDRequest req;

	VID_ADD_DATA_1B( 0,RS485_Baudrate,		1,0,3,2);
	VID_ADD_DATA_1B( 1,TTL_Baudrate,		1,0,3,2);
	VID_ADD_DATA_1B( 2,RS232_Baudrate,		1,0,3,2);
	VID_ADD_DATA_1B( 3,IO_PA_IO_Mode,		1,4,7,0);
	VID_ADD_DATA_1B( 4,IO_PA_Analog_Mode,	1,4,7,0);
	VID_ADD_DATA_1B( 5,IO_PA_PWM,			1,0,1,1);
	VID_ADD_DATA_2B( 6,IO_PA_PWM_CYCLE,		2,0,16383,16383);
	VID_ADD_DATA_1B( 8,Through_Port,		1,0,254,0);
	VID_ADD_DATA_1B( 9,Servo_Type_RS485,	1,0,3,1);
	VID_ADD_DATA_1B(10,Servo_Type_TTL,		1,0,3,0);
	VID_ADD_DATA_1B(11,IMU_Type,			1,0,2,3);
	VID_ADD_DATA_1B(12,Balancer_Flag,		1,0,1,0);
	VID_ADD_DATA_1B(13,Theta_Th,			1,1,100,0xa);
	VID_ADD_DATA_1B(14,Cycletime,			1,1,100,1);
	VID_ADD_DATA_1B(15,Min_Cmp,				1,1,250,0xa);
	VID_ADD_DATA_1B(16,Flag_Ack,			1,0,1,1);
	VID_ADD_DATA_1B(17,Volt_Th,				1,60,90,0);
	VID_ADD_DATA_1B(18,Initialize_Torque,	1,0,1,1);
	VID_ADD_DATA_1B(19,Initialize_Angle,	1,0,1,1);
	VID_ADD_DATA_1B(20,Inspection_Flag,		1,0,1,1);
	VID_ADD_DATA_1B(21,Inspection_Type,		1,0,1,0);
	VID_ADD_DATA_1B(22,Robot_Model,			1,0,2,0xe);
	VID_ADD_DATA_1B(23,UID_Flag,			1,0,1,0x1f);


	/// uart data
	list<unsigned char> recieved = 
	{
		0xff,0x67 ,0x1c ,0x02 ,0x02 ,0x02 ,0x00 ,0x00,
		0x01 ,0xfe ,0xfe ,0x00 ,0x01 ,0x00 ,0x03 ,0x00,
		0x0a ,0x01 ,0x0a ,0x01 ,0x00 ,0x01 ,0x01 ,0x01,
		0x00 ,0x0e ,0x1f ,0x58
	};
	DUMMY_ACK(recieved);

	auto ack = req.execBK();
	
	
	ack.forEachVar([](const string &name ,int value){
		VID_CHECK_DATA_1B( 0,RS485_Baudrate,		1,0,3,2);
		VID_CHECK_DATA_1B( 1,TTL_Baudrate,		1,0,3,2);
		VID_CHECK_DATA_1B( 2,RS232_Baudrate,		1,0,3,2);
		VID_CHECK_DATA_1B( 3,IO_PA_IO_Mode,		1,4,7,0);
		VID_CHECK_DATA_1B( 4,IO_PA_Analog_Mode,	1,4,7,0);
		VID_CHECK_DATA_1B( 5,IO_PA_PWM,			1,0,1,1);
		VID_CHECK_DATA_2B( 6,IO_PA_PWM_CYCLE,		2,0,16383,16383);
		VID_CHECK_DATA_1B( 8,Through_Port,		1,0,254,0);
		VID_CHECK_DATA_1B( 9,Servo_Type_RS485,	1,0,3,1);
		VID_CHECK_DATA_1B(10,Servo_Type_TTL,		1,0,3,0);
		VID_CHECK_DATA_1B(11,IMU_Type,			1,0,2,3);
		VID_CHECK_DATA_1B(12,Balancer_Flag,		1,0,1,0);
		VID_CHECK_DATA_1B(13,Theta_Th,			1,1,100,0xa);
		VID_CHECK_DATA_1B(14,Cycletime,			1,1,100,1);
		VID_CHECK_DATA_1B(15,Min_Cmp,				1,1,250,0xa);
		VID_CHECK_DATA_1B(16,Flag_Ack,			1,0,1,1);
		VID_CHECK_DATA_1B(17,Volt_Th,				1,60,90,0);
		VID_CHECK_DATA_1B(18,Initialize_Torque,	1,0,1,1);
		VID_CHECK_DATA_1B(19,Initialize_Angle,	1,0,1,1);
		VID_CHECK_DATA_1B(20,Inspection_Flag,		1,0,1,1);
		VID_CHECK_DATA_1B(21,Inspection_Type,		1,0,1,0);
		VID_CHECK_DATA_1B(22,Robot_Model,			1,0,2,0xe);
		VID_CHECK_DATA_1B(23,UID_Flag,			1,0,1,0x1f);
	});
	
}

