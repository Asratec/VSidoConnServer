require 'json'
require_relative 'VSidoConnect'

#
# VSido Serviceと通信する
#

module VSido
	#
	# 接続する。
	# ==== Args
	# - dev デバイス名前。
	# 例："/dev/tty.USB0"。
	#
	def connect(dev = "")
		VSidoConnect::connect(dev);
	end

	#
	# 切断する。
	#
	def disConnect()
		VSidoConnect::disConnect();
	end
	
	module_function :connect;
	module_function :disConnect;
	
	#
	# 目標角度設定。
	#
	class AngleRequest
		#
		# コンストラクタ。
		#
		def initialize()
			@angle = VSidoConnect::AngleRequest.new();
		end
		
		#
		# 目標角度に移行するまでの時間を設定する。
		# 後に設定した値だけ有効。
		# ==== Args
		# - cyc 指定範囲 1～100 （単位は 10msec）。
		#
		def cycle(cyc)
			@angle.cycle(cyc);
		end

		#
		# 目標角度を指定する。
		# 複数の指定可能。
		# ==== Args
		# - sid サーボ ID 指定範囲 1～254。
		# - deg 目標角度 指定範囲 -1800～1800 （0.1deg 刻み）。
		#
		def angle(sid,deg)
			@angle.angle(sid,deg);
		end
		
		#
		# 結果を返すように、コマンドを実行する。
		# ==== Return
		# JSON
		def execResult()
			ack = VSidoConnect::execWithJsonResult(@angle);
			return ack;
		end
	end
	
	#
	# サーボ情報要求。
	#
	class ServoInfoRequest
		#
		# コンストラクタ。
		#
		def initialize()
			@servo = VSidoConnect::ServoInfoRequest.new();
		end

		#
		# 取得情報を指定する。
		# 複数の指定可能。
		# ==== Args
		# - sid サーボ ID 指定範囲 1～254。
		# - address サーボ情報格納先の先頭アドレス。
		#                  指定範囲 0～53。
		# - length サーボ情報読み出しデータ長。
		#                  指定範囲 1～54。
		#
		def info(sid,address,length)
			@servo.info(sid,address,length);
		end

		#
		# 結果を返すように、コマンドを実行する。
		# ==== Args
		# JSON
		#
		def execResult()
			ack = VSidoConnect::execWithJsonResult(@servo);
			return ack;
		end
	end
end

