/*
Copyright (c) 2015, Asratec Corp.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  vector of conditions and the following disclaimer.

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
#include "vsido_vid_table.hpp"
#include "vsido_servo_info_table.hpp"
#include "VSidoConnectInner.hpp"
using namespace VSido;
#include "debug.h"


#include <iostream>
using namespace std;



/** コンストラクタ
*/
IKSetRequest::IKSetRequest()
:pos_(false)
,torque_(false)
,rot_(false)
,kdt_()
{
	cmd_ = 'k';
}

/** 位置情報ありかを調べる
* @param pos true あり、false なし
* @return なし
*/
void IKSetRequest::position(bool pos)
{
	pos_ = pos;
}

/** トルク情報ありかを調べる
* @param tor true あり、false なし
* @return なし
*/
void IKSetRequest::torque(bool tor)
{
	torque_ = tor;
}

/** 姿勢情報ありかを調べる
* @param rot true あり、false なし
* @return なし
*/
void IKSetRequest::rotation(bool rot)
{
	rot_ = rot;
}

/** 位置データを設定する
* 複数の指定可能。
* @param kid 体のパーツID
* KID   0 1 2 3 4 5 6 7
* 項目 体幹 頭部 右手 左手 右足 左足 予約 予約
* KID   8 9 10 11 12 13 14 15
* 項目 予約 予約 予約 予約 予約 予約 予約 予約
* @return なし
* @param x X軸 可動範囲% -100%~100%
* @param y Y軸 可動範囲% -100%~100%
* @param z Z軸 可動範囲% -100%~100%
*/
void IKSetRequest::position(unsigned char kid,signed char x,signed char y,signed char z)
{
	DUMP_VAR(kdt_.size());
	if(false == check(kid,x,y,z))
	{
		return ;
	}
	DUMP_VAR(x);
	DUMP_VAR(y);
	DUMP_VAR(z);
	auto it = kdt_.find(kid);
	if(kdt_.end() != it)
	{
		kdt_[kid] = make_tuple(
			x,y,z,
			std::get<3>(it->second),std::get<4>(it->second),std::get<5>(it->second),
			std::get<6>(it->second),std::get<7>(it->second),std::get<8>(it->second)
		);
	}
	else
	{
		DUMP_VAR((int)kid);
		kdt_[kid] = make_tuple(x,y,z,0,0,0,0,0,0);
	}
	DUMP_VAR(kdt_.size());
}

/** トルクデータを設定する
* 複数の指定可能。
* @param kid 体のパーツID
* KID   0 1 2 3 4 5 6 7
* 項目 体幹 頭部 右手 左手 右足 左足 予約 予約
* KID   8 9 10 11 12 13 14 15
* 項目 予約 予約 予約 予約 予約 予約 予約 予約
* @return なし
* @param x X軸 可動範囲% -100%~100%
* @param y Y軸 可動範囲% -100%~100%
* @param z Z軸 可動範囲% -100%~100%
*/
void IKSetRequest::torque(unsigned char kid,signed char x,signed char y,signed char z)
{
	DUMP_VAR(kdt_.size());
	if(false == check(kid,x,y,z))
	{
		return ;
	}
	DUMP_VAR(x);
	DUMP_VAR(y);
	DUMP_VAR(z);
	auto it = kdt_.find(kid);
	if(kdt_.end() != it)
	{
		kdt_[kid] = make_tuple(
			std::get<0>(it->second),std::get<1>(it->second),std::get<2>(it->second),
			std::get<3>(it->second),std::get<4>(it->second),std::get<5>(it->second),
			x,y,z
		);
	}
	else
	{
		DUMP_VAR((int)kid);
		kdt_[kid] = make_tuple(0,0,0,0,0,0,x,y,z);
	}
	DUMP_VAR(kdt_.size());
}

/** 姿勢データを設定する
* 複数の指定可能。
* @param kid 体のパーツID
* KID   0 1 2 3 4 5 6 7
* 項目 体幹 頭部 右手 左手 右足 左足 予約 予約
* KID   8 9 10 11 12 13 14 15
* 項目 予約 予約 予約 予約 予約 予約 予約 予約
* @return なし
* @param x X軸 可動範囲% -100%~100%
* @param y Y軸 可動範囲% -100%~100%
* @param z Z軸 可動範囲% -100%~100%
*/
void IKSetRequest::rotation(unsigned char kid,signed char x,signed char y,signed char z)
{
	DUMP_VAR(kdt_.size());
	if(false == check(kid,x,y,z))
	{
		return ;
	}
	DUMP_VAR(x);
	DUMP_VAR(y);
	DUMP_VAR(z);
	auto it = kdt_.find(kid);
	if(kdt_.end() != it)
	{
		kdt_[kid] = make_tuple(
			std::get<0>(it->second),std::get<1>(it->second),std::get<2>(it->second),
			x,y,z,
			std::get<6>(it->second),std::get<7>(it->second),std::get<8>(it->second)
		);
	}
	else
	{
		DUMP_VAR((int)kid);
		kdt_[kid] = make_tuple(0,0,0,x,y,z,0,0,0);
	}
	DUMP_VAR(kdt_.size());
}

/** UARTコマンドに変更する。
* @return None
*/
void IKSetRequest::uart()
{
	unsigned char ikflag = 0;
	if(torque_)
	{
		ikflag |=  0x04;
	}
	if(rot_)
	{
		ikflag |=  0x02;
	}
	if(pos_)
	{
		ikflag |=  0x01;
	}
	DUMP_VAR(ikflag);
	uart_.push_back(ikflag);
	DUMP_VAR(pos_);
	DUMP_VAR(rot_);
	DUMP_VAR(torque_);
	DUMP_VAR(kdt_.size());
	for(const auto &k: kdt_)
	{
		uart_.push_back((unsigned char)k.first);
		if(pos_)
		{
			int x = (int) std::get<0>(k.second)  + 100;
			DUMP_VAR(x);
			uart_.push_back((unsigned char)x);
			int y = (int) std::get<1>(k.second) + 100;
			DUMP_VAR(y);
			uart_.push_back((unsigned char)y);
			int z = (int) std::get<2>(k.second) + 100;
			DUMP_VAR(z);
			uart_.push_back((unsigned char)z);
		}
		if(rot_)
		{
			int x = (int) std::get<3>(k.second)  + 100;
			DUMP_VAR(x);
			uart_.push_back((unsigned char)x);
			int y = (int) std::get<4>(k.second) + 100;
			DUMP_VAR(y);
			uart_.push_back((unsigned char)y);
			int z = (int) std::get<5>(k.second) + 100;
			DUMP_VAR(z);
			uart_.push_back((unsigned char)z);
		}

		if(torque_)
		{
			int x = (int) std::get<6>(k.second)  + 100;
			DUMP_VAR(x);
			uart_.push_back((unsigned char)x);
			int y = (int) std::get<7>(k.second) + 100;
			DUMP_VAR(y);
			uart_.push_back((unsigned char)y);
			int z = (int) std::get<8>(k.second) + 100;
			DUMP_VAR(z);
			uart_.push_back((unsigned char)z);
		}
	}
	length_ += uart_.size();
}


bool IKSetRequest::check(unsigned char kid,signed char x,signed char y,signed char z)
{
	if(16 < kid)
	{
		return false;
	}
	if(-100 > x || 100 < x )
	{
		return false;
	}
	if(-100 > y || 100 < y )
	{
		return false;
	}
	if(-100 > z || 100 < z )
	{
		return false;
	}
	return true;
}


/** コンストラクタ
*/
IKReadRequest::IKReadRequest()
:pos_(false)
,torque_(false)
,rot_(false)
,kid_()
{
	cmd_ = 'k';
}

/** 位置情報の取得を設定する
* @param pos true あり、false なし
* @return なし
*/
void IKReadRequest::position(bool pos)
{
	pos_ = pos;
}

/** トルク情報の取得を設定する
* @param tor true あり、false なし
* @return なし
*/
void IKReadRequest::torque(bool tor)
{
	torque_ = tor;
}

/** 姿勢情報の取得を設定する
* @param rot true あり、false なし
* @return なし
*/
void IKReadRequest::rotation(bool rot)
{
	rot_ = rot;
}

/** 体のパーツIDを設定する
* 複数の指定可能。
* @param kid 体のパーツID
* KID   0 1 2 3 4 5 6 7
* 項目 体幹 頭部 右手 左手 右足 左足 予約 予約
* KID   8 9 10 11 12 13 14 15
* 項目 予約 予約 予約 予約 予約 予約 予約 予約
* @return なし
*/
void IKReadRequest::kid(unsigned char kid)
{
	if(16 < kid)
	{
		return;
	}
	kid_.push_back(kid);
}
/** UARTコマンドに変更する。
* @return None
*/
void IKReadRequest::uart()
{
	unsigned char ikflag = 0;
	if(torque_)
	{
		ikflag |=  0x20;
	}
	if(rot_)
	{
		ikflag |=  0x10;
	}
	if(pos_)
	{
		ikflag |=  0x08;
	}
	uart_.push_back(ikflag);
	for(const auto &id: kid_)
	{
		uart_.push_back(id);
	}
	length_ += uart_.size();
}



static IKReadRequest badIKReadRequest;
/** コンストラクタ
*/
IKResponse::IKResponse()
:Response({},badIKReadRequest)
,pos_(false)
,torque_(false)
,rot_(false)
,parse_(true)
,kdts_()
,kdt_()
{
	mine_ = false;
}


/** コンストラクタ
*/
IKResponse::IKResponse(list<unsigned char> uart,Request &req)
:Response(uart,req)
,pos_(false)
,torque_(false)
,rot_(false)
,parse_(true)
,kdts_()
,kdt_()
{
	if('k' != op_)
	{
		mine_ = false;
	}
}

/** JSONへ変換する
* @return 文字列
*/
string IKResponse::JSON()
{
	return "";
}

/** 位置情報ありかを調べる
* @return true あり、false なし
*/
bool IKResponse::position()
{
	parse();
	return pos_;
}

/** トルク情報ありかを調べる
* @return true あり、false なし
*/
bool IKResponse::torque()
{
	parse();
	return torque_;
}

/** 姿勢情報ありかを調べる
* @return true あり、false なし
*/
bool IKResponse::rotation()
{
	parse();
	return rot_;
}

/** IKデータを取得する
* @return IKデータ
*/
map<unsigned char,tuple<signed char,signed char,signed char,signed char,signed char,signed char,signed char,signed char,signed char>> IKResponse::kdt()
{
	parse();
	return kdt_;
}
void IKResponse::parse()
{
	/// filter mulity
	if(false == parse_)
	{
		return ;
	}
	parse_ = false;
	parseIKF();
	spliteKDT();
	parseKDT();
}

void IKResponse::parseIKF(void)
{
	try 
	{
		if(uart_.empty())
		{
			throw string("data error");
		}
		unsigned char ikf = uart_.front();
		uart_.pop_front();

		if( 0x20 &ikf )
		{
			torque_ = true;
		}
		if( 0x10 &ikf )
		{
			rot_ = true;
		}
		if( 0x08 &ikf )
		{
			pos_ = true;
		}
	}
	catch(const string &err)
	{
		DUMP_VAR(err);
	}
}

void IKResponse::spliteKDT(void)
{
	int kdtUnit = 1; // kid 1 byte.
	if(torque_)
	{
		kdtUnit += 3; // Tx,Ty,Tz. 3byte
	}
	if(rot_)
	{
		kdtUnit += 3; // Rx,Ry,Rz. 3byte
	}
	if(pos_)
	{
		kdtUnit += 3; // x,y,z. 3byte
	}
	DUMP_VAR(kdtUnit);
	/// 最少4バイト
	if(4 > kdtUnit)
	{
		return;
	}
	list<unsigned char> kdt;
	int i = 0;
	for(auto ch :uart_)
	{
		kdt.push_back(ch);
		if(0 == ++i%kdtUnit)
		{
			kdts_.push_back(kdt);
			kdt.clear();
		}
	}
	DUMP_VAR(kdts_.size());
}


void IKResponse::parseKDT(void)
{
	try
	{
		for(auto &kdt :kdts_)
		{
			if(kdt.empty())
			{
				throw string("data error");
			}
			auto kid = kdt.front();
			kdt.pop_front();
			signed char x,y,z,Rx,Ry,Rz,Tx,Ty,Tz;
			x=y=z=Rx=Ry=Rz=Tx=Ty=Tz = 0;
			if(pos_)
			{
				if(kdt.empty())
				{
					throw string("data error");
				}
				x = (int)kdt.front() - 100;
				kdt.pop_front();
				
				if(kdt.empty())
				{
					throw string("data error");
				}
				y =  (int)kdt.front() - 100;
				kdt.pop_front();

				if(kdt.empty())
				{
					throw string("data error");
				}
				z =  (int)kdt.front() - 100;
				kdt.pop_front();
			}		
			if(rot_)
			{
				if(kdt.empty())
				{
					throw string("data error");
				}
				Rx =  (int)kdt.front() - 100;
				kdt.pop_front();

				if(kdt.empty())
				{
					throw string("data error");
				}
				Ry =  (int)kdt.front() - 100;
				kdt.pop_front();

				if(kdt.empty())
				{
					throw string("data error");
				}
				Rz =  (int)kdt.front() - 100;
				kdt.pop_front();
			}
			if(torque_)
			{
				if(kdt.empty())
				{
					throw string("data error");
				}
				Tx =  (int)kdt.front() - 100;
				kdt.pop_front();

				if(kdt.empty())
				{
					throw string("data error");
				}
				Ty =  (int)kdt.front() - 100;
				kdt.pop_front();

				if(kdt.empty())
				{
					throw string("data error");
				}
				Tz =  (int)kdt.front() - 100;
				kdt.pop_front();
			}
			kdt_[kid] = make_tuple(x,y,z,Rx,Ry,Rz,Tx,Ty,Tz);
		}
	}
	catch(const string &err)
	{
		DUMP_VAR(err);
	}
}


