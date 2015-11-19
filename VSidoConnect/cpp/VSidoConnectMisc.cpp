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
#include "VSidoConnect.hpp"
#include "vsido_servo_info_table.hpp"
#include "vsido_vid_table.hpp"
#include "VSidoConnectInner.hpp"
using namespace VSido;
#include "debug.h"


#include <iostream>
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <thread>
using namespace std;




#define log_erro_param(...)


namespace VSido
{
/** 送信する
* @param data 送信データ
* @return None
*/
void sendUart(list<unsigned char> data);

} // namespace VSido









/** コンストラクタ
*/
Request::Request()
:uart_{}
,st_(0xff)
,cmd_(0x0)
,length_(0x4)
,sum_(0x0)
,good_(true)
{
	if(useuid_)
	{
		length_ = 0x6;
	}
}

unsigned char Request::uidCounter_ = 1;

/** UART共通の部分を生成する
* @param None
* @return None
*/
void Request::uartCommon()
{
	uart_.push_front(length_);
	uart_.push_front(cmd_);
	if(useuid_)
	{
		auto uid2 = (unsigned char)(uidCounter_%16) | 0x80;
		uart_.push_front(uid2);
		uid_ = uid2;
		auto uid1 = (unsigned char)((uidCounter_>>4)%16) | 0x80;
		uart_.push_front( uid1);
		uid_ |= (uid1 << 8);

		uidCounter_++;
	}
	uart_.push_front(st_);
	/// do check sum
	for(auto data : uart_)
	{
		sum_ ^= data;
	}
	uart_.push_back(sum_);
}


/**
* 要求コマンドを実行する
* @return None
*/
void Request::exec()
{
	this->uart();
	this->uartCommon();
	sendUart(this->uart_);
}

/**
* 要求コマンドを実行する(No Ack)
* @return None
*/
void Request::execNA()
{
	exec();
}



/** コンストラクタ
*/
Response::Response(list<unsigned char> uart,Request &req)
:uart_(uart)
,raw_(uart)
,req_(req)
,pair_(true)
,mine_(true)
,bad_format_(false)
{
	DUMP_VAR(uart_.size());
	DUMP_VAR(raw_.size());
	int checkSameCounter = 1;
	if(Request::useuid_)
	{
		checkSameCounter = 3;
	}

	DUMP_VAR_DETAILS(checkSameCounter);
	for(int i = 0;i < checkSameCounter;i++)
	{
		DUMP_VAR_DETAILS(uart_.size());
		DUMP_VAR_DETAILS(req.uart_.size());
		if(uart_.empty() || req.uart_.empty())
		{
			pair_ = false;
			break;
		}
		auto resChar = uart_.front();
		auto reqChar = req.uart_.front();
		DUMP_VAR_DETAILS((int)resChar);
		DUMP_VAR_DETAILS((int)reqChar);
		if(resChar != reqChar)
		{
			pair_ = false;
			break;
		}
		uart_.pop_front();
		req.uart_.pop_front();
	}
	// check op
	if(uart_.empty())
	{
		mine_ = false;
	}
	else
	{
		op_ = uart_.front();
		uart_.pop_front();
	}
	// length
	if(false == uart_.empty())
	{
		uart_.pop_front();
	}

	// check sum
	if(false == uart_.empty())
	{
		uart_.pop_back();
	}
	DUMP_VAR(uart_.size());
	DUMP_VAR(raw_.size());
	
}

/** JSONへ変換する
* @return 文字列
*/
string Response::JSON()
{
	string json;
	return json;
}

#include <cstdio>
#include <cstdlib>
using namespace std;

/** 文字列へ変換する
* @return 文字列
*/
Response::operator string ()
{
	DUMP_VAR(raw_.size());
	string ret;
	for(auto ch :raw_)
	{
		char buff[6] = {0};
		sprintf(buff,"0x%02x,",ch);
		ret += buff;
	}
	if(false == ret.empty())
	{
		ret.pop_back();
	}
	DUMP_VAR(ret);
	return ret;
}

static AngleRequest badAngleRequest;
/** コンストラクタ
*/
AckResponse::AckResponse()
:Response({},badAngleRequest)
{
	mine_ = false;
}

/** コンストラクタ
*/
AckResponse::AckResponse(list<unsigned char> uart,Request &req)
:Response(uart,req)
{
	if('!' != op_)
	{
		mine_ = false;
	}
}

/** メッセージを取得する。
* @return メッセージ
*/
string AckResponse::msg(void)
{
	string msg;
	for(auto ch :uart_)
	{
		msg += (char)ch;
	}
	return msg;
}

/** JSONへ変換する
* @return 文字列
*/
string AckResponse::JSON()
{
	string json;
	return json;
}





/** コンストラクタ
*/
AngleRequest::AngleRequest()
:cycle_(1)
,angle_()
{
	cmd_ = 'o';
}

/** 目標角度に移行するまでの時間を設定する
* @param cyc 指定範囲 1～100 （単位は 10msec）
* @return None
*/
void AngleRequest::cycle(unsigned char cyc)
{
	if(1 > cyc || 100 < cyc)
	{
		return;
	}
	cycle_ = cyc;
}

/** 目標角度を指定する
* @param sid サーボ ID 指定範囲 1～254
* @param deg 目標角度 指定範囲 -180.0～180.0 （精度0.1deg 刻み）
* @return None
*/
void AngleRequest::angle(unsigned char sid,double deg)
{
	if(false == checkSID(sid))
	{
		log_erro_param(SID);
		return ;
	}
	if(false == checkAngle(deg))
	{
		log_erro_param(deg);
		return ;
	}
	angle_.push_back(make_tuple(sid,deg));
}

/** UARTコマンドに変更する。
* @return None
*/
void AngleRequest::uart()
{
	uart_.push_back((unsigned char)cycle_);
	for(const auto &servo:angle_)
	{
		auto sid = std::get<0>(servo);
		uart_.push_back((unsigned char)sid);

		auto angle = std::get<1>(servo);
		convert2Byte((int16_t)(10*angle));
	}
	length_ += uart_.size();
}


/** コンストラクタ
*/
ComplianceRequest::ComplianceRequest()
:comp_()
{
	cmd_ = 'c';
}



/** コンプライアンスを指定する
* 複数の指定可能。
* @param sid サーボ ID 指定範囲 1～254
* @param cpCW 時計回りのコンプライアンススロープ値
*                  指定範囲 1～250 （1deg 刻み）
* @param cpCCW 反時計回りのコンプライアンススロープ値
*                  指定範囲 1～250 （1deg 刻み）
* @return None
*/
void ComplianceRequest::comp(unsigned char sid,unsigned char cpCW,unsigned char cpCCW)
{
	if(false == checkSID(sid))
	{
		log_erro_param(SID);
		return ;
	}
	if(false == checkCompliance(cpCW))
	{
		log_erro_param(cpCW);
		return ;
	}
	if(false == checkCompliance(cpCCW))
	{
		log_erro_param(cpCCW);
		return ;
	}
	comp_.push_back(make_tuple(sid,cpCW,cpCCW));
}

/** UARTコマンドに変更する。
* @return None
*/
void ComplianceRequest::uart()
{
	for(const auto &servo:comp_)
	{
		auto sid = std::get<0>(servo);
		uart_.push_back((unsigned char)sid);

		auto cp1 = std::get<1>(servo);
		uart_.push_back((unsigned char)cp1);

		auto cp2 = std::get<2>(servo);
		uart_.push_back((unsigned char)cp2);
	}
	length_ += uart_.size();
}



/** コンストラクタ
*/
MinMaxAngleRequest::MinMaxAngleRequest()
:minMax_()
{
	cmd_ = 'm';
}


/** 最大・最小角設定を指定する
* 複数の指定可能。
* @param sid サーボ ID 指定範囲 1～254
* @param min サーボ最小角度
*                  指定範囲 -180.0～180.0 （精度0.1deg 刻み）
* @param max サーボ最大角度
*                  指定範囲 -180.0～180.0 （精度0.1deg 刻み）
* @return None
*/
void MinMaxAngleRequest::minMax(unsigned char sid,double min,double max)
{
	if(false == checkSID(sid))
	{
		log_erro_param(SID);
		return ;
	}
	if(false == checkAngle(min))
	{
		log_erro_param(min);
		return ;
	}
	if(false == checkAngle(max))
	{
		log_erro_param(max);
		return ;
	}
	minMax_.push_back(make_tuple(sid,min,max));
}

/** UARTコマンドに変更する。
* @return None
*/
void MinMaxAngleRequest::uart()
{
	for(const auto &servo:minMax_)
	{
		auto sid = std::get<0>(servo);
		uart_.push_back((unsigned char)sid);
/// 2,1の順に書いているが、ここは正しい
		auto max = std::get<2>(servo);
		convert2Byte((int16_t)(10*max));
		DUMP_VAR_DETAILS((int16_t)(10*max));

		auto min = std::get<1>(servo);
		convert2Byte((int16_t)(10*min));
		DUMP_VAR_DETAILS((int16_t)(10*min));

	}
	length_ += uart_.size();
}



/** コンストラクタ
*/
WriteFlashRequest::WriteFlashRequest()
{
	cmd_ = 'w';
}




/** コンストラクタ
*/
GPIORequest::GPIORequest()
:gpip_()
{
	cmd_ = 'i';
}

/** GPIOを指定する
* 複数の指定可能。
* @param port GPIO入出力ポート
* 指定範囲 4～7
* 注）V-Sido CONNECT RC の基板上に記載された PA4～PA7 に対応する。
* @param val IOの出力内容
* false が入力、true が出力
* @return None
*/
void GPIORequest::gpio(unsigned char port,bool val)
{
	if(4 > port || 7< port)
	{
		return;
	}
	gpip_.push_back(make_tuple(port,val));
}

/** UARTコマンドに変更する。
* @return None
*/
void GPIORequest::uart()
{
	for(const auto &io:gpip_)
	{
		auto port = std::get<0>(io);
		uart_.push_back((unsigned char)port);

		auto val = std::get<1>(io);
		uart_.push_back((unsigned char)val);
	}
	length_ += uart_.size();
}

/** コンストラクタ
*/
PWMRequest::PWMRequest()
:pluse_()
{
	cmd_ = 'p';
}

/** GPIOを指定する
* 複数の指定可能。
* @param port GPIO入出力ポート
* 指定範囲 6 or 7
* 注）V-Sido CONNECT RC の基板上に記載された PA6、PA7 に対応する
* @param width パルス幅
* パルス幅は、4usec 刻みで 14bit の範囲で設定する。
* 指定範囲 0～16383
* @return None
*/
void PWMRequest::pluse(unsigned char port,unsigned short width)
{
	if(6 > port || 7< port)
	{
		return;
	}
	pluse_.push_back(make_tuple(port,width));
}

/** UARTコマンドに変更する。
* @return None
*/
void PWMRequest::uart()
{
	for(const auto &pls:pluse_)
	{
		auto port = std::get<0>(pls);
		uart_.push_back((unsigned char)port);

		auto width = std::get<1>(pls);
		convert2Byte((int16_t)(width));
	}
	length_ += uart_.size();
}


/** コンストラクタ
*/
JointRequest::JointRequest()
{
	cmd_ = 'j';
}


static JointRequest badJointRequest;
/** コンストラクタ
*/
JointResponse::JointResponse()
:Response({},badJointRequest)
,parse_(true)
,tim_()
{
	mine_ = false;
}




/** コンストラクタ
*/
JointResponse::JointResponse(list<unsigned char> uart,Request &req)
:Response(uart,req)
,parse_(true)
,tim_()
{
	if('j' != op_)
	{
		mine_ = false;
	}
}

/** JSONへ変換する
* @return 文字列
*/
string JointResponse::JSON()
{
	return "";
}

/** データを取得する
* @return Servo TIMデータ
*/
vector<tuple<unsigned char,unsigned char>> JointResponse::tim()
{
	this->parse();
	return tim_;
}

void JointResponse::parse()
{
	if(false == parse_)
	{
		return ;
	}
	parse_ = false;
	int width = 2;
	int i = 0;
	unsigned char pre = 0;
	for(auto ch :uart_)
	{
		if(0 == ++i%width)
		{
			tim_.push_back(make_tuple(pre,ch));
		}
		pre = ch;
	}
}








/** コンストラクタ
*/
WalkRequest::WalkRequest()
:forward_(100)
,turn_(100)
{
	cmd_ = 't';
}

/** 前後に移動する
* @param speed 前後の速度
* プラス前進、マイナス後退
* @return None
*/
void WalkRequest::forward(signed char speed)
{
	if(-100 > speed || 100 < speed)
	{
		return ;
	}
	forward_ = (unsigned char)(speed+100);
}

/** 旋回する
* @param speed 旋回の速度
* プラスCW、マイナスCCW
* @return None
*/
void WalkRequest::turn(signed char speed)
{
	if(-100 > speed || 100 < speed)
	{
		return ;
	}
	turn_ = (unsigned char)(speed+100);
}

/** UARTコマンドに変更する。
* @return None
*/
void WalkRequest::uart()
{
	uart_.push_back(0);
	uart_.push_back(2);
	uart_.push_back(forward_);
	uart_.push_back(turn_);
	length_ += uart_.size();
}


/** コンストラクタ
*/
AccelerationRequest::AccelerationRequest()
{
	cmd_ = 'a';
}


static AccelerationRequest badAccelerationRequest;
/** コンストラクタ
*/
AccelerationResponse::AccelerationResponse()
:Response({},badAccelerationRequest)
,parse_(true)
,x_(0.0)
,y_(0.0)
,z_(0.0)
{
}



/** コンストラクタ
*/
AccelerationResponse::AccelerationResponse(list<unsigned char> uart,Request &req)
:Response(uart,req)
,parse_(true)
,x_(0.0)
,y_(0.0)
,z_(0.0)
{
}

/** JSONへ変換する
* @return 文字列
*/
string AccelerationResponse::JSON()
{
	return "";
}

/** X軸方向の加速度を取得する。
* @return 加速度 単位g
*/
double AccelerationResponse::x(void)
{
	this->parse();
	return x_;
}

/** Y軸方向の加速度を取得する。
* @return 加速度 単位g
*/
double AccelerationResponse::y(void)
{
	this->parse();
	return y_;
}

/** Z軸方向の加速度を取得する。
* @return 加速度 単位g
*/
double AccelerationResponse::z(void)
{
	this->parse();
	return z_;
}

void AccelerationResponse::parse()
{
	if(false == parse_)
	{
		return;
	}
	parse_ = false;
	if(false == uart_.empty())
	{
		x_ = (double)(uart_.front() -128 )/(double)32;
		uart_.pop_front();
	}
	else
	{
	}

	if(false == uart_.empty())
	{
		y_ = (double)(uart_.front() -128 )/(double)32;
		uart_.pop_front();
	}
	else
	{
	}
	
	if(false == uart_.empty())
	{
		z_ = (double)(uart_.front() -128 )/(double)32;
		uart_.pop_front();
	}
	else
	{
	}
}



/** コンストラクタ
*/
VoltageRequest::VoltageRequest()
{
	cmd_ = 'v';
}



static VoltageRequest badVoltageRequest;
/** コンストラクタ
*/
VoltageResponse::VoltageResponse()
:Response({},badVoltageRequest)
,parse_(true)
,v_(0.0)
{
}


/** コンストラクタ
*/
VoltageResponse::VoltageResponse(list<unsigned char> uart,Request &req)
:Response(uart,req)
,parse_(true)
,v_(0.0)
{
}

/** JSONへ変換する
* @return 文字列
*/
string VoltageResponse::JSON()
{
	return "";
}

/** 電源電圧を取得する。
* @return 電圧 単位 V 精度 0.1
*/
double VoltageResponse::volt(void)
{
	this->parse();
	return v_;
}


void VoltageResponse::parse()
{
	if(false == parse_)
	{
		return;
	}
	parse_ = false;
	if(false == uart_.empty())
	{
		v_ = (double)(uart_.front())/(double)10;
		uart_.pop_front();
	}
	else
	{
	}

}





/** コンストラクタ
*/
RawRequest::RawRequest()
:Request()
,autoHead_(false)
{
	cmd_ = '?';
}


/** Operatorを入れる
* @return None
*/
void RawRequest::op(unsigned char oper)
{
	autoHead_ = true;
	cmd_ = oper;
}

/** バイナリを入れる
* @return None
*/
void RawRequest::bin(unsigned char data)
{
	uart_.push_back(data);
}


/** バイナリを入れる
* @return None
*/
void RawRequest::bin(vector<unsigned char> data)
{
	for(auto x : data)
	{
		uart_.push_back(x);
	}
}
/** UARTコマンドに変更する。
* @return None
*/
void RawRequest::uart()
{
	length_ += uart_.size();
}



/** UART共通の部分を生成する
* @param None
* @return None
*/
void RawRequest::uartCommon()
{
	if(autoHead_)
	{
		uart_.push_front(length_);
		uart_.push_front(cmd_);
		if(useuid_)
		{
			auto uid2 = (unsigned char)(uidCounter_%16) | 0x80;
			uart_.push_front(uid2);
			uid_ = uid2;
			auto uid1 = (unsigned char)((uidCounter_>>4)%16) | 0x80;
			uart_.push_front( uid1);
			uid_ |= (uid1 << 8);
			uidCounter_++;
		}
		uart_.push_front(st_);
		/// do check sum
		for(auto data : uart_)
		{
			sum_ ^= data;
		}
		uart_.push_back(sum_);
	}
	else
	{
		// do nothing but send raw data.
	}
}



static RawRequest badRawRequest;
/** コンストラクタ
*/
RawResponse::RawResponse()
:Response({},badRawRequest)
{
}

/** コンストラクタ
*/
RawResponse::RawResponse(list<unsigned char> uart,Request &req)
:Response(uart,req)
{
}




