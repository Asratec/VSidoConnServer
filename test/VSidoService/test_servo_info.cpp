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
#include "cmd_servo_info.hpp"
#include "cmd_parser.hpp"
using namespace VSido;


#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test.hpp>


#include "test_dump.hpp"
#include "test_uart.hpp"











#define SERVO_DATA_1B(x,y,z) (z)
#define SERVO_DATA_2B(x,y,z) _HBYTE(z),_LBYTE(z)


#define FILL_SERVO_DATA(x)\
		x,\
		SERVO_DATA_2B( 0,rom_model_num,			0x1234),\
		SERVO_DATA_1B( 2,rom_servo_ID,			0x1f),\
		SERVO_DATA_2B( 3,rom_cw_agl_lmt,		0x1234),\
		SERVO_DATA_2B( 5,rom_ccw_agl_lmt,		0x1234),\
		SERVO_DATA_1B( 7,rom_damper,			0x1f),\
		SERVO_DATA_1B( 8,rom_cw_cmp_margin,		0x1f),\
		SERVO_DATA_1B( 9,rom_ccw_cmp_margin,	0x1f),\
		SERVO_DATA_1B(10,rom_cw_cmp_slope,		0x1f),\
		SERVO_DATA_1B(11,rom_ccw_cmp_slope,		0x1f),\
		SERVO_DATA_1B(12,rom_punch,				0x1f),\
		SERVO_DATA_2B(13,ram_goal_pos,			0x1234),\
		SERVO_DATA_2B(15,ram_goal_tim,			0x1234),\
		SERVO_DATA_1B(17,ram_max_torque,		0x1f),\
		SERVO_DATA_1B(18,ram_torque_mode,		0x1f),\
		SERVO_DATA_2B(19,ram_pres_pos,			0x1234),\
		SERVO_DATA_2B(21,ram_pres_time,			0x1234),\
		SERVO_DATA_2B(23,ram_pres_spd,			0x1234),\
		SERVO_DATA_2B(25,ram_pres_curr,			0x1234),\
		SERVO_DATA_2B(27,ram_pres_temp,			0x1234),\
		SERVO_DATA_2B(29,ram_pres_volt,			0x1234),\
		SERVO_DATA_1B(31,Flags,					0x1f),\
		SERVO_DATA_2B(32,alg_ofset,				0x1234),\
		SERVO_DATA_1B(34,parents_ID,			0x1f),\
		SERVO_DATA_1B(35,connected,				0x1f),\
		SERVO_DATA_2B(36,read_time,				0x1234),\
		SERVO_DATA_2B(38,_ram_goal_pos,			0x1234),\
		SERVO_DATA_2B(40,__ram_goal_pos,		0x1234),\
		SERVO_DATA_2B(42,_ram_res_pos,			0x1234),\
		SERVO_DATA_1B(44,_send_speed,			0x1f),\
		SERVO_DATA_1B(45,_send_cmd_time,		0x1f),\
		SERVO_DATA_1B(46,flg_min_max,			0x1f),\
		SERVO_DATA_1B(47,flg_goal_pos,			0x1f),\
		SERVO_DATA_1B(48,flg_parent_inv,		0x1f),\
		SERVO_DATA_1B(49,flg_cmp_slope,			0x1f),\
		SERVO_DATA_1B(50,flg_check_angle,		0x1f),\
		SERVO_DATA_1B(51,port_type,				0x1f),\
		SERVO_DATA_1B(52,servo_type,			0x1f),\
		



BOOST_AUTO_TEST_CASE(test_case_ServoInfoJSONRequest_0)
{
	string json("{\"cmd\":\"GetServoInfo\",\"servo\":[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20],\"item\":[\"all\"]}");
	JSONRequestParser parser(json);
	
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		 1,0,53,
		 2,0,53,
		 3,0,53,
		 4,0,53,
		 5,0,53,
		 6,0,53,
		 7,0,53,
		 8,0,53,
		 9,0,53,
		10,0,53,
		11,0,53,
		12,0,53,
		13,0,53,
		14,0,53,
		15,0,53,
		16,0,53,
		17,0,53,
		18,0,53,
		19,0,53,
		20,0,53,
	};
	GEN_COMMON_SEND_UART('d');
	dumpTestData(uartExpectedSend);
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<ServoInfoJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);


	list<unsigned char> uartAck = 
	{
		FILL_SERVO_DATA(1)
		FILL_SERVO_DATA(2)
		FILL_SERVO_DATA(3)
		FILL_SERVO_DATA(4)
		FILL_SERVO_DATA(5)
		FILL_SERVO_DATA(6)
		FILL_SERVO_DATA(7)
		FILL_SERVO_DATA(8)
		FILL_SERVO_DATA(9)
		FILL_SERVO_DATA(10)
		FILL_SERVO_DATA(11)
		FILL_SERVO_DATA(12)
		FILL_SERVO_DATA(13)
		FILL_SERVO_DATA(14)
		FILL_SERVO_DATA(15)
		FILL_SERVO_DATA(16)
		FILL_SERVO_DATA(17)
		FILL_SERVO_DATA(18)
		FILL_SERVO_DATA(19)
		FILL_SERVO_DATA(20)
	};
	GEN_COMMON_READ_UART('d');
	
	DUMMY_ACK(uartAck);
	
	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);

}






BOOST_AUTO_TEST_CASE(test_case_ServoInfoJSONRequest_1)
{
	string json("{\"cmd\":\"GetServoInfo\",\"servo\":[1],\"item\":[\"all\"]}");
	JSONRequestParser parser(json);
	
	/// uart data
	list<unsigned char> uartExpectedSend = 
	{
		 1,0,53,
	};
	GEN_COMMON_SEND_UART('d');
	dumpTestData(uartExpectedSend);
	
	auto request = parser.create();
	DUMP_VAR(typeid(request).name());
	
	auto _request = std::dynamic_pointer_cast<ServoInfoJSONRequest>(request);
	DUMP_VAR(typeid(_request).name());
	
	BOOST_CHECK(nullptr != _request);


	list<unsigned char> uartAck = 
	{
		FILL_SERVO_DATA(1)
	};
	GEN_COMMON_READ_UART('d');
	
	DUMMY_ACK(uartAck);
	
	auto ack = request->exec();
	dumpTestData(ut_uart_send);
	dumpTestData(uartExpectedSend);
	BOOST_CHECK(ut_uart_send == uartExpectedSend);
	
	
	DUMP_VAR(ack);
	const string jsonAck("{\"raw\":\"0xff,0x64,0x3a,0x01,0x68,0x48,0x1f,0x68,0x48,0x68,0x48,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x68,0x48,0x68,0x48,0x1f,0x1f,0x68,0x48,0x68,0x48,0x68,0x48,0x68,0x48,0x68,0x48,0x68,0x48,0x1f,0x68,0x48,0x1f,0x1f,0x68,0x48,0x68,0x48,0x68,0x48,0x68,0x48,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0xbf\",\"servo\":[{\"Flags\":31,\"__ram_goal_pos\":466,\"_ram_goal_pos\":466,\"_ram_res_pos\":466,\"_send_cmd_time\":31,\"_send_speed\":31,\"alg_ofset\":466,\"connected\":31,\"flg_check_angle\":31,\"flg_cmp_slope\":31,\"flg_goal_pos\":31,\"flg_min_max\":31,\"flg_parent_inv\":31,\"parents_ID\":31,\"port_type\":31,\"ram_goal_pos\":466,\"ram_goal_tim\":4660,\"ram_max_torque\":31,\"ram_pres_curr\":4660,\"ram_pres_pos\":466,\"ram_pres_spd\":4660,\"ram_pres_temp\":4660,\"ram_pres_time\":4660,\"ram_pres_volt\":4660,\"ram_torque_mode\":31,\"read_time\":4660,\"rom_ccw_agl_lmt\":466,\"rom_ccw_cmp_margin\":31,\"rom_ccw_cmp_slope\":31,\"rom_cw_agl_lmt\":466,\"rom_cw_cmp_margin\":31,\"rom_cw_cmp_slope\":31,\"rom_damper\":31,\"rom_model_num\":4660,\"rom_punch\":31,\"rom_servo_ID\":31,\"servo_type\":31,\"sid\":1}],\"type\":\"GetServoInfo\"}");
	BOOST_CHECK_EQUAL(ack,jsonAck);

}



