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




#define CHECK_SERVO(x,y,z) \
{ \
	if(#y==name)\
	{\
		BOOST_CHECK_EQUAL(value,z);\
	}\
}
#define CHECK_SERVO_1B CHECK_SERVO
#define CHECK_SERVO_2B CHECK_SERVO


#define SERVO_DATA_1B(x,y,z) (z)
#define SERVO_DATA_2B(x,y,z) _HBYTE(z),_LBYTE(z)


BOOST_AUTO_TEST_CASE(test_case_FeedBackResponse_0)
{

	/// uart data
	list<unsigned char> recieved = 
	{
		2,
		SERVO_DATA_2B( 0,rom_model_num,			0x1234),
		SERVO_DATA_1B( 2,rom_servo_ID,			0x1f),
		SERVO_DATA_2B( 3,rom_cw_agl_lmt,		0x1234),
		SERVO_DATA_2B( 5,rom_ccw_agl_lmt,		0x1234),
		SERVO_DATA_1B( 7,rom_damper,			0x1f),
		SERVO_DATA_1B( 8,rom_cw_cmp_margin,		0x1f),
		SERVO_DATA_1B( 9,rom_ccw_cmp_margin,	0x1f),
		SERVO_DATA_1B(10,rom_cw_cmp_slope,		0x1f),
		SERVO_DATA_1B(11,rom_ccw_cmp_slope,		0x1f),
		SERVO_DATA_1B(12,rom_punch,				0x1f),
		SERVO_DATA_2B(13,ram_goal_pos,			0x1234),
		SERVO_DATA_2B(15,ram_goal_tim,			0x1234),
		SERVO_DATA_1B(17,ram_max_torque,		0x1f),
		SERVO_DATA_1B(18,ram_torque_mode,		0x1f),
		SERVO_DATA_2B(19,ram_pres_pos,			0x1234),
		SERVO_DATA_2B(21,ram_pres_time,			0x1234),
		SERVO_DATA_2B(23,ram_pres_spd,			0x1234),
		SERVO_DATA_2B(25,ram_pres_curr,			0x1234),
		SERVO_DATA_2B(27,ram_pres_temp,			0x1234),
		SERVO_DATA_2B(29,ram_pres_volt,			0x1234),
		SERVO_DATA_1B(31,Flags,					0x1f),
		SERVO_DATA_2B(32,alg_ofset,				0x1234),
		SERVO_DATA_1B(34,parents_ID,			0x1f),
		SERVO_DATA_1B(35,connected,				0x1f),
		SERVO_DATA_2B(36,read_time,				0x1234),
		SERVO_DATA_2B(38,_ram_goal_pos,			0x1234),
		SERVO_DATA_2B(40,__ram_goal_pos,		0x1234),
		SERVO_DATA_2B(42,_ram_res_pos,			0x1234),
		SERVO_DATA_1B(44,_send_speed,			0x1f),
		SERVO_DATA_1B(45,_send_cmd_time,		0x1f),
		SERVO_DATA_1B(46,flg_min_max,			0x1f),
		SERVO_DATA_1B(47,flg_goal_pos,			0x1f),
		SERVO_DATA_1B(48,flg_parent_inv,		0x1f),
		SERVO_DATA_1B(49,flg_cmp_slope,			0x1f),
		SERVO_DATA_1B(50,flg_check_angle,		0x1f),
		SERVO_DATA_1B(51,port_type,				0x1f),
		SERVO_DATA_1B(52,servo_type,			0x1f),
	};
	GEN_COMMON_READ_UART('r');
	DUMMY_ACK(recieved);

	FeedBackRequest req;
	req.feedback(0,53);
	auto ack = req.exec();

	dumpTestData(sendBuffer);
	dumpTestData(recieved);
	
	auto feedback = ack.feedback();

	DUMP_VAR(feedback.size());
	BOOST_CHECK_EQUAL(feedback.size(),1);
	DUMP_VAR(feedback[2].size());
	BOOST_CHECK_EQUAL(feedback[2].size(),37);
	
	for(auto const &kv:feedback)
	{
		DUMP_VAR(kv.first);
		for(auto pair :kv.second)
		{
			auto name = std::get<0>(pair);
			auto value = std::get<1>(pair);
			DUMP_VAR(name);
			DUMP_VAR(value);

			CHECK_SERVO_2B( 0,rom_model_num,			0x1234);
			CHECK_SERVO_1B( 2,rom_servo_ID,			0x1f);
			CHECK_SERVO_2B( 3,rom_cw_agl_lmt,		0x1234);
			CHECK_SERVO_2B( 5,rom_ccw_agl_lmt,		0x1234);
			CHECK_SERVO_1B( 7,rom_damper,			0x1f);
			CHECK_SERVO_1B( 8,rom_cw_cmp_margin,	0x1f);
			CHECK_SERVO_1B( 9,rom_ccw_cmp_margin,	0x1f);
			CHECK_SERVO_1B(10,rom_cw_cmp_slope,		0x1f);
			CHECK_SERVO_1B(11,rom_ccw_cmp_slope,	0x1f);
			CHECK_SERVO_1B(12,rom_punch,			0x1f);
			CHECK_SERVO_2B(13,ram_goal_pos,			0x1234);
			CHECK_SERVO_2B(15,ram_goal_tim,			0x1234);
			CHECK_SERVO_1B(17,ram_max_torque,		0x1f);
			CHECK_SERVO_1B(18,ram_torque_mode,		0x1f);
			CHECK_SERVO_2B(19,ram_pres_pos,			0x1234);
			CHECK_SERVO_2B(21,ram_pres_time,			0x1234);
			CHECK_SERVO_2B(23,ram_pres_spd,			0x1234);
			CHECK_SERVO_2B(25,ram_pres_curr,			0x1234);
			CHECK_SERVO_2B(27,ram_pres_temp,			0x1234);
			CHECK_SERVO_2B(29,ram_pres_volt,			0x1234);
			CHECK_SERVO_1B(31,Flags,					0x1f);
			CHECK_SERVO_2B(32,alg_ofset,				0x1234);
			CHECK_SERVO_1B(34,parents_ID,			0x1f);
			CHECK_SERVO_1B(35,connected,				0x1f);
			CHECK_SERVO_2B(36,read_time,				0x1234);
			CHECK_SERVO_2B(38,_ram_goal_pos,			0x1234);
			CHECK_SERVO_2B(40,__ram_goal_pos,		0x1234);
			CHECK_SERVO_2B(42,_ram_res_pos,			0x1234);
			CHECK_SERVO_1B(44,_send_speed,			0x1f);
			CHECK_SERVO_1B(45,_send_cmd_time,		0x1f);
			CHECK_SERVO_1B(46,flg_min_max,			0x1f);
			CHECK_SERVO_1B(47,flg_goal_pos,			0x1f);
			CHECK_SERVO_1B(48,flg_parent_inv,		0x1f);
			CHECK_SERVO_1B(49,flg_cmp_slope,			0x1f);
			CHECK_SERVO_1B(50,flg_check_angle,		0x1f);
			CHECK_SERVO_1B(51,port_type,				0x1f);
			CHECK_SERVO_1B(52,servo_type,			0x1f);
		}
	}

}

BOOST_AUTO_TEST_CASE(test_case_FeedBackResponse_1)
{

	/// uart data
	list<unsigned char> recieved = {2};
	for(int i = 1;i < 54;i++)
	{
		recieved.push_back(i);
	}
	recieved.push_back(5);
	for(int i = 1;i < 54;i++)
	{
		recieved.push_back(i);
	}
	
	GEN_COMMON_READ_UART('r');
	DUMMY_ACK(recieved);


	FeedBackRequest req;
	req.feedback(0,53);
	auto ack = req.exec();

	dumpTestData(sendBuffer);
	dumpTestData(recieved);
	
	auto feedback = ack.feedback();
	DUMP_VAR(feedback.size());
	BOOST_CHECK_EQUAL(feedback.size(),2);

	DUMP_VAR(feedback[2].size());
	BOOST_CHECK_EQUAL(feedback[2].size(),37);

	DUMP_VAR(feedback[5].size());
	BOOST_CHECK_EQUAL(feedback[5].size(),37);
	
}

BOOST_AUTO_TEST_CASE(test_case_FeedBackResponse_2)
{

	/// uart data
	list<unsigned char> recieved = {2};
	for(int i = 1;i < 33;i++)
	{
		recieved.push_back(i);
	}
	recieved.push_back(5);
	for(int i = 1;i < 33;i++)
	{
		recieved.push_back(i);
	}
	
	GEN_COMMON_READ_UART('r');
	DUMMY_ACK(recieved);


	FeedBackRequest req;
	req.feedback(5,32);
	auto ack = req.exec();

	dumpTestData(sendBuffer);
	dumpTestData(recieved);
	
	auto feedback = ack.feedback();
	DUMP_VAR(feedback.size());
	BOOST_CHECK_EQUAL(feedback.size(),2);

	DUMP_VAR(feedback[2].size());
	BOOST_CHECK_EQUAL(feedback[2].size(),22);

	DUMP_VAR(feedback[5].size());
	BOOST_CHECK_EQUAL(feedback[5].size(),22);
	
}


BOOST_AUTO_TEST_CASE(test_case_FeedBackResponse_3)
{

	/// uart data
	list<unsigned char> recieved = {2};
	for(int i = 1;i < 33;i++)
	{
		recieved.push_back(i);
	}
	recieved.push_back(5);
	for(int i = 1;i < 33;i++)
	{
		recieved.push_back(i);
	}
	
	GEN_COMMON_READ_UART('r');
	DUMMY_ACK(recieved);

	dumpTestData(recieved);

	FeedBackRequest req;
	req.feedback(5,32);
	auto ack = req.exec();

	BOOST_CHECK_EQUAL("" , ack.JSON());
	
}


