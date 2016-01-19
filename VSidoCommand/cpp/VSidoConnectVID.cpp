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
GetVIDRequest::GetVIDRequest()
:fields_(vid_table)
,ids_()
{
	cmd_ = 'g';
}

/** vidを指定する
* 複数の指定可能。
* @param id 設定値 ID（5 バイト目以降、必要 ID 分設定を行うこと）
*			 指定範囲 0～23 （RC 版での仕様。今後拡張される予定）
* @return None
*/
void GetVIDRequest::vid(unsigned char id)
{
	if(checkVid(id))
	{
		ids_.push_back(id);
	}
}

/** vidを指定する
* 複数の指定可能。
* @param label vid_label_tableを参照してください。
* @return None
*/
void GetVIDRequest::vid(string label)
{
	auto it = vid_label_table.find(label);
	if(it != vid_label_table.end())
	{
		auto id = (unsigned char)get<0>(it->second);
		ids_.push_back(id);
	}
}



bool GetVIDRequest::checkVid(unsigned char id)
{
	if(fields_.end() == fields_.find(id))
	{
		return false;
	}
	return true;
}


/** UARTコマンドに変更する。
* @return None
*/
void GetVIDRequest::uart()
{
	for(const auto &id: ids_)
	{
		uart_.push_back(id);
		if(6 == id)
		{
			uart_.push_back(7);
		}
	}
	length_ += uart_.size();
}




static GetVIDRequest badGetVIDRequest;
/** コンストラクタ
*/
GetVIDResponse::GetVIDResponse()
:Response({},badGetVIDRequest)
,reqGetVID_(badGetVIDRequest)
,fields_(vid_table)
,parse_(true)
,vars_()
,splite_()
{
}

/** コンストラクタ
*/
GetVIDResponse::GetVIDResponse(list<unsigned char> uart,Request &req)
:Response(uart,req)
,reqGetVID_(dynamic_cast<GetVIDRequest &>(req))
,fields_(vid_table)
,parse_(true)
,vars_()
,splite_()
{
	if('g' != op_)
	{
		mine_ = false;
	}
}

/** JSONへ変換する
* @return 文字列
*/
string GetVIDResponse::JSON()
{
	return "";
}

/** 各種変数を取得する。
* @return 情報マップ 
*         0   -> 項目名
*         1   -> 項目値
*/
map<string,int> GetVIDResponse::vars(void)
{
	this->parse();
	return vars_;
}




void GetVIDResponse::parse()
{
	/// parse one time only.
	if(false == parse_)
	{
		return ;
	}
	parse_ = false;
	this->splite();
	
	for(const auto &var :splite_)
	{
		int value;
		if(2==var.second.size())
		{
			DUMP_VAR_DETAILS((int)var.second.front());
			DUMP_VAR_DETAILS((int)var.second.back());
			auto it = vid_label_table.find(var.first);
			if(it != vid_label_table.end())
			{
				/// signed 
				if(std::get<4>(it->second))
				{
					value = Mix2BtyeU(var.second.front(),var.second.back());
				}
				else
				{
					value = Mix2Btye(var.second.front(),var.second.back());
				}
			}
		}
		else
		{
			DUMP_VAR_DETAILS((int)var.second.front());
			auto it = vid_label_table.find(var.first);
			if(it != vid_label_table.end())
			{
				/// signed 
				if(std::get<4>(it->second))
				{
					value = (int)var.second.front();
				}
				else
				{
					value = (int)((char)var.second.front());
				}
			}
		}
		DUMP_VAR_DETAILS(var.first);
		DUMP_VAR_DETAILS(value);
		vars_[var.first] = value;
	}
}

void GetVIDResponse::splite()
{
	auto vids = reqGetVID_.ids_;
	DUMP_VAR_DETAILS(vids.size());
	vector<unsigned char> var;
	int i = 0;
	bool lowByte = false;
	string name("unkown");
	DUMP_VAR_DETAILS(uart_.size());
	for(auto vdt :uart_)
	{
		if(lowByte)
		{
			lowByte = false;
			var.push_back(vdt);
			if("unkown" != name)
			{
				splite_[name]= var;
			}
			DUMP_VAR_DETAILS(var.size());
			var.clear();
			continue;
		}
		
		if(vids.size() > i )
		{
			auto vid = vids.at(i++);
			auto it = fields_.find(vid);
			if(fields_.end() != it)
			{
				name = std::get<0>(it->second);
				auto size = std::get<1>(it->second);
				DUMP_VAR_DETAILS(size);
				if(size == 2)
				{
					var.push_back(vdt);
					lowByte = true;
				}
				else
				{
					var.push_back(vdt);
					if("unkown" != name)
					{
						splite_[name]= var;
					}
					DUMP_VAR_DETAILS(name);
					DUMP_VAR_DETAILS((int)vdt);
					DUMP_VAR_DETAILS(var.size());
					var.clear();
				}
			}
			else
			{
				FATAL_VAR( ((int)vid) );
				FATAL_VAR("error !!!");
				bad_format_ = false;
			}
		}
	}
	DUMP_VAR_DETAILS(splite_.size());
}



/** コンストラクタ
*/
SetVIDRequest::SetVIDRequest()
:fields_(vid_table)
,vars_()
,vars2B_()
{
	cmd_ = 's';
}

/** １バイト値を設定する
* 複数の指定可能。
* @param id 設定値 ID（5 バイト目以降、必要 ID 分設定を行うこと）
*			 指定範囲 0～23 （RC 版での仕様。今後拡張される予定）
* @param val 1 byte設定値
* @return None
*/
void SetVIDRequest::value(unsigned char id,unsigned char val)
{
	if(checkVid(id,val))
	{
		vars_.push_back(make_tuple(id,val));
	}
}

/** １バイト値を設定する
* 複数の指定可能。
* @param label vid_label_tableを参照してください。
* @param val 1 byte設定値
* @return None
*/
void SetVIDRequest::value(string label,unsigned char val)
{
	auto it = vid_label_table.find(label);
	if(it != vid_label_table.end())
	{
		auto id = (unsigned char)get<0>(it->second);
		vars_.push_back(make_tuple(id,val));
	}
}



/** ２バイト値を設定する
* 複数の指定可能。
* @param id 設定値 ID（5 バイト目以降、必要 ID 分設定を行うこと）
*			 指定範囲 0～23 （RC 版での仕様。今後拡張される予定）
* @param val ２ byte設定値
* @return None
*/
void SetVIDRequest::value2B(unsigned char id,unsigned short val)
{
	if(checkVid(id,val))
	{
		vars2B_.push_back(make_tuple(id,val));
	}
}

/** ２バイト値を設定する
* 複数の指定可能。
* @param label vid_label_tableを参照してください。
* @param val ２ byte設定値
* @return None
*/
void SetVIDRequest::value2B(string label,unsigned short val)
{
	auto it = vid_label_table.find(label);
	if(it != vid_label_table.end())
	{
		auto id = (unsigned char)get<0>(it->second);
		vars2B_.push_back(make_tuple(id,val));
	}
}


/** UARTコマンドに変更する。
* @return None
*/
void SetVIDRequest::uart()
{
	for(const auto &var: vars_)
	{
		auto vid = get<0>(var);
		uart_.push_back(vid);
		auto val = get<1>(var);
		uart_.push_back(val);
	}
	for(const auto &var: vars2B_)
	{
		auto vid = get<0>(var);
		auto val = get<1>(var);
		convert2ByteVid(vid,val);
	}
	length_ += uart_.size();
}

template <typename T>bool SetVIDRequest::checkVid(unsigned char id,T val)
{
	const auto it = fields_.find(id);
	if(fields_.end() != it)
	{
		const auto min = std::get<2>(it->second);
		const auto max = std::get<3>(it->second);
		if(min > val || max < val)
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}


