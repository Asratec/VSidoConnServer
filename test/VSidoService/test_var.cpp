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
#include "cmd_var.hpp"
#include "cmd_parser.hpp"
using namespace VSido;


#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test.hpp>


#include "test_dump.hpp"
#include "test_uart.hpp"



#define VID_DATA_1B(x,y,z,min,max,val) (x),(val)
#define VID_DATA_2B(x,y,z,min,max,val) (x),_HBYTE(val),(x+1),_LBYTE(val)


BOOST_AUTO_TEST_CASE(test_case_VarSetJSONRequest_0)
{
	string json("{\"cmd\":\"SetVIDValue\",\"vid\":[{\"RS485_Baudrate\":2},{\"TTL_Baudrate\":1},{\"RS232_Baudrate\":0},{\"IO_PA_IO_Mode\":5},{\"IO_PA_Analog_Mode\":6},{\"IO_PA_PWM\":0},{\"IO_PA_PWM_CYCLE\":2261},{\"Through_Port\":100},{\"Servo_Type_RS485\":3},{\"Servo_Type_TTL\":0},{\"IMU_Type\":1},{\"Balancer_Flag\":0},{\"Theta_Th\":32},{\"Cycletime\":73},{\"Min_Cmp\":99},{\"Flag_Ack\":0},{\"Volt_Th\":80},{\"Initialize_Torque\":0},{\"Initialize_Angle\":1},{\"Inspection_Flag\":0},{\"Inspection_Type\":0},{\"Robot_Model\":2},{\"UID_Flag\":0}]}");
	JSONRequestParser parser(json);
	
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		VID_DATA_1B( 0,VID_RS485_Baudrate,		1,0,3,2),
		VID_DATA_1B( 1,VID_TTL_Baudrate,			1,0,3,1),
		VID_DATA_1B( 2,VID_RS232_Baudrate,		1,0,3,0),
		VID_DATA_1B( 3,VID_IO_PA_IO_Mode,			1,4,7,5),
		VID_DATA_1B( 4,VID_IO_PA_Analog_Mode,		1,4,7,6),
		VID_DATA_1B( 5,VID_IO_PA_PWM,				1,0,1,0),
//		VID_DATA_2B( 6,VID_IO_PA_PWM_CYCLE,		2,0,16383,2261/4),
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
		VID_DATA_2B( 6,VID_IO_PA_PWM_CYCLE,		2,0,16383,2261/4),
	};
	GEN_COMMON_SEND_UART('s');
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<VarSetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	DO_ACK_GOOD_CHECK();



}




BOOST_AUTO_TEST_CASE(test_case_VarSetJSONRequest_1)
{
	string json("{\"cmd\":\"SetVIDValue\",\"vid\":[{\"RS485_Baudrate\":\"B_9600\"},{\"TTL_Baudrate\":\"B_57600\"},{\"RS232_Baudrate\":\"B_115200\"},{\"Servo_Type_RS485\":\"None\"},{\"Servo_Type_TTL\":\"None\"},{\"IMU_Type\":\"None\"},{\"Robot_Model\":\"None\"}]}");
	JSONRequestParser parser(json);
	
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		VID_DATA_1B( 0,VID_RS485_Baudrate,		1,0,3,0),
		VID_DATA_1B( 1,VID_TTL_Baudrate,			1,0,3,1),
		VID_DATA_1B( 2,VID_RS232_Baudrate,		1,0,3,2),
		VID_DATA_1B( 9,VID_Servo_Type_RS485,		1,0,3,0),
		VID_DATA_1B(10,VID_Servo_Type_TTL,		1,0,3,0),
		VID_DATA_1B(11,VID_IMU_Type,				1,0,2,0),
		VID_DATA_1B(22,VID_Robot_Model,			1,0,2,0),
	};
	GEN_COMMON_SEND_UART('s');
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<VarSetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	DO_ACK_GOOD_CHECK();



}


BOOST_AUTO_TEST_CASE(test_case_VarSetJSONRequest_2)
{
	string json("{\"cmd\":\"SetVIDValue\",\"vid\":[{\"RS485_Baudrate\":\"B_1000000\"},{\"TTL_Baudrate\":\"B_57600\"},{\"RS232_Baudrate\":\"B_1000000\"},{\"Servo_Type_RS485\":\"FUTABA\"},{\"Servo_Type_TTL\":\"ROBOTIS_v1.0\"},{\"IMU_Type\":\"None\"},{\"Robot_Model\":\"GR-001\"}]}");
	JSONRequestParser parser(json);
	
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		VID_DATA_1B( 0,VID_RS485_Baudrate,		1,0,3,3),
		VID_DATA_1B( 1,VID_TTL_Baudrate,			1,0,3,1),
		VID_DATA_1B( 2,VID_RS232_Baudrate,		1,0,3,3),
		VID_DATA_1B( 9,VID_Servo_Type_RS485,		1,0,3,1),
		VID_DATA_1B(10,VID_Servo_Type_TTL,		1,0,3,2),
		VID_DATA_1B(11,VID_IMU_Type,				1,0,2,0),
		VID_DATA_1B(22,VID_Robot_Model,			1,0,2,1),
	};
	GEN_COMMON_SEND_UART('s');
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<VarSetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	DO_ACK_GOOD_CHECK();



}

BOOST_AUTO_TEST_CASE(test_case_VarSetJSONRequest_3)
{
	string json("{\"cmd\":\"SetVIDValue\",\"vid\":[{\"RS485_Baudrate\":\"B_1000000\"},{\"TTL_Baudrate\":\"B_57600\"},{\"RS232_Baudrate\":\"B_1000000\"},{\"Servo_Type_RS485\":\"ROBOTIS_v2.0\"},{\"Servo_Type_TTL\":\"ROBOTIS_v1.0\"},{\"IMU_Type\":\"None\"},{\"Robot_Model\":\"DARWIN-MINI\"}]}");
	JSONRequestParser parser(json);
	
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		VID_DATA_1B( 0,VID_RS485_Baudrate,		1,0,3,3),
		VID_DATA_1B( 1,VID_TTL_Baudrate,			1,0,3,1),
		VID_DATA_1B( 2,VID_RS232_Baudrate,		1,0,3,3),
		VID_DATA_1B( 9,VID_Servo_Type_RS485,		1,0,3,3),
		VID_DATA_1B(10,VID_Servo_Type_TTL,		1,0,3,2),
		VID_DATA_1B(11,VID_IMU_Type,				1,0,2,0),
		VID_DATA_1B(22,VID_Robot_Model,			1,0,2,2),
	};
	GEN_COMMON_SEND_UART('s');
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<VarSetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	DO_ACK_GOOD_CHECK();



}


BOOST_AUTO_TEST_CASE(test_case_VarSetJSONRequest_4)
{
	string json("{\"cmd\":\"SetVIDValue\",\"vid\":[{\"dfkdskfjadsfllRS485_Baudrate\":1}]}");
	JSONRequestParser parser(json);
	
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
	};
	GEN_COMMON_SEND_UART('s');
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<VarSetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	DO_ACK_GOOD_CHECK();
}


BOOST_AUTO_TEST_CASE(test_case_VarSetJSONRequest_5)
{
	string json("{\"cmd\":\"SetVIDValue\",\"vid\":[]}");
	JSONRequestParser parser(json);
	
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
	};
	GEN_COMMON_SEND_UART('s');
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<VarSetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	DO_ACK_GOOD_CHECK();
}



BOOST_AUTO_TEST_CASE(test_case_VarSetJSONRequest_6)
{
	string json("{\"command\":\"SetVIDValue\",\"vid\":[{\"IO_PA_IO_Mode\":\"0\"},{\"IO_PA_Analog_Mode\":\"0\"},{\"IO_PA_PWM\":\"0\"}]}");
	JSONRequestParser parser(json);
	
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x03,0x00,0x04,0x00,0x05,0x00
	};
	GEN_COMMON_SEND_UART('s');
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<VarSetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);
	DO_ACK_GOOD_CHECK();
}


#define VID_DATA_R_1B(x,y,z,min,max,val) (val)
#define VID_DATA_R_2B(x,y,z,min,max,val) _HBYTE(val),_LBYTE(val)


BOOST_AUTO_TEST_CASE(test_case_VarGetJSONRequest_0)
{
	string json("{\"cmd\":\"GetVIDValue\",\"vid\":[\"RS485_Baudrate\",\"TTL_Baudrate\",\"RS232_Baudrate\",\"IO_PA_IO_Mode\",\"IO_PA_Analog_Mode\",\"IO_PA_PWM\",\"IO_PA_PWM_CYCLE\",\"Through_Port\",\"Servo_Type_RS485\",\"Servo_Type_TTL\",\"IMU_Type\",\"Balancer_Flag\",\"Theta_Th\",\"Cycletime\",\"Min_Cmp\",\"Flag_Ack\",\"Volt_Th\",\"Initialize_Torque\",\"Initialize_Angle\",\"Inspection_Flag\",\"Inspection_Type\",\"Robot_Model\",\"UID_Flag\"]}");
	JSONRequestParser parser(json);
	
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23
	};
	GEN_COMMON_SEND_UART('g');
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<VarGetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);


	list<unsigned char> uartAck = 
	{
		VID_DATA_R_1B( 0,VID_RS485_Baudrate,		1,0,3,2),
		VID_DATA_R_1B( 1,VID_TTL_Baudrate,			1,0,3,1),
		VID_DATA_R_1B( 2,VID_RS232_Baudrate,		1,0,3,0),
		VID_DATA_R_1B( 3,VID_IO_PA_IO_Mode,			1,4,7,5),
		VID_DATA_R_1B( 4,VID_IO_PA_Analog_Mode,		1,4,7,6),
		VID_DATA_R_1B( 5,VID_IO_PA_PWM,				1,0,1,0),
		VID_DATA_R_2B( 6,VID_IO_PA_PWM_CYCLE,		2,0,16383,2261/4),
		VID_DATA_R_1B( 8,VID_Through_Port,			1,0,254,100),
		VID_DATA_R_1B( 9,VID_Servo_Type_RS485,		1,0,3,3),
		VID_DATA_R_1B(10,VID_Servo_Type_TTL,		1,0,3,0),
		VID_DATA_R_1B(11,VID_IMU_Type,				1,0,2,1),
		VID_DATA_R_1B(12,VID_Balancer_Flag,			1,0,1,0),
		VID_DATA_R_1B(13,VID_Theta_Th,				1,1,100,32),
		VID_DATA_R_1B(14,VID_Cycletime,				1,1,100,73),
		VID_DATA_R_1B(15,VID_Min_Cmp,				1,1,250,99),
		VID_DATA_R_1B(16,VID_Flag_Ack,				1,0,1,0),
		VID_DATA_R_1B(17,VID_Volt_Th,				1,60,90,80),
		VID_DATA_R_1B(18,VID_Initialize_Torque,		1,0,1,0),
		VID_DATA_R_1B(19,VID_Initialize_Angle,		1,0,1,1),
		VID_DATA_R_1B(20,VID_Inspection_Flag,		1,0,1,0),
		VID_DATA_R_1B(21,VID_Inspection_Type,		1,0,1,0),
		VID_DATA_R_1B(22,VID_Robot_Model,			1,0,2,2),
		VID_DATA_R_1B(23,VID_UID_Flag,				1,0,1,0),
	};
	GEN_COMMON_READ_UART('g');
	
	DUMMY_ACK(uartAck);
	
	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("{\"raw\":\"0xff,0x67,0x1c,0x02,0x01,0x00,0x05,0x06,0x00,0x6a,0x08,0x64,0x03,0x00,0x01,0x00,0x20,0x49,0x63,0x00,0x50,0x00,0x01,0x00,0x00,0x02,0x00,0xd9\",\"type\":\"GetVIDValue\",\"vid\":{\"Balancer_Flag\":0,\"Cycletime\":73,\"Flag_Ack\":0,\"IMU_Type\":1,\"IO_PA_Analog_Mode\":6,\"IO_PA_IO_Mode\":5,\"IO_PA_PWM\":0,\"IO_PA_PWM_CYCLE\":2260,\"Initialize_Angle\":1,\"Initialize_Torque\":0,\"Inspection_Flag\":0,\"Inspection_Type\":0,\"Min_Cmp\":99,\"RS232_Baudrate\":0,\"RS485_Baudrate\":2,\"Robot_Model\":2,\"Servo_Type_RS485\":3,\"Servo_Type_TTL\":0,\"TTL_Baudrate\":1,\"Theta_Th\":32,\"Through_Port\":100,\"UID_Flag\":0,\"Volt_Th\":80}}");
	BOOST_CHECK_EQUAL(ack,jsonAck);

}


BOOST_AUTO_TEST_CASE(test_case_VarGetJSONRequest_1)
{
	string json("{\"cmd\":\"GetVIDValue\",\"vid\":[\"all\"]}");
	JSONRequestParser parser(json);
	
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		0x0c,0x0e,0x10,0x0b,0x04,0x03,0x05,0x06,0x07,0x13,0x12,0x14,0x15,0x0f,0x02,0x00,0x16,0x09,0x0a,0x01,0x0d,0x08,0x17,0x11
	};
	GEN_COMMON_SEND_UART('g');
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<VarGetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);


	list<unsigned char> uartAck = 
	{
		VID_DATA_R_1B(12,VID_Balancer_Flag,			1,0,1,0),
		VID_DATA_R_1B(14,VID_Cycletime,				1,1,100,73),
		VID_DATA_R_1B(16,VID_Flag_Ack,				1,0,1,0),
		VID_DATA_R_1B(11,VID_IMU_Type,				1,0,2,1),
		VID_DATA_R_1B( 4,VID_IO_PA_Analog_Mode,		1,4,7,6),
		VID_DATA_R_1B( 3,VID_IO_PA_IO_Mode,			1,4,7,5),
		VID_DATA_R_1B( 5,VID_IO_PA_PWM,				1,0,1,0),
		VID_DATA_R_2B( 6,VID_IO_PA_PWM_CYCLE,		2,0,16383,2261/4),
		VID_DATA_R_1B(19,VID_Initialize_Angle,		1,0,1,1),
		VID_DATA_R_1B(18,VID_Initialize_Torque,		1,0,1,0),
		VID_DATA_R_1B(20,VID_Inspection_Flag,		1,0,1,0),
		VID_DATA_R_1B(21,VID_Inspection_Type,		1,0,1,0),
		VID_DATA_R_1B(15,VID_Min_Cmp,				1,1,250,99),
		VID_DATA_R_1B( 2,VID_RS232_Baudrate,		1,0,3,0),
		VID_DATA_R_1B( 0,VID_RS485_Baudrate,		1,0,3,2),
		VID_DATA_R_1B(22,VID_Robot_Model,			1,0,2,2),
		VID_DATA_R_1B( 9,VID_Servo_Type_RS485,		1,0,3,3),
		VID_DATA_R_1B(10,VID_Servo_Type_TTL,		1,0,3,0),
		VID_DATA_R_1B( 1,VID_TTL_Baudrate,			1,0,3,1),
		VID_DATA_R_1B(13,VID_Theta_Th,				1,1,100,32),
		VID_DATA_R_1B( 8,VID_Through_Port,			1,0,254,100),
		VID_DATA_R_1B(23,VID_UID_Flag,				1,0,1,0),
		VID_DATA_R_1B(17,VID_Volt_Th,				1,60,90,80),
	};
	GEN_COMMON_READ_UART('g');
	
	DUMMY_ACK(uartAck);
	
	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("{\"raw\":\"0xff,0x67,0x1c,0x00,0x49,0x00,0x01,0x06,0x05,0x00,0x6a,0x08,0x01,0x00,0x00,0x00,0x63,0x00,0x02,0x02,0x03,0x00,0x01,0x20,0x64,0x00,0x50,0xd9\",\"type\":\"GetVIDValue\",\"vid\":{\"Balancer_Flag\":0,\"Cycletime\":73,\"Flag_Ack\":0,\"IMU_Type\":1,\"IO_PA_Analog_Mode\":6,\"IO_PA_IO_Mode\":5,\"IO_PA_PWM\":0,\"IO_PA_PWM_CYCLE\":2260,\"Initialize_Angle\":1,\"Initialize_Torque\":0,\"Inspection_Flag\":0,\"Inspection_Type\":0,\"Min_Cmp\":99,\"RS232_Baudrate\":0,\"RS485_Baudrate\":2,\"Robot_Model\":2,\"Servo_Type_RS485\":3,\"Servo_Type_TTL\":0,\"TTL_Baudrate\":1,\"Theta_Th\":32,\"Through_Port\":100,\"UID_Flag\":0,\"Volt_Th\":80}}");
	BOOST_CHECK_EQUAL(ack,jsonAck);

}



BOOST_AUTO_TEST_CASE(test_case_VarGetJSONRequest_2)
{
	string json("{\"cmd\":\"GetVIDValue\",\"vid\":[\"IO_PA_PWM_CYCLE\"]}");
	JSONRequestParser parser(json);
	
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		6,7
	};
	GEN_COMMON_SEND_UART('g');
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<VarGetJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);


	list<unsigned char> uartAck = 
	{
		VID_DATA_R_2B( 6,VID_IO_PA_PWM_CYCLE,		2,0,16383,16383),
	};
	GEN_COMMON_READ_UART('g');
	
	DUMMY_ACK(uartAck);
	
	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("{\"raw\":\"0xff,0x67,0x06,0xfe,0xfe,0x9e\",\"type\":\"GetVIDValue\",\"vid\":{\"IO_PA_PWM_CYCLE\":65532}}");
	BOOST_CHECK_EQUAL(ack,jsonAck);

}



