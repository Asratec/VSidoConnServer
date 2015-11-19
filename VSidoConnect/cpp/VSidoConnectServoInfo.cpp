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
#include "vsido_vid_table.hpp"
#include "vsido_servo_info_table.hpp"
#include "VSidoConnectInner.hpp"
using namespace VSido;
#include "debug.h"


#include <iostream>
using namespace std;


#define log_erro_param(...)




/** コンストラクタ
*/
ServoInfoRequest::ServoInfoRequest()
:info_()
{
	cmd_ = 'd';
}


/** 取得情報を指定する
* 複数の指定可能。同じsidで複数指定された場合、後勝ち。
* @param sid サーボ ID 指定範囲 1～254
* @param address サーボ情報格納先の先頭アドレス
*                  指定範囲 0～53
* @param length サーボ情報読み出しデータ長
*                  指定範囲 1～54
* @return None
*/
void ServoInfoRequest::info(unsigned char sid,unsigned char address,unsigned char length)
{
	if(false == checkSID(sid))
	{
		log_erro_param(SID);
		return ;
	}
	if(53 < address)
	{
		log_erro_param(address);
		return ;
	}
	if(1 > length|| 54 < length)
	{
		log_erro_param(length);
		return ;
	}
	auto it = info_.find(sid);
	if(info_.end() != it)
	{
		it->second = make_tuple(address,length);
	}
	else
	{
		info_[sid] = make_tuple(address,length);;
	}
}

/** UARTコマンドに変更する。
* @return None
*/
void ServoInfoRequest::uart()
{
	for(const auto &servo:info_)
	{
		auto sid = servo.first;
		uart_.push_back(sid);

		auto address = std::get<0>(servo.second);
		uart_.push_back(address);

		auto length = std::get<1>(servo.second);
		uart_.push_back(length);
	}
	length_ += uart_.size();
}









static ServoInfoRequest badServoInfoRequest;
/** コンストラクタ
*/
ServoInfoResponse::ServoInfoResponse()
:Response({},badServoInfoRequest)
,reqInfo_(badServoInfoRequest)
,parse_(true)
,_servos()
,_fields(servo_table)
{
	mine_ = false;
}


/** コンストラクタ
*/
ServoInfoResponse::ServoInfoResponse(list<unsigned char> uart,Request &req)
:Response(uart,req)
,reqInfo_(dynamic_cast<ServoInfoRequest &>(req))
,parse_(true)
,_servos()
,_fields(servo_table)
{
	DUMP_VAR(uart_.size());
	if('d' != op_)
	{
		mine_ = false;
	}
}

/** サーボ情報を取得する。
* @return 情報マップ 
*         0   -> 項目名
*         1   -> 項目値
*/
map<unsigned char,vector<tuple<string,int>>> ServoInfoResponse::info(void)
{
	this->parse();
	return info_;
}

/** JSONへ変換する
* @return 文字列
*/
string ServoInfoResponse::JSON()
{
	string json;
	return json;
}


void ServoInfoResponse::parse()
{
	/// parse one time only.
	if(false == parse_)
	{
		return ;
	}
	parse_ = false;
	
	this->splite();
	for(auto &servo :_servos)
	{
		auto sid = servo.front();
		servo.pop_front();
		auto dat = parseDAT(sid,servo);
		info_[sid] = dat;
	}
}

void ServoInfoResponse::splite()
{
	list<unsigned char> servo;
	int i = 0;
	bool newSid = true;
	int width = 0xffff;
	DUMP_VAR(uart_.size());
	for(auto ch :uart_)
	{
		DUMP_VAR_DETAILS((int)ch);
		if(true==newSid)
		{
			
			auto it= reqInfo_.info_.find(ch);
			if(it != reqInfo_.info_.end())
			{
				newSid = false;
				auto dad = std::get<0>(it->second);
				auto dln = std::get<1>(it->second);
				width = dln;
				if(_iConstMaxDataLength < width  + dad)
				{
					width = _iConstMaxDataLength - dad;
				}
				// sidの分
				width++;
				DUMP_VAR_DETAILS((int)dad);
				DUMP_VAR_DETAILS((int)dln);
				DUMP_VAR_DETAILS((int)width);
			}
			else
			{
				// delete no garbage.
				FATAL_VAR("bad data format");
				bad_format_ = false;
				continue;
			}
		}
		DUMP_VAR_DETAILS((int)width);
		servo.push_back(ch);
		if(0 == ++i% width )
		{
			DUMP_VAR(servo.size());
			_servos.push_back(servo);
			servo.clear();
			newSid = true;
			i = 0;
		}
	}
	DUMP_VAR(_servos.size());
}


vector<tuple<string,int>>  ServoInfoResponse::parseDAT(unsigned char sid,list<unsigned char> &data)
{
	vector<tuple<string,int>> dat;

	DUMP_VAR((int)sid);
	DUMP_VAR(data.size());
	auto it= reqInfo_.info_.find(sid);
	int dad = 0xffff;
	if(it != reqInfo_.info_.end())
	{
		dad = std::get<0>(it->second);
	}
	else
	{
		DUMP_VAR("error");
		return dat;
	}

	int offset = dad;
	string name("unkown");
	vector<unsigned char> elementRaw;
	map<string,vector<unsigned char>> datRaw;
	for(auto ch :data)
	{
		DUMP_VAR_DETAILS((int)ch);
		const auto field = this->_fields.find(offset++);
		if(field != this->_fields.end())
		{
			// keep old one 
			datRaw[name]= elementRaw;
			
			// for next element.
			name = std::get<0>(field->second);
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
		DUMP_VAR_DETAILS(kv.first);
		if(1 == kv.second.size())
		{
			auto it = servo_label_table.find(kv.first);
			if(it!= servo_label_table.end())
			{
				bool unsignedFlag = std::get<2>(it->second);
				if(unsignedFlag)
				{
					value = (int)kv.second.front();
				}
				else
				{
					value = (int)((char)kv.second.front());
				}
			}
		}
		else if(2 == kv.second.size())
		{
			auto it = servo_label_table.find(kv.first);
			if(it!= servo_label_table.end())
			{
				bool unsignedFlag = std::get<2>(it->second);
				if(unsignedFlag)
				{
					value = Mix2BtyeU(kv.second.front(),kv.second.back());
				}
				else
				{
					value = Mix2Btye(kv.second.front(),kv.second.back());
				}
			}
		}
		else
		{
			continue;
		}
		dat.push_back(make_tuple(kv.first,value));
	}
	data.clear();
	return dat;
}

