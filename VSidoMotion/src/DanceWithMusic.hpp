#if 0 //# buidl error
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
#endif //# buidl error

#pragma once
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

#include "debug.h"

namespace VSD_DWM
{
// #buidl error    unsafe
    class vsd_dwm
    {
    public:
        // 定数
        const double DONT_CARE = 1000; // 関節角度値が指定されていないとき
        const int MAX_LIMBS_NUM = 256; // 最大関節数
        const int MAX_IK_NUM = 16; // IK用最大関節数

        struct LIMBS
        {
        	LIMBS():flg_enable(false),dig_angle(0.){}
            bool flg_enable; // アニメーション上で反映するかどうか
            double dig_angle; // 関節角
        };

        struct LIMBS_IK
        {
        	LIMBS_IK():x(0.),y(0.),z(0.),flg_enable(false){}
            double x;
            double y;
            double z;
            bool flg_enable; // アニメーション上で反映するかどうか
        };

        struct FLAMES
        {
            int tic;//時間 robot コマンドの更新単位、Intel Edisonでは、20ms。
            map<int,LIMBS> limbs;//関節
            bool flg_ik_bottom;//trueにすると下半身の関節角度は関節位置から決まる
            bool flg_ik_top;//trueにすると下半身の関節角度は関節位置から決まる
            bool flg_walk_enable;//歩行中フラグ（逆運動学フラグより優先される）
            double dbl_walk_speed;//歩幅変化によるスピード調整
            double dbl_walk_turn;//一歩あたりの旋回角度
            map<int,LIMBS_IK> ik_lmbs;//IKの位置情報
        };

    	vector <int> valid_libms;
        // その他
        string str_bgm_file_name;//BGMのファイル名も一緒に保持する
        map<int,FLAMES> flm_list;// tic ,frame

    public:
        vsd_dwm()
        {
            str_bgm_file_name = "";
        }
    	vsd_dwm& operator=(const vsd_dwm& rhs)
    	{
    		this->str_bgm_file_name = rhs.str_bgm_file_name;
    		this->flm_list = rhs.flm_list;
    		this->valid_libms = rhs.valid_libms;
    		return *this;
    	}
    	
    	void clear()
    	{
    		flm_list.clear();
    		str_bgm_file_name = "";
    	}
    	void setName(string name)
    	{
    		str_bgm_file_name = name;
    	}
        double get_angle(int int_limb_id, double tic)
        {
            double dtic = std::max(0., tic);
        	auto cnt_before = flm_list.end();
            auto cnt_after = flm_list.end();
            for (auto cnt = flm_list.begin(); cnt != flm_list.end(); cnt++)
            {
                if (cnt->second.limbs[int_limb_id].flg_enable)
                {
                    if (cnt->second.tic >= dtic)
                    {
                        cnt_after = cnt;
                        break;
                    }
                    cnt_before = cnt;
                }
            }

            if (cnt_before == flm_list.end() && cnt_after == flm_list.end() )//前後共に見つからなかった場合
                return DONT_CARE;

            if (cnt_before == flm_list.end())//後しか見つからなかった場合
                return cnt_after->second.limbs[int_limb_id].dig_angle;

            if (cnt_after == flm_list.end())//前しか見つからなかった場合
                return cnt_before->second.limbs[int_limb_id].dig_angle;

            double tic_before = cnt_before->second.tic;
            double tic_after = cnt_after->second.tic;
            double agl_before = cnt_before->second.limbs[int_limb_id].dig_angle;
            double agl_after = cnt_after->second.limbs[int_limb_id].dig_angle;

            return
                ( (tic_after - dtic) * agl_before
                + (dtic - tic_before) * agl_after
                ) / ((double)(tic_after - tic_before));

        }

        void add_Limbs(int tic, int joint_id , double dig_angle)
        {
        	auto it = flm_list.find(tic);
        	if(flm_list.end() == it)
        	{
            	add_Flame(tic);
        		it = flm_list.find(tic);
        	}
        	if(flm_list.end() == it)
        	{
        		// map inster error.
        		cout << "map inster error.memory full?" << endl;
        		return;
        	}
            it->second.limbs[joint_id].dig_angle = dig_angle;
            it->second.limbs[joint_id].flg_enable = true;
        }

        void delete_limbs(int tic,int int_limb_id)
        {
        	auto it = flm_list.find(tic);
        	if(flm_list.end() != it)
        	{
        		auto it2 = it->second.limbs.find(int_limb_id);
        		if(it->second.limbs.end() != it2)
        		{
        			it2->second.flg_enable = false;
        		}
        	}
        }

    	bool get_state(int tic, int int_limb_id = -1)
        {
            if (int_limb_id < 0)
            {
                return true;
            }
        	auto it = flm_list.find(tic);
        	if(flm_list.end() != it)
        	{
        		auto it2 = it->second.limbs.find(int_limb_id);
        		if(it->second.limbs.end() != it2)
        		{
        			return it2->second.flg_enable;
        		}
        	}
        	//検索失敗
        	//完全一致なし
            return false;
        }


    	void add_Limbs_IK(int tic, int ik_id , double x,double y,double z)
        {
        	auto it = flm_list.find(tic);
        	if(flm_list.end() == it)
        	{
            	add_Flame(tic);
        		it = flm_list.find(tic);
        	}
        	if(flm_list.end() == it)
        	{
        		// map inster error.
        		cout << "map inster error.memory full?" << endl;
        	}
            it->second.ik_lmbs[ik_id].x = x;
            it->second.ik_lmbs[ik_id].y = y;
            it->second.ik_lmbs[ik_id].z = z;
            it->second.ik_lmbs[ik_id].flg_enable = true;
        }

        void delete_limbs_IK(int tic,int int_ik_id)
        {
        	auto it = flm_list.find(tic);
        	if(flm_list.end() != it)
        	{
        		auto it2 = it->second.ik_lmbs.find(int_ik_id);
        		if(it->second.ik_lmbs.end() != it2)
        		{
        			it2->second.flg_enable = false;
        		}
        	}
        }
    	
    	
    	void add_Walk(int tic, double speed,double turn)
        {
        	auto it = flm_list.find(tic);
        	if(flm_list.end() == it)
        	{
            	add_Flame(tic);
        		it = flm_list.find(tic);
        	}
        	if(flm_list.end() == it)
        	{
        		// map inster error.
        		cout << "map inster error.memory full?" << endl;
        	}
            it->second.flg_walk_enable = true;
            it->second.dbl_walk_speed = speed;
            it->second.dbl_walk_turn = turn;
        }

        void delete_Walk(int tic)
        {
        	auto it = flm_list.find(tic);
        	if(flm_list.end() != it)
        	{
	            it->second.flg_walk_enable = false;
	            it->second.dbl_walk_speed = 0.;
	            it->second.dbl_walk_turn = 0.;
        	}
        }
    	
    	void add_IK_Bottom(int tic, bool flag)
        {
        	auto it = flm_list.find(tic);
        	if(flm_list.end() == it)
        	{
            	add_Flame(tic);
        		it = flm_list.find(tic);
        	}
        	if(flm_list.end() == it)
        	{
        		// map inster error.
        		cout << "map inster error.memory full?" << endl;
        	}
            it->second.flg_ik_bottom = flag;
        }
    	
    	void add_IK_Top(int tic, bool flag)
        {
        	auto it = flm_list.find(tic);
        	if(flm_list.end() == it)
        	{
            	add_Flame(tic);
        		it = flm_list.find(tic);
        	}
        	if(flm_list.end() == it)
        	{
        		// map inster error.
        		cout << "map inster error.memory full?" << endl;
        	}
            it->second.flg_ik_top = flag;
        }
    	
    	void dump()
    	{
    		DUMP_VAR_DETAILS( flm_list.size());
    		for(const auto frame:flm_list)
    		{
    			DUMP_VAR_DETAILS( frame.first);
    			DUMP_VAR_DETAILS( frame.second.limbs.size());
    			for(const auto Limb:frame.second.limbs)
    			{
    				DUMP_VAR_DETAILS( Limb.first);
    				DUMP_VAR_DETAILS( Limb.second.flg_enable);
    				DUMP_VAR_DETAILS( Limb.second.dig_angle);
    			}
    		}
    	}
    	void optimize()
    	{
    		for(const auto frame:flm_list)
    		{
    			for(const auto Limb:frame.second.limbs)
    			{
    				if(Limb.second.flg_enable)
    				{
    					valid_libms.push_back(Limb.first);
    				}
    			}
    		}
    		FATAL_VAR(valid_libms.size());
    		sort(valid_libms.begin(), valid_libms.end());
    		valid_libms.erase(unique(valid_libms.begin(), valid_libms.end()),valid_libms.end());
    		FATAL_VAR(valid_libms.size());
    	}
    	
    private:
        void add_Flame(int tic)//新規フレームの追加
        {
        	auto it = flm_list.find(tic);
        	if(flm_list.end() != it)
        	{
        		// すでにあるかチェックする
        		it->second.tic = tic;
        	}
        	else
        	{
        		// 新規
	            FLAMES flm;
	            flm.tic = tic;
        		flm_list[tic] = flm;
        	}

        }

        void delete_Flame(int tic)
        {
        	auto it = flm_list.find(tic);
        	if(flm_list.end() != it)
        	{
        		flm_list.erase(it);
        	}
        }
    }; //class vsd_dwm
} //namespace VSD_DWM











