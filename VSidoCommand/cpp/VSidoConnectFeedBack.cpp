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
FeedBackIDRequest::FeedBackIDRequest()
:sid_()
{
	cmd_ = 'f';
}

/** IDを指定する
* 複数の指定可能。
* @param sid サーボ ID 指定範囲 1～254
* @return None
*/
void FeedBackIDRequest::sid(unsigned char sid)
{
	sid_.push_back(sid);
}

/** UARTコマンドに変更する。
* @return None
*/
void FeedBackIDRequest::uart()
{
	for(const auto &sid : sid_)
	{
		uart_.push_back(sid);
	}
	length_ += uart_.size();
}



/** コンストラクタ
*/
FeedBackRequest::FeedBackRequest()
:dad_(0)
,dln_(0)
{
	cmd_ = 'r';
}

/** フィードバック情報を指定する
* @param address サーボ情報格納先の先頭アドレス
*                  指定範囲 0～53
* @param length サーボ情報読み出しデータ長
*                  指定範囲 1～54
* @return None
*/
void FeedBackRequest::feedback(unsigned char address,unsigned char length)
{
	dad_ = address;
	dln_ = length;
}


/** UARTコマンドに変更する。
* @return None
*/
void FeedBackRequest::uart()
{
	uart_.push_back(dad_);
	uart_.push_back(dln_);
	length_ += uart_.size();
}











static FeedBackRequest badFeedBackRequest;
/** コンストラクタ
*/
FeedBackResponse::FeedBackResponse()
:Response({},badFeedBackRequest)
,reqFB_(badFeedBackRequest)
,parse_(true)
,feedback_()
,_fields(servo_table)
{
	mine_ = false;
}

/** コンストラクタ
*/
FeedBackResponse::FeedBackResponse(list<unsigned char> uart,Request &req)
:Response(uart,req)
,reqFB_(dynamic_cast<FeedBackRequest &>(req))
,parse_(true)
,feedback_()
,_fields(servo_table)
{
	if('r' != op_)
	{
		mine_ = false;
	}
}

/** サーボ情報を取得する。
* @return 情報マップ 
*         0   -> 項目名
*         1   -> 項目値
*/
map<unsigned char,vector<tuple<string,int>>> FeedBackResponse::feedback(void)
{
	this->parse();
	return fb_;
}

/** JSONへ変換する
* @return 文字列
*/
string FeedBackResponse::JSON()
{
	string json;
	return json;
}


void FeedBackResponse::parse()
{
	/// filter mulity
	if(false == parse_)
	{
		return ;
	}
	parse_ = false;
	this->splite();
	for(auto &fb :feedback_)
	{
		auto sid = fb.front();
		fb.pop_front();
		auto dat = parseDAT(fb);
		DUMP_VAR_DETAILS((int)sid);
		fb_[sid] = dat;
	}
}

void FeedBackResponse::splite()
{
	auto dad = reqFB_.dad_;
	auto dln = reqFB_.dln_;
	auto width = dln;
	
	if(_iConstMaxDataLength < width  + dad)
	{
		width = _iConstMaxDataLength - dad;
	}
	// sidの分
	width++;
	
	DUMP_VAR_DETAILS((int)width);
	list<unsigned char> feedback;
	int i = 0;
	for(auto ch :uart_)
	{
		feedback.push_back(ch);
		if(0 == ++i%width)
		{
			feedback_.push_back(feedback);
			feedback.clear();
			DUMP_VAR_DETAILS((int)ch);
		}
	}
	DUMP_VAR_DETAILS(feedback_.size());
}

vector<tuple<string,int>>  FeedBackResponse::parseDAT(list<unsigned char> &data)
{
	vector<tuple<string,int>> dat;

	int offset = reqFB_.dad_;
	string name("unkown");
	list<unsigned char> elementRaw;
	map<string,list<unsigned char>> datRaw;
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
		FATAL_VAR(kv.first);
		if(1 == kv.second.size())
		{
			auto it = servo_label_table.find(kv.first);
			if(it!= servo_label_table.end())
			{
				bool unsignedFlag = std::get<2>(it->second);
				if(unsignedFlag)
				{
					value = (short)kv.second.front();
				}
				else
				{
					value = (short)((char)kv.second.front());
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


