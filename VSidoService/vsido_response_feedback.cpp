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
#include "vsido_response_feedback.hpp"
using namespace VSido;
#include <string>
#include <iostream>
using namespace std;


#include "debug.h"


/** コンストラクタ
* @param[in] uart VSidoからの返事 フィードバック
*/
ResponseFeedback::ResponseFeedback(const list<unsigned char> &uart)
:Response(uart)
,_feedbacks()
{
	_jsonObj["type"] = picojson::value("feedback");
}


/** VSidoの返事をJsonに変換する
* @param None
* @return Jsonデータ
* {
* 	"type": "feedback",
* 	"feedback":[
* 	  {
* 	    "sid":1,
* 	    "dat": 100
* 	  },
* 	  {
* 	    "sid":2,
* 	    "dat": 200
* 	  }
* 	]
* }	
*/
string ResponseFeedback::conv(void)
{
	picojson::array feedbackArray;
	for(auto &feedback :_feedbacks)
	{
		picojson::object fbObj;
		fbObj["sid"] = picojson::value((double)feedback.front());
		feedback.pop_front();

		fbObj["dat"] = picojson::value((double)feedback.front());
		feedback.pop_front();
		
		feedbackArray.push_back(picojson::value(fbObj));
	}
	_jsonObj["feedback"] = picojson::value(feedbackArray);
	return Response::conv();
}


void ResponseFeedback::splite(void)
{
	list<unsigned char> feedback;
	int i = 0;
	for(auto ch :_uart)
	{
		feedback.push_back(ch);
		if(0 == ++i%2)
		{
			_feedbacks.push_back(feedback);
			feedback.clear();
		}
	}
}


