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
using namespace VSido;
#include "debug.h"






	
/** サーボIDの範囲チェックする。
* @param sid サーボ ID 指定範囲 1～254
* @return true 範囲内、false 範囲外
*/
inline bool checkSID(unsigned char sid)
{
	if(0 == sid || 254 < sid )
	{
		return false;
	}
	return true;
}

/** 角度の範囲チェックする。
* @param angle 指定範囲 -180.0～180.0 （精度0.1deg 刻み）
* @return true 範囲内、false 範囲外
*/
inline bool checkAngle(double angle)
{
	if(-180.0  > angle || 180.0 < angle )
	{
		return false;
	}
	return true;
}

/** コンプライアンススロープ値の範囲チェックする。
* @param cp 指定範囲 1～250 （1deg 刻み）
* @return true 範囲内、false 範囲外
*/
inline bool checkCompliance(unsigned char cp)
{
	if(1  > cp || 250 < cp )
	{
		return false;
	}
	return true;
}


#define convert2Byte(value) \
{\
	int16_t value2 = ((int16_t)value) << 1;\
	uart_.push_back(((unsigned char)value2)& 0xff);\
	int8_t value_h = ((unsigned char)(value2>>8)) & 0xff ;\
	uart_.push_back( value_h << 1);\
}

#define convert2ByteVid(vid,value) \
{\
	uart_.push_back(vid);\
	int16_t value2 = ((int16_t)value) << 1;\
	uart_.push_back(((unsigned char)value2)& 0xff);\
	int8_t value_h = ((unsigned char)(value2>>8)) & 0xff ;\
	uart_.push_back(vid+1);\
	uart_.push_back( value_h << 1);\
}


inline short Mix2Btye(unsigned char low_byte,unsigned char high_byte)
{
	short data_low = low_byte;
	short data_high = (short)((high_byte & 0x80) | (high_byte) >> 1);
	short combined_data = (short)(((data_high << 8) & 0x0000FF00) | ((data_low) & 0x000000FF));
	return  (short)((combined_data & 0x8000) | (combined_data >> 1));
}

inline unsigned short Mix2BtyeU(unsigned char low_byte,unsigned char high_byte)
{
	short data_low = low_byte;
	short data_high = (short)(high_byte >> 1);
	short combined_data = (short)(((data_high << 8) & 0x0000FF00) | ((data_low) & 0x000000FF));
	return  (short)(combined_data >> 1);
}

