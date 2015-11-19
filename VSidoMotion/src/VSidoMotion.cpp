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
#include "VSidoMotion.hpp"
#include "VSido.hpp"
using namespace VSido;
#include "tinyxml2.h"
using namespace tinyxml2;
#include "DanceWithMusic.hpp"
using namespace VSD_DWM;

#include <iostream>
#include <fstream>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <vector>
#include <memory>
using namespace std;

#include "debug.h"

static const string kStrRoot = "/home/sysroot/usr/share/data";


static const chrono::milliseconds iConstMotionIntervalCheck(5);
static const chrono::milliseconds iConstMotionInterval(45);
static const chrono::seconds iConstMotionIdle(1);


static const double iConstMotionIntervalAdjustRate = 1.0;


static atomic<bool> tickStartPointUpdate(false);
static chrono::steady_clock::time_point tickStartPoint;



static string strCurrentMotionID;
static map<string,vsd_dwm> gDanceMotionData;
static mutex gDMDataMutex;




static mutex mtxLoopCheckPoint;
static condition_variable cvLoopCheckPoint;

extern  string forceStrRoot;


atomic<bool> gMotionRunFlag(false);
atomic<bool> gMotionLoopFlag(false);
static const int kDLoopMargin = 3;

static const int kMaxJointNum = 5;


static void loadFrame(vsd_dwm &dwm,XMLElement *frame);
static void stopMotion(void);




void Motion::loadXml(const string &name)
{
	lock_guard<mutex> lock(gDMDataMutex);
	string path(kStrRoot);
	if(false == forceStrRoot.empty())
	{
		path = forceStrRoot;
	}
	path += "/";
	path += name;
	path += ".xml";
	
	XMLDocument xml;
	auto retXML = xml.LoadFile(path.c_str());
	DUMP_VAR_DETAILS(retXML);
	if(XML_SUCCESS == retXML)
	{
		cout << "load a new dance file." << endl;
		//xml.Print();
		cout << "read model from xml file." << endl;
		
		cout << "clear up old dance model." << endl;
		
		auto ArrayOfVsd_dwm = xml.FirstChildElement("ArrayOfVsd_dwm");
		if(ArrayOfVsd_dwm)
		{
			auto Vsd_dwm = ArrayOfVsd_dwm->FirstChildElement("vsd_dwm");
			while(Vsd_dwm)
			{
				vsd_dwm dwm;
				
				auto str_bgm_file_name = Vsd_dwm->FirstChildElement("str_bgm_file_name");
				if(str_bgm_file_name)
				{
					auto bgm = str_bgm_file_name->GetText();
					if(bgm)
					{
						DUMP_VAR_DETAILS(bgm);
						dwm.setName(string(bgm));
					}
				}
				
				auto flm_list = Vsd_dwm->FirstChildElement("flm_list");
				if(flm_list)
				{
					auto frame = flm_list->FirstChildElement("FLAMES");
					while(frame)
					{
						
						loadFrame(dwm,frame);
						frame = frame->NextSiblingElement("FLAMES");
					}
				}
				if(false == dwm.flm_list.empty())
				{
					gDanceMotionData[name] = dwm;
				}
				Vsd_dwm = Vsd_dwm->NextSiblingElement("vsd_dwm");
			}
		}
		cout << "finnish load dance model." << endl;
	}
	else
	{
		cout << "can not found file." << endl;
	}
	for(auto it = gDanceMotionData.begin();it != gDanceMotionData.end();it++)
	{
		it->second.dump();
		it->second.optimize();
		auto maxflm = it->second.flm_list.rbegin();
		if(maxflm != it->second.flm_list.rend())
		{
			maxTick_ = max(maxTick_,maxflm->first + kDLoopMargin);
		}
	}
	DUMP_VAR(gDanceMotionData.size());
}


static string execShell(string cmd)
{
    FILE* pipe = ::popen(cmd.c_str(), "r");
    if (!pipe)
	{
		return "ERROR";
	}
    char buffer[128];
    std::string result = "";
    while(!feof(pipe))
	{
    	if(fgets(buffer, 128, pipe) != NULL)
		{
    		result += buffer;
		}
    }
    pclose(pipe);
    return result;
}

#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
static vector<string> split(const string &str, char delim)
{
	istringstream iss(str); 
	string tmp; vector<string> res;
	while(getline(iss, tmp, delim))
	{ 
		res.push_back(tmp);
	}
	return res;
}


void Motion::loadXmlAll(void)
{
	string findfiles("cd ");
	string path(kStrRoot);
	if(false == forceStrRoot.empty())
	{
		path = forceStrRoot;
	}
	findfiles += path;
	findfiles += " && find -type f | grep \".xml\" | sed -e 's/\\.xml//g' | sed -e 's/\\.\\///g'";
    auto filesList = execShell(findfiles);
	auto files = split(filesList,'\n');
	for(const auto &file:files)
	{
		FATAL_VAR(file);
		loadXml(file);
	}
}


inline double modD(double val,int mod)
{
	int iVal = val;
	int iRemain = mod * (iVal/mod);
	return val - iRemain;
}


/** モーション再生スレッド本体
* @return None
*/
void Motion::operator()()
{
	while(true)
	{
		if(true == gMotionRunFlag)
		{
			if(tickStartPointUpdate)
			{
				tickStartPoint = chrono::steady_clock::now();
				tickStartPointUpdate = false;
			}
			mutex cv_m;
			unique_lock<mutex> lk(cv_m);
			condition_variable cv;
			auto now = std::chrono::system_clock::now();
			if(cv_status::timeout == cv.wait_until(lk, now + iConstMotionIntervalCheck))
			{
				/// time out 
				auto run = chrono::steady_clock ::now();
				static auto pre = run;
				auto interval = chrono::duration_cast<chrono::milliseconds>(run - pre);
				if(iConstMotionInterval < interval)
				{
					DUMP_VAR_DETAILS(interval.count());
					auto tick = chrono::duration_cast<chrono::milliseconds>(run - tickStartPoint);
					DUMP_VAR_DETAILS(tick.count());
					auto dtick = (double)tick.count()/(double)(iConstMotionInterval.count()+iConstMotionIntervalCheck.count());
					dtick *= iConstMotionIntervalAdjustRate; 
					auto dtickAll = dtick;
					dtick = modD(dtick,maxTick_);
					FATAL_VAR(dtick);
					loopMotion(dtick);
					DUMP_VAR_DETAILS(maxTick_);
					if(dtickAll > maxTick_)
					{
						tickStartPointUpdate = true;
						DUMP_VAR_DETAILS(gMotionLoopFlag);
						if(false == gMotionLoopFlag)
						{
							gMotionRunFlag = false;
						}
					}
					pre = run;
				}
			}
		}
		else
		{
			this_thread::sleep_for(iConstMotionIdle);
		}
		this_thread::yield();
		DUMP_VAR_DETAILS(gMotionRunFlag);
		unique_lock<mutex> lck(mtxLoopCheckPoint);
		cvLoopCheckPoint.notify_all();
		DUMP_VAR_DETAILS(gMotionRunFlag);
	}
}



void Motion::loopMotion(double tic)
{
	lock_guard<mutex> lock(gDMDataMutex);
	auto motion = gDanceMotionData.find(strCurrentMotionID);
	if(gDanceMotionData.end() == motion)
	{
		return;
	}
	auto &model = motion->second;
	//it->dump();
	int counter = 0;
	shared_ptr<AngleRequest> req(new AngleRequest);
	req->cycle(10);
	for(auto sid :model.valid_libms)
	{
		auto angle = model.get_angle(sid,tic);
		if(model.MAX_LIMBS_NUM <= angle )
		{
			continue;
		}
		auto it = previousJiontAngle_.find(sid);
		if(previousJiontAngle_.end() != it && it->second == angle)
		{
			// 同じ角度なら、再送しない。
			continue;
		}
		previousJiontAngle_[sid] = angle;
		FATAL_VAR(sid);
		FATAL_VAR(angle);

		req->angle(sid,angle);
		if(kMaxJointNum - 1 == counter++%kMaxJointNum)
		{
			if(*req)
			{
				req->execNA();
			}
			shared_ptr <AngleRequest> reqNew(new AngleRequest);
			req = reqNew;
			req->cycle(10);
		}
	}
	if(kMaxJointNum - 1 != counter++%kMaxJointNum)
	{
		if(*req)
		{
			req->execNA();
		}
	}
}


void Motion::start(const string &id,bool loop)
{
	tickStartPointUpdate = true;
	// 現在のモーション再生停止する。
	if(gMotionRunFlag)
	{
		this->stop();
	}
	gMotionLoopFlag = loop;
	gMotionRunFlag = true;
	FATAL_VAR(gMotionLoopFlag);
	if(false == id.empty())
	{
		lock_guard<mutex> lock(gDMDataMutex);
		strCurrentMotionID = id;
	}
	else
	{
		lock_guard<mutex> lock(gDMDataMutex);
		if(false == gDanceMotionData.empty())
		{
			strCurrentMotionID = gDanceMotionData.begin()->first;
		}
	}
	// check maxtick.
	{
		lock_guard<mutex> lock(gDMDataMutex);
		auto it = gDanceMotionData.find(strCurrentMotionID);
		auto maxflm = it->second.flm_list.rbegin();
		if(maxflm != it->second.flm_list.rend())
		{
			maxTick_ = max(maxTick_,maxflm->first + kDLoopMargin);
		}
	}
	
}
void Motion::stop(void)
{
	gMotionRunFlag = false;
	FATAL_VAR(gMotionRunFlag);
	unique_lock<mutex> lck(mtxLoopCheckPoint);
	cvLoopCheckPoint.wait(lck);
	FATAL_VAR(gMotionRunFlag);
}


/** モーションidの一覧を取得する
* @return モーションid(ファイル名)
*/
vector<string> Motion::list(void)
{
	lock_guard<mutex> lock(gDMDataMutex);
	vector<string> ids;
	for(auto & motion: gDanceMotionData)
	{
		ids.push_back(motion.first);
	}
	return ids;
}

/** モーションファイルをアップロードする。
* @param id モーションid(ファイル名)
* @param data モーションデータ
* @return None
*/
void Motion::upload(const string &id,const string &data)
{
	// write to file.
	string path(kStrRoot);
	if(false == forceStrRoot.empty())
	{
		path = forceStrRoot;
	}
	path += "/";
	path += id;
	path += ".xml";
	
	// scope over ,file closed.
	{
		ofstream ofs(path);
		ofs << data;
	}
	execShell("sync");
	loadXml(id);
}


static void loadLimb(vsd_dwm &dwm,int tic,int id,XMLElement *limb)
{
	auto flg_enable = limb->FirstChildElement("flg_enable");
	if(nullptr != flg_enable)
	{
		bool flg_enableV = false;
		if(XML_SUCCESS != flg_enable->QueryBoolText(&flg_enableV))
		{
			return;
		}
		if(false == flg_enableV)
		{
			return;
		}
	}
	auto angle = limb->FirstChildElement("dig_angle");
	double angleV = 0.;
	if(nullptr != angle)
	{
		if(XML_SUCCESS != angle->QueryDoubleText(&angleV))
		{
			return;
		}
	}
	dwm.add_Limbs(tic,id,angleV);
}


#if 0 // next stage
static void loadLimbs_IK(vsd_dwm &dwm,int tic,XMLElement *ik)
{
	int id = -1;
	if(XML_SUCCESS != ik->QueryAttribute("id",&id))
	{
		return;
	}
	DUMP_VAR_DETAILS(id);
	auto x = ik->FirstChildElement("x");
	double xV = 0.;
	if(nullptr != x)
	{
		if(XML_SUCCESS != x->QueryDoubleText(&xV))
		{
			return;
		}
	}
	DUMP_VAR_DETAILS(xV);

	auto y = ik->FirstChildElement("y");
	double yV = 0.;
	if(nullptr != y)
	{
		if(XML_SUCCESS != y->QueryDoubleText(&yV))
		{
			return;
		}
	}
	DUMP_VAR_DETAILS(yV);

	auto z = ik->FirstChildElement("z");
	double zV = 0.;
	if(nullptr != z)
	{
		if(XML_SUCCESS != z->QueryDoubleText(&zV))
		{
			return;
		}
	}
	DUMP_VAR_DETAILS(zV);
	dwm.add_Limbs_IK(tic,id,xV,yV,zV);
}


static void loadWalk(vsd_dwm &dwm,int tic,XMLElement *walk)
{
	auto speed = walk->FirstChildElement("speed");
	double speedV = 0.;
	if(nullptr != speed)
	{
		if(XML_SUCCESS != speed->QueryDoubleText(&speedV))
		{
			return;
		}
	}
	DUMP_VAR_DETAILS(speedV);

	auto turn = walk->FirstChildElement("turn");
	double turnV = 0.;
	if(nullptr != turn)
	{
		if(XML_SUCCESS != turn->QueryDoubleText(&turnV))
		{
			return;
		}
	}
	DUMP_VAR_DETAILS(turnV);
	
	dwm.add_Walk(tic,speedV,turnV);
}
#endif // next stage

static void loadFrame(vsd_dwm &dwm,XMLElement *frame)
{
	int tic = -1;
	auto ticXML = frame->FirstChildElement("tic");
	if(nullptr != ticXML)
	{
		if(XML_SUCCESS != ticXML->QueryIntText(&tic))
		{
			return;
		}
	}
	else
	{
		return;
	}
	DUMP_VAR_DETAILS(tic);
	
	auto limbArray = frame->FirstChildElement("limbs");
	DUMP_VAR_DETAILS(limbArray);
	if(limbArray)
	{
		auto limb = limbArray->FirstChildElement("LIMBS");
		int id = 1;
		DUMP_VAR_DETAILS(limb);
		while(limb)
		{
			DUMP_VAR_DETAILS(limb);
			loadLimb(dwm,tic,id,limb);
			limb = limb->NextSiblingElement("LIMBS");
			id++;
		}
	}
#if 0 // next stage
	auto limbs_ik = frame->FirstChildElement("limbs_ik");
	DUMP_VAR_DETAILS(limbs_ik);
	while(limbs_ik)
	{
		loadLimbs_IK(dwm,tic,limbs_ik);
		limbs_ik = limbs_ik->NextSiblingElement("limbs_ik");
	}

	auto walk = frame->FirstChildElement("walk");
	DUMP_VAR_DETAILS(walk);
	if(walk)
	{
		loadWalk(dwm,tic,walk);
	}

	auto ik_bottom = frame->FirstChildElement("ik_bottom");
	if(nullptr != ik_bottom)
	{
		bool ik_bottomV = false;
		if(XML_SUCCESS != ik_bottom->QueryBoolText(&ik_bottomV))
		{
			return;
		}
		DUMP_VAR_DETAILS(ik_bottomV);
		dwm.add_IK_Bottom(tic,ik_bottomV);
	}

	auto ik_top = frame->FirstChildElement("ik_top");
	if(nullptr != ik_top)
	{
		bool ik_topV = false;
		if(XML_SUCCESS != ik_top->QueryBoolText(&ik_topV))
		{
			return;
		}
		DUMP_VAR_DETAILS(ik_topV);
		dwm.add_IK_Top(tic,ik_topV);
	}
#endif
}


