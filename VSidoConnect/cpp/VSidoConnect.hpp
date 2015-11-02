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
#ifndef __VSIDO_CONNECT__
#define __VSIDO_CONNECT__


#include <string>
#include <vector>
#include <list>
#include <map>
#include <tuple>
#include <memory>
using namespace std;

/**
 * VSido Serviceと通信する
 */
namespace VSido
{
	/** V-Sido CONNECTと接続する
	* @param device V-Sido CONNECTと接続しているデバイス名。
	*                例えば、"/dev/ttyAMA0","/dev/tty.USB0"。
	*                何も入れない場合、事前に用意したデバイスリストから探し出す。
	* @return None
	*/
	void connect(const string &device = "",int baudrate = 0 ) throw(string);
    
	/** V-Sido CONNECTと切断する
	* @return None
	*/
	void disConnect(void) throw(string);
	
	class Request;
	
	class Response;
	class AckResponse;
	class ServoInfoResponse;
	class FeedBackResponse;
	class GetVIDResponse;
	class JointResponse;
	class IKResponse;
	class AccelerationResponse;
	class VoltageResponse;
	class RawResponse;

	
	/**
	* VSido CONNECTへ要求コマンド実行
	*/
	template <typename T> class ExecRequest
	{
	public:
		/**
		* 要求コマンドを実行する
		* @return VSido CONNECTから返事クラス
		*/
		T exec();

		/**
		* 要求コマンドを実行する(No Ack)
		* @return None
		*/
		void execNA();
	protected:
	};
	
	/**
	* VSido CONNECTへ要求コマンド
	*/
	class Request
	{
	public:
		template <typename T> friend class ExecRequest;

		/** コマンドが正しいか調べる
		* @return true 正しい、false 正しくない。
		*/
		operator bool (){return good_;}

		/** コマンドが正しくない場合、ヒント情報を取得する
		* @return 文字列
		*/
		string msg(){return msg_;}
	protected:
		/** コンストラクタ
		*/
		Request();

		/** UARTコマンドに変更する。
		* @return None
		*/
		virtual void uart(){}

		/** UART共通の部分を生成する
		* @return None
		*/
		virtual void uartCommon();

		friend class Response;
	private:
		
	protected:
		list<unsigned char> uart_;
		unsigned char st_;
		unsigned char cmd_;
		unsigned char length_;
		unsigned char sum_;
		bool good_;
		string msg_;
		
	public:
		static unsigned int uidCounter_;
		static const bool uid_ = false;
	};
	
	/**
	* VSido CONNECTから返事
	*/
	class Response
	{
	public:
		/** JSONへ変換する
		* @return 文字列
		*/
		virtual string JSON();

		/** 文字列へ変換する
		* @return 文字列
		*/
		explicit operator string ();

		/** response が正しいか調べる
		* @return true 正しい、false 正しくない。
		*/
		explicit operator bool ()
		{ 
			bool good = mine_ == true && bad_format_ == false;
			return good;
		}

		/** response がTimeoutか調べる
		* @return true Timeout、false Timeoutではない。
		*/
		bool timeout()
		{ 
			bool timeout_ = raw_.empty();
			return timeout_;
		}
		
	protected:
		/** コンストラクタ
		*/
		Response(list<unsigned char> uart,Request &req);

		/** デストラクタ
		*/
		virtual ~Response(){};
		
	protected:
		list<unsigned char> uart_;
		unsigned char op_;
		list<unsigned char> raw_;
		Request &req_;
		bool pair_;
		bool mine_;
		bool bad_format_;
	};

	
	
	/**
	* 要求コマンドを実行する
	* @param req
	* @return VSido CONNECTから返事クラス
	*/
	string execWithJsonResult(Request &req);

	
	/** 目標角度設定
	* 指定したサーボ ID のサーボを、指定した目標角度に移行させる。
	*/
	class AngleRequest : public Request,public ExecRequest<AckResponse>
	{
	public:
		/** コンストラクタ
		*/
		AngleRequest();
		
		/** 目標角度に移行するまでの時間を設定する
		* 後に設定した値だけ有効。
		* @param cyc 指定範囲 1～100 （単位は 10msec）
		* @return None
		*/
		void cycle(unsigned char cyc);

		/** 目標角度を指定する
		* 複数の指定可能。
		* @param sid サーボ ID 指定範囲 1～254
		* @param deg 目標角度 指定範囲 -180.0～180.0 （精度0.1deg 刻み）
		* @return None
		*/
		void angle(unsigned char,double deg);
	protected:
		/** UARTコマンドに変更する。
		* @return None
		*/
		virtual void uart();
	private:
		int cycle_;
		vector<tuple<unsigned char,double>> angle_;
	};
	/**
	* VSido CONNECTから目標角度返事
	*/
	using AngleResponse = AckResponse;

	/**
	* VSido CONNECTからACK返事
	*/
	class AckResponse :public Response
	{
	public:
		/** コンストラクタ
		*/
		AckResponse(list<unsigned char> uart,Request &req);
		
		/** コンストラクタ
		*/
		AckResponse();

		/** メッセージを取得する。
		* @return メッセージ
		*/
		string msg(void);

		/** JSONへ変換する
		* @return 文字列
		*/
		virtual string JSON();
		
		/** デストラクタ
		*/
		virtual ~AckResponse(){};
	private:
	};

	
	/** コンプライアンス設定
	* 指定サーボに対しコンプライアンスに関する設定を行う。
	*/
	class ComplianceRequest : public Request,public ExecRequest<AckResponse>
	{
	public:
		/** コンストラクタ
		*/
		ComplianceRequest();
		
		/** コンプライアンスを指定する
		* 複数の指定可能。
		* @param sid サーボ ID 指定範囲 1～254
		* @param cpCW 時計回りのコンプライアンススロープ値
		*                  指定範囲 1～250 （1deg 刻み）
		* @param cpCCW 反時計回りのコンプライアンススロープ値
		*                  指定範囲 1～250 （1deg 刻み）
		* @return None
		*/
		void comp(unsigned char sid,unsigned char cpCW,unsigned char cpCCW);

	protected:
		/** UARTコマンドに変更する。
		* @return None
		*/
		virtual void uart();
	private:
		vector<tuple<unsigned char,unsigned char,unsigned char>> comp_;
	};
	/**
	* VSido CONNECTからコンプライアンス返事
	*/
	using ComplianceResponse = AckResponse;

	
	/** 最大・最小角設定
	* サーボに対し可動範囲の最小、最大角度を指定する。
	*/
	class MinMaxAngleRequest : public Request,public ExecRequest<AckResponse>
	{
	public:
		/** コンストラクタ
		*/
		MinMaxAngleRequest();
		
		/** 最大・最小角設定を指定する
		* 複数の指定可能。
		* @param sid サーボ ID 指定範囲 1～254
		* @param min サーボ最小角度
		*                  指定範囲 -180.0～180.0 （精度0.1deg 刻み）
		* @param max サーボ最大角度
		*                  指定範囲 -180.0～180.0 （精度0.1deg 刻み）
		* @return None
		*/
		void minMax(unsigned char sid,double min,double max);
		
	protected:
		/** UARTコマンドに変更する。
		* @return None
		*/
		virtual void uart();
	private:
		vector<tuple<unsigned char,double,double>> minMax_;
	};
	/**
	* VSido CONNECTから最大・最小角返事
	*/
	using MinMaxResponse = AckResponse;
	
	/** サーボ情報要求
	* サーボの現在状態を取得する。
	*/
	class ServoInfoRequest : public Request,public ExecRequest<ServoInfoResponse>
	{
	public:
		/** コンストラクタ
		*/
		ServoInfoRequest();
		
		/** 取得情報を指定する
		* 複数の指定可能。同じsidで複数指定された場合、後勝ち。
		* @param sid サーボ ID 指定範囲 1～254
		* @param address サーボ情報格納先の先頭アドレス
		*                  指定範囲 0～53
		* @param length サーボ情報読み出しデータ長
		*                  指定範囲 1～54
		* @return None
		*/
		void info(unsigned char sid,unsigned char address,unsigned char length);
		
		friend class ServoInfoResponse;
	protected:
		/** UARTコマンドに変更する。
		* @return None
		*/
		virtual void uart();
	private:
		map<unsigned char,tuple<unsigned char, unsigned char>> info_;
	};
	/**
	* VSido CONNECTからサーボ情報返事
	*/
	class ServoInfoResponse :public Response
	{
	public:
		/** コンストラクタ
		*/
		ServoInfoResponse(list<unsigned char> uart,Request &req);

		/** コンストラクタ
		*/
		ServoInfoResponse();

		/** JSONへ変換する
		* @return 文字列
		*/
		virtual string JSON();

		/** サーボ情報を取得する。
		* @return 情報マップ 
		*         0   -> 項目名
		*         1   -> 項目値
		*/
		map<unsigned char,vector<tuple<string,int>>> info(void);

		/** サーボ情報を取得する。（サーボごと）
		* @return 情報マップ 
		*         0   -> 項目名
		*         1   -> 項目値
		*/
		template <typename Function> void forEachServo(Function f)
		{
			this->parse();
			for(auto& kv:info_)
			{
				f(kv.first,kv.second);
			}
		}
		
		/** サーボ情報を取得する。(項目ごと)
		* @return 情報マップ 
		*         0   -> 項目名
		*         1   -> 項目値
		*/
		template <typename Function> void forEachItem(Function f)
		{
			this->parse();
			for(auto& kv:info_)
			{
				for(auto& ir: kv.second)
				{
					f(kv.first,std::get<0>(ir),std::get<1>(ir));
				}
			}
		}

		/** デストラクタ
		*/
		virtual ~ServoInfoResponse(){};

	private:
		ServoInfoRequest &reqInfo_;
		map<unsigned char,vector<tuple<string,int>>> info_;
		bool parse_;
		void parse();
		void splite();
		vector<tuple<string,int>> parseDAT(unsigned char sid,list<unsigned char> &data);

		
		const unsigned int _iConstMaxDataLength = 54;
		vector<list<unsigned char>> _servos;
		const map<int,tuple<string,bool>> _fields;
	};

	

	/** フィードバックID 設定
	* サーボに関するフィードバックID を設定する。
	*/
	class FeedBackIDRequest : public Request,public ExecRequest<AckResponse>
	{
	public:
		/** コンストラクタ
		*/
		FeedBackIDRequest();
		
		/** IDを指定する
		* 複数の指定可能。
		* @param sid サーボ ID 指定範囲 1～254
		* @return None
		*/
		void sid(unsigned char sid);
		
	protected:
		/** UARTコマンドに変更する。
		* @return None
		*/
		virtual void uart();
	private:
		vector<unsigned char> sid_;
	};
	/**
	* VSido CONNECTからフィードバックID返事
	*/
	using FeedBackIDResponse = AckResponse;
	
	
	/** フィードバック要求
	* フィードバック ID 設定コマンド（前項参照）で登録したサーボ ID のサーボ情報を取得する。
	*/
	class FeedBackRequest : public Request,public ExecRequest<FeedBackResponse>
	{
	public:
		/** コンストラクタ
		*/
		FeedBackRequest();
		
		/** フィードバック情報を指定する
		* @param address サーボ情報格納先の先頭アドレス
		*                  指定範囲 0～53
		* @param length サーボ情報読み出しデータ長
		*                  指定範囲 1～54
		* @return None
		*/
		void feedback(unsigned char address,unsigned char length);
		
		
		friend class FeedBackResponse;
	protected:
		/** UARTコマンドに変更する。
		* @return None
		*/
		virtual void uart();
	private:
		unsigned char dad_;
		unsigned char dln_;
	};
	/**
	* VSido CONNECTからフィードバック返事
	*/
	class FeedBackResponse :public Response
	{
	public:
		/** コンストラクタ
		*/
		FeedBackResponse(list<unsigned char> uart,Request &req);

		/** コンストラクタ
		*/
		FeedBackResponse();

		/** JSONへ変換する
		* @return 文字列
		*/
		virtual string JSON();

		/** フィードバックを取得する。
		* @return 情報マップ 
		*         0   -> 項目名
		*         1   -> 項目値
		*/
		map<unsigned char,vector<tuple<string,int>>> feedback(void);

		/** フィードバックを取得する。（サーボごと）
		* @return 情報マップ 
		*         0   -> 項目名
		*         1   -> 項目値
		*/
		template <typename Function> void forEachServo(Function f)
		{
			this->parse();
			for(auto& kv:fb_)
			{
				f(kv.first,kv.second);
			}
		}
		
		/** フィードバックを取得する。(項目ごと)
		* @return 情報マップ 
		*         0   -> 項目名
		*         1   -> 項目値
		*/
		template <typename Function> void forEachItem(Function f)
		{
			this->parse();
			for(auto& kv:fb_)
			{
				for(auto& ir: kv.second)
				{
					f(kv.first,std::get<0>(ir),std::get<1>(ir));
				}
			}
		}

		/** デストラクタ
		*/
		virtual ~FeedBackResponse(){};
	private:
		FeedBackRequest &reqFB_;
		map<unsigned char,vector<tuple<string,int>>> fb_;
		bool parse_;
		void parse();
		void splite();
		vector<tuple<string,int>> parseDAT(list<unsigned char> &data);
		
		const unsigned int _iConstMaxDataLength = 54;
		vector<list<unsigned char>> feedback_;
		const map<int,tuple<string,bool>> _fields;
	};

	

	/**  各種変数（VID）設定 
	* VID（設定値）を設定する。
	*/
	class SetVIDRequest : public Request,public ExecRequest<AckResponse>
	{
	public:
		/** コンストラクタ
		*/
		SetVIDRequest();
		
		/** １バイト値を設定する
		* 複数の指定可能。
		* @param id 設定値 ID（5 バイト目以降、必要 ID 分設定を行うこと）
		*			 指定範囲 0～23 （RC 版での仕様。今後拡張される予定）
		* @param val 1 byte設定値
		* @return None
		*/
		void value(unsigned char id,unsigned char val);
		
		/** １バイト値を設定する
		* 複数の指定可能。
		* @param label vid_label_tableを参照してください。
		* @param val 1 byte設定値
		* @return None
		*/
		void value(string label,unsigned char val);

		/** ２バイト値を設定する
		* 複数の指定可能。
		* @param id 設定値 ID（5 バイト目以降、必要 ID 分設定を行うこと）
		*			 指定範囲 0～23 （RC 版での仕様。今後拡張される予定）
		* @param val ２ byte設定値
		* @return None
		*/
		void value2B(unsigned char id,unsigned short val);

		/** ２バイト値を設定する
		* 複数の指定可能。
		* @param label vid_label_tableを参照してください。
		* @param val ２ byte設定値
		* @return None
		*/
		void value2B(string label,unsigned short val);
		
	protected:
		/** UARTコマンドに変更する。
		* @return None
		*/
		virtual void uart();
	private:
		template <typename T>bool checkVid(unsigned char id,T val);
	private:
		const map<int,tuple<string,int,int,int,bool>> fields_;
		vector<tuple<unsigned char,unsigned char>> vars_;
		vector<tuple<unsigned char,unsigned short>> vars2B_;
	};
	/**
	* VSido CONNECTから各種変数（VID）返事
	*/
	using SetVIDResponse = AckResponse;
	
	/** 各種変数（VID）要求
	* VID（設定値）を要求する。
	*/
	class GetVIDRequest : public Request,public ExecRequest<GetVIDResponse>
	{
	public:
		/** コンストラクタ
		*/
		GetVIDRequest();
		
		/** vidを指定する
		* 複数の指定可能。
		* @param id 設定値 ID（5 バイト目以降、必要 ID 分設定を行うこと）
		*			 指定範囲 0～23 （RC 版での仕様。今後拡張される予定）
		* @return None
		*/
		void vid(unsigned char id);
		
		/** vidを指定する
		* 複数の指定可能。
		* @param label vid_label_tableを参照してください。
		* @return None
		*/
		void vid(string label);

		friend class GetVIDResponse;
	protected:
		/** UARTコマンドに変更する。
		* @return None
		*/
		virtual void uart();
	private:
		bool checkVid(unsigned char id);
	private:
		const map<int,tuple<string,int,int,int,bool>> fields_;
		vector<unsigned char> ids_;
	};
	/**
	* VSido CONNECTから各種変数（VID）要求返事
	*/
	class GetVIDResponse :public Response
	{
	public:
		/** コンストラクタ
		*/
		GetVIDResponse(list<unsigned char> uart,Request &req);

		/** コンストラクタ
		*/
		GetVIDResponse();

		/** JSONへ変換する
		* @return 文字列
		*/
		virtual string JSON();

		/** 各種変数を取得する。
		* @return 情報マップ 
		*         0   -> 項目名
		*         1   -> 項目値
		*/
		map<string,int> vars(void);

		/** 各種変数を取得する。
		* @return 情報マップ 
		*         0   -> 項目名
		*         1   -> 項目値
		*/
		template <typename Function> void forEachVar(Function f)
		{
			this->parse();
			for(auto& val:vars_)
			{
				f(val.first,val.second);
			}
		}

		/** デストラクタ
		*/
		virtual ~GetVIDResponse(){};
	private:
		void parse();
		void splite();
	private:
		GetVIDRequest &reqGetVID_;
		const map<int,tuple<string,int,int,int,bool>> fields_;
		bool parse_;
		map<string,int> vars_;
		map<string,vector<unsigned char>> splite_;
	};
	
	
	/** フラッシュ書き込み
	* V-Sido CONNECT RC のフラッシュ領域に、現在の VID 設定値を書き込む。
	*/
	class WriteFlashRequest : public Request,public ExecRequest<AckResponse>
	{
	public:
		/** コンストラクタ
		*/
		WriteFlashRequest();
		
	protected:
	private:
	};
	/**
	* VSido CONNECTからフラッシュ書き込み返事
	*/
	using WriteFlashResponse = AckResponse;
	
	/** IO設定
	* GPIO 入出力ポートの設定を行う。
	*/
	class GPIORequest : public Request,public ExecRequest<AckResponse>
	{
	public:
		/** コンストラクタ
		*/
		GPIORequest();

		/** GPIOを指定する
		* 複数の指定可能。
		* @param port GPIO入出力ポート
		* 指定範囲 4～7
		* 注）V-Sido CONNECT RC の基板上に記載された PA4～PA7 に対応する。
		* @param val IOの出力内容
		* false が入力、true が出力
		* @return None
		*/
		void gpio(unsigned char port,bool val);
		
	protected:
		/** UARTコマンドに変更する。
		* @return None
		*/
		virtual void uart();
	private:
		vector<tuple<unsigned char,bool>> gpip_;
	};
	/**
	* VSido CONNECTからIO設定返事
	*/
	using GPIOResponse = AckResponse;
	
	/**  PWM設定
	* PWM 制御を行うためのポート設定を行う。
	*/
	class PWMRequest : public Request,public ExecRequest<AckResponse>
	{
	public:
		/** コンストラクタ
		*/
		PWMRequest();

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
		void pluse(unsigned char port,uint16_t width);
		
	protected:
		/** UARTコマンドに変更する。
		* @return None
		*/
		virtual void uart();
	private:
		vector<tuple<unsigned char,unsigned short>> pluse_;
	};
	/**
	* VSido CONNECTからPWM設定返事
	*/
	using PWMResponse = AckResponse;
	
	/**  接続確認要求
	* サーボモータの接続確認を行う。
	*/
	class JointRequest : public Request,public ExecRequest<JointResponse>
	{
	public:
		/** コンストラクタ
		*/
		JointRequest();
	protected:
	private:
	};

	/**
	* VSido CONNECTから各種変数（VID）要求返事
	*/
	class JointResponse :public Response
	{
	public:
		/** コンストラクタ
		*/
		JointResponse(list<unsigned char> uart,Request &req);

		/** コンストラクタ
		*/
		JointResponse();

		/** JSONへ変換する
		* @return 文字列
		*/
		virtual string JSON();

		/** サーボ接続状況を取得する
		* @return Servo TIMデータ
		*/
		vector<tuple<unsigned char,unsigned char>> tim();

		/** 各種サーボ接続状況を取得する
		* @return None
		*/
		template <typename Function> void forEach(Function f)
		{
			this->parse();
			for(auto& tim:tim_)
			{
				f(std::get<0>(tim),std::get<1>(tim));
			}
		}

		/** デストラクタ
		*/
		virtual ~JointResponse(){};
	private:
		void parse();
	private:
		vector<tuple<unsigned char,unsigned char>> tim_;
		bool parse_;
	};
	
	/** IK設定
	* IK制御を行うためのデータ送信を行う。
	*/
	class IKSetRequest : public Request,public ExecRequest<AckResponse>
	{
	public:
		/** コンストラクタ
		*/
		IKSetRequest();

		/** 位置情報ありかを調べる
		* @param pos true あり、false なし
		* @return なし
		*/
		void position(bool pos=true);

		/** 姿勢情報ありかを調べる
		* @param rot true あり、false なし
		* @return なし
		*/
		void rotation(bool rot=true);
		
		/** トルク情報ありかを調べる
		* @param tor true あり、false なし
		* @return なし
		*/
		void torque(bool tor=true);

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
		void position(unsigned char kid,signed char x,signed char y,signed char z);

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
		void rotation(unsigned char kid,signed char x,signed char y,signed char z);

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
		void torque(unsigned char kid,signed char x,signed char y,signed char z);
		
	protected:
		/** UARTコマンドに変更する。
		* @return None
		*/
		virtual void uart();
	private:
		bool check(unsigned char kid,signed char x,signed char y,signed char z);
	private:
		bool pos_;
		bool torque_;
		bool rot_;
		map<unsigned char,tuple<signed char,signed char,signed char,signed char,signed char,signed char,signed char,signed char,signed char>> kdt_;
	};
	
	/** IK設定読み込み
	* IK制御を行うためのデータ送信を行う。
	*/
	class IKReadRequest : public Request,public ExecRequest<IKResponse>
	{
	public:
		/** コンストラクタ
		*/
		IKReadRequest();

		/** 位置情報の取得を設定する
		* @param pos true あり、false なし
		* @return なし
		*/
		void position(bool pos=true);
		
		/** 姿勢情報の取得を設定する
		* @param rot true あり、false なし
		* @return なし
		*/
		void rotation(bool rot=true);

		/** トルク情報の取得を設定する
		* @param tor true あり、false なし
		* @return なし
		*/
		void torque(bool tor=true);
		
		/** 体のパーツIDを設定する
		* 複数の指定可能。
		* @param kid 体のパーツID
		* KID   0 1 2 3 4 5 6 7
		* 項目 体幹 頭部 右手 左手 右足 左足 予約 予約
		* KID   8 9 10 11 12 13 14 15
		* 項目 予約 予約 予約 予約 予約 予約 予約 予約
		* @return なし
		*/
		void kid(unsigned char kid);
	protected:
		/** UARTコマンドに変更する。
		* @return None
		*/
		virtual void uart();
	private:
		bool pos_;
		bool torque_;
		bool rot_;
		vector<unsigned char > kid_;
	};

	/**
	* VSido CONNECTからIK要求返事
	*/
	class IKResponse :public Response
	{
	public:
		/** コンストラクタ
		*/
		IKResponse(list<unsigned char> uart,Request &req);

		/** コンストラクタ
		*/
		IKResponse();

		/** JSONへ変換する
		* @return 文字列
		*/
		virtual string JSON();
		
		/** 位置情報ありかを調べる
		* @return true あり、false なし
		*/
		bool position();

		/** 姿勢情報ありかを調べる
		* @return true あり、false なし
		*/
		bool rotation();
		
		/** トルク情報ありかを調べる
		* @return true あり、false なし
		*/
		bool torque();

		/** IKデータを取得する
		* @return IKデータ
		*/
		map<unsigned char,tuple<signed char,signed char,signed char,signed char,signed char,signed char,signed char,signed char,signed char>> kdt();

		/** IKデータを取得する。
		* @return 情報マップ 
		*         0   -> 項目名
		*         1   -> 項目値
		*/
		template <typename Function> void forEach(Function f)
		{
			this->parse();
			for(auto& kv:kdt_)
			{
				f(kv.first,kv.second);
			}
		}

		/** IK位置データを取得する。
		* @return 情報マップ 
		*         0   -> 項目名
		*         1   -> 項目値
		*/
		template <typename Function> void forEachPosition(Function f)
		{
			this->parse();
			if(false==pos_)
			{
				return;
			}
			for(auto& kv:kdt_)
			{
				f(kv.first,std::get<0>(kv.second),std::get<1>(kv.second),std::get<2>(kv.second));
			}
		}

		/** IK姿勢データを取得する。
		* @return 情報マップ 
		*         0   -> 項目名
		*         1   -> 項目値
		*/
		template <typename Function> void forEachRotation(Function f)
		{
			this->parse();
			if(false==rot_)
			{
				return;
			}
			for(auto& kv:kdt_)
			{
				f(kv.first,std::get<3>(kv.second),std::get<4>(kv.second),std::get<5>(kv.second));
			}
		}

		/** IKトルクデータを取得する。
		* @return 情報マップ 
		*         0   -> 項目名
		*         1   -> 項目値
		*/
		template <typename Function> void forEachTorque(Function f)
		{
			this->parse();
			if(false==torque_)
			{
				return;
			}
			for(auto& kv:kdt_)
			{
				f(kv.first,std::get<6>(kv.second),std::get<7>(kv.second),std::get<8>(kv.second));
			}
		}
		
		/** デストラクタ
		*/
		virtual ~IKResponse(){};
	private:
		void parse();
		void parseIKF();
		void spliteKDT();
		void parseKDT();
	private:
		bool pos_;
		bool torque_;
		bool rot_;
		bool parse_;
		vector<list<unsigned char>> kdts_;
		map<unsigned char,tuple<signed char,signed char,signed char,signed char,signed char,signed char,signed char,signed char,signed char>> kdt_;
	};

	
	
	/** 移動情報指定
	* 歩行に関する情報を送信する。
	*/
	class WalkRequest : public Request,public ExecRequest<AckResponse>
	{
	public:
		/** コンストラクタ
		*/
		WalkRequest();

		/** 前後に移動する
		* @param speed 前後の速度
		* プラス前進、マイナス後退
		* @return None
		*/
		void forward(signed char speed);

		/** 旋回する
		* @param speed 旋回の速度
		* プラスCW、マイナスCCW
		* @return None
		*/
		void turn(signed char speed);

	protected:
		/** UARTコマンドに変更する。
		* @return None
		*/
		virtual void uart();
	private:
		unsigned char  forward_;
		unsigned char  turn_;
	};
	
	/** 加速度センサ値要求
	* 加速度センサの情報(X/Y/Z 軸)を取得する（RC 版では機能制限あり）。
	*/
	class AccelerationRequest : public Request,public ExecRequest<AccelerationResponse>
	{
	public:
		/** コンストラクタ
		*/
		AccelerationRequest();
	protected:
	private:
	};
	/**
	* VSido CONNECTから加速度センサの情報返事
	*/
	class AccelerationResponse :public Response
	{
	public:
		/** コンストラクタ
		*/
		AccelerationResponse(list<unsigned char> uart,Request &req);

		/** コンストラクタ
		*/
		AccelerationResponse();

		/** JSONへ変換する
		* @return 文字列
		*/
		virtual string JSON();

		/** X軸方向の加速度を取得する。
		* @return 加速度 単位g
		*/
		double x(void);

		/** Y軸方向の加速度を取得する。
		* @return 加速度 単位g
		*/
		double y(void);

		/** Z軸方向の加速度を取得する。
		* @return 加速度 単位g
		*/
		double z(void);

		/** デストラクタ
		*/
		virtual ~AccelerationResponse(){};
	private:
		void parse();
	private:
		bool parse_;
		double x_;
		double y_;
		double z_;
	};
	
	
	/** 電源電圧要求
	* 電源電圧を取得する（RC 版では機能未実装）。
	*/
	class VoltageRequest : public Request,public ExecRequest<VoltageResponse>
	{
	public:
		/** コンストラクタ
		*/
		VoltageRequest();
	protected:
	private:
	};
	/**
	* VSido CONNECTから電源電圧返事
	*/
	class VoltageResponse :public Response
	{
	public:
		/** コンストラクタ
		*/
		VoltageResponse(list<unsigned char> uart,Request &req);

		/** コンストラクタ
		*/
		VoltageResponse();

		/** JSONへ変換する
		* @return 文字列
		*/
		virtual string JSON();

		/** 電源電圧を取得する。
		* @return 電圧 単位 V 精度 0.1
		*/
		double volt(void);

		/** デストラクタ
		*/
		virtual ~VoltageResponse(){};
	private:
		void parse();
	private:
		bool parse_;
		double v_;
	};

	/** バイナリ要求
	* バイナリコマンドを取得する。
	*/
	class RawRequest : public Request,public ExecRequest<RawResponse>
	{
	public:
		/** コンストラクタ
		*/
		RawRequest();
		
		/** Operatorを入れる
		* @param oper Operator
		* @return None
		*/
		void op(unsigned char data);
		
		/** バイナリを入れる
		* @param data バイナリ
		* @return None
		*/
		void bin(unsigned char data);

		/** バイナリを入れる
		* @param data バイナリ
		* @return None
		*/
		void bin(vector<unsigned char> data);

		/** 自動的に、通信ヘッダを付ける。通信ヘッダはST,length,Checksum
		* @return None
		*/
		void autoHead(){autoHead_ = true;}

	protected:
		/** UARTコマンドに変更する。
		* @return None
		*/
		virtual void uart();

		/** UART共通の部分を生成する
		* @return None
		*/
		virtual void uartCommon();
	private:
		bool autoHead_;
	};
	/**
	* VSido CONNECTからバイナリコマンド返事
	*/
	class RawResponse :public Response
	{
	public:
		/** コンストラクタ
		*/
		RawResponse(list<unsigned char> uart,Request &req);

		/** コンストラクタ
		*/
		RawResponse();

		/** デストラクタ
		*/
		virtual ~RawResponse(){};
	private:
	private:
	};
	
	/** サーボスルーコマンド
	*  リターン返信なし 
	*/
	class ServoThrough
	{
	public:
		/** コンストラクタ
		*/
		ServoThrough();
		
		/** Binaryデータを付ける
		* @param data 送信データ
		* @return None
		*/
		void binay(list<unsigned char> data);
	};

	
	
	
	/** サーボスルーコマンド
	*   リターン返信あり
	*/
	class ServoThroughWithAck
	{
	public:
		/** コンストラクタ
		*/
		ServoThroughWithAck();

		/** Binaryデータを付ける
		* @param data 送信データ
		* @return None
		*/
		void binay(vector<unsigned char> data);

		/** 返信データ長さを設定する
		* @param leng 送信データ
		* @return None
		*/
		void expectLength(unsigned int leng);
	};
	/** サーボスルーコマンド返事
	*   リターン返信あり
	*/
	class ServoThroughResponse
	{
	public:
		/** コンストラクタ
		*/
		ServoThroughResponse();

		/** Binaryデータを受ける
		* @return 受信データ
		*/
		vector<unsigned char>binay();
	};
	

	/**  I2C スルーコマンド
	*  リターン返信なし 
	*/
	class I2CThrough
	{
	public:
		/** コンストラクタ
		*/
		I2CThrough();

		/** Binaryデータを付ける
		* @param data 送信データ
		* @return None
		*/
		void binay(vector<unsigned char> data);
	};

	
	/** I2C スルーコマンド
	*   リターン返信あり
	*/
	class I2CThroughWithAck
	{
	public:
		/** コンストラクタ
		*/
		I2CThroughWithAck();

		/** Binaryデータを付ける
		* @param data 送信データ
		* @return None
		*/
		void binay(vector<unsigned char> data);

		/** 返信データ長さを設定する
		* @param leng 送信データ
		* @return None
		*/
		void expectLength(unsigned int leng);
	};
	/** I2Cスルーコマンド返事
	*   リターン返信あり
	*/
	class I2CThroughResponse
	{
	public:
		/** コンストラクタ
		*/
		I2CThroughResponse();

		/** Binaryデータを受ける
		* @return 受信データ
		*/
		vector<unsigned char>binay();
	};
	
} // namespace VSido

#endif //__VSIDO_CONNECT__

