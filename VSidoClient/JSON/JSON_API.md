/*! \mainpage V-Sido CONNECT Client Library JSON コマンドリファレンス
\section intro 1 始めに
\n「V-Sido Server Application」を経由し、WebSocketとJSONデータを使って、ロボットを制御する方法を提示する。
\n Client Libraryにおいて、WebSocketの接続方法とJSONデータ通信フォーマットの説明を行う。
\subsection sytem 1.1 システム構成
<img src="./vscad.png" style="margin-left:40px;"></img>

\section Connet 2 WebSocket接続に関しては
<table border="1" width="300" cellspacing="0" cellpadding="5" bordercolor="#333333" style="margin-left:40px;">
<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>接続方式</td>        
<td bgcolor="#FFFFFF" valign="top" width="150">WebSocket</td>
</tr>
<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>ポート</td>        
<td bgcolor="#FFFFFF" valign="top" width="150">20080</td>
</tr>
<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>プロトコール</td>        
<td bgcolor="#FFFFFF" valign="top" width="150">vsido-cmd</td>
</tr>
<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>データフォーマット</td>        
<td bgcolor="#FFFFFF" valign="top" width="150">JSON</td>
</tr>
</table>
* 2.1 接続例(JavaScript):
\code{.js}
var ws = new WebSocket('ws://localhost:20080','vsido-cmd');
\endcode




\section cmd_list 3 コマンド一覧
<table border="1" width="800" cellspacing="0" cellpadding="5" bordercolor="#333333" style="margin-left:40px;">
<tr>
<th bgcolor="#CFCFCF">名称</th>
<th bgcolor="#CFCFCF">コマンド</th>
<th bgcolor="#CFCFCF">機能内容</th>
</tr>

<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>目標角度設定</td>        
<td bgcolor="#FFFFFF" width="150">SetServoAngle</td>
<td bgcolor="#FFFFFF" width="450">指定した目標角度に移行させる</td>
</tr>

<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>角度取得</td> 
<td bgcolor="#FFFFFF" width="150">GetServoAngle</td>
<td bgcolor="#FFFFFF" width="450">指定したサーボ ID のサーボの現在角度を取得する</td>
</tr>

<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>コンプライアンス設定</td>
<td bgcolor="#FFFFFF" width="150">SetServoCompliance</td>
<td bgcolor="#FFFFFF" width="450">コンプライアンスに関する設定を行う</td>
</tr>

<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>最大・最小角設定</td>
<td bgcolor="#FFFFFF" width="150">SetServoMinMaxAngle</td>
<td bgcolor="#FFFFFF" width="450">サーボに対し最小、最大角度を指定する</td>
</tr>

<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>サーボ情報要求</td>
<td bgcolor="#FFFFFF" width="150">GetServoInfo</td>
<td bgcolor="#FFFFFF" width="450">サーボの現在状態を取得する</td>
</tr>
<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>フィードバックID設定</td>
<td bgcolor="#FFFFFF" width="150">SetFeedbackID</td>
<td bgcolor="#FFFFFF" width="450">サーボに関するフィードバック情報を設定する</td>
</tr>

<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>フィードバック要求</td>
<td bgcolor="#FFFFFF" width="150">GetServoFeedback</td>
<td bgcolor="#FFFFFF" width="450">サーボのフィードバック情報を取得する</td>
</tr>

<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>各種変数(VID)設定</td>
<td bgcolor="#FFFFFF" width="150">SetVIDValue</td>
<td bgcolor="#FFFFFF" width="450">VID(設定値)を設定する</td>
</tr>

<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>各種変数(VID)要求</td>
<td bgcolor="#FFFFFF" width="150">GetVIDValue</td>	
<td bgcolor="#FFFFFF" width="450">VID(設定値)を取得する</td>
</tr>

<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>フラッシュ書き込み要求</td>
<td bgcolor="#FFFFFF" width="150">WriteFlash</td>
<td bgcolor="#FFFFFF" width="450">V-Sido CONNECT のフラッシュへ VID を書き込む</td>
</tr>

<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>IO設定</td>
<td bgcolor="#FFFFFF" width="150">SetGPIOValue</td>
<td bgcolor="#FFFFFF" width="450">GPIO 入出力ポートの設定を行う</td>
</tr>

<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>PWM設定</td>
<td bgcolor="#FFFFFF" width="150">SetPWMPulse</td>
<td bgcolor="#FFFFFF" width="450">PWM 制御を行うためのポート設定を行う</td>
</tr>

<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>接続確認</td>
<td bgcolor="#FFFFFF" width="150">CheckConnectedServo</td>
<td bgcolor="#FFFFFF" width="450">サーボモータの接続確認を行う</td>
</tr>

<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>IK設定</td>
<td bgcolor="#FFFFFF" width="150">SetIK</td> 
<td bgcolor="#FFFFFF" width="450">IK 制御を行うためのデータ送信を行う</td>
</tr>

<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>IK取得</td>
<td bgcolor="#FFFFFF" width="150">GetIK</td>	
<td bgcolor="#FFFFFF" width="450">IK制御を行うためのデータ取得を行う</td>
</tr>

<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>移動情報指定</td>
<td bgcolor="#FFFFFF" width="150">Walk</td>
<td bgcolor="#FFFFFF" width="450">歩行に関する情報を送信する</td>
</tr>

<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>加速度センサ値要求</td>
<td bgcolor="#FFFFFF" width="150">GetAcceleration</td>
<td bgcolor="#FFFFFF" width="450">加速度センサの情報(X/Y/Z 軸)を取得する(RC
版は機能制限あり)</td>
</tr>

<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>電源電圧要求</td>
<td bgcolor="#FFFFFF" width="150">GetVoltage</td>
<td bgcolor="#FFFFFF" width="450">電源電圧を取得する(RC では未実装)</td>
</tr>

<tr>
<td bgcolor="#FFFFCF" align="right" nowrap>状態通知</td>
<td bgcolor="#FFFFFF" width="150">StatusNotify</td>
<td bgcolor="#FFFFFF" width="450">ロボット状態変更を通知する(RC では未実装)</td>
</tr>

</table>


\section cmd_common 4 各コマンド共通
* 4.1 各コマンド共通送信JSON形式
\code{.js}
{
	"command": "...",
	...
}
\endcode
\arg command 各コマンドへ参考。
* 4.2 成功時の返事JSON形式(各コマンド共通部分)
\code{.js}
{
	"type": "...",
	"raw":"",
	...
}
\endcode
\arg type 各コマンドへ参考。
\arg raw  V-Sido CONNECTから返事のバイナリフォーマット。

* 4.3 タイムアウトの返事JSON形式(各コマンド共通)
\code{.js}
{
	"type": "timeout"
}
\endcode
* 4.4 エラー時の返事JSON形式(各コマンド共通)
\code{.js}
{
	"type": "error",
	"detail": ""  
}
\endcode
\arg detail エラーの詳細情報。


* 4.5 共通パラメータ説明
\arg 4.5.1 sid サーボID
\n     指定範囲 1～254
\n なお、サーボIDに127(0x7F)を設定した場合、接続されているRS-485サーボすべてに値を設定し、
\n 254(0xFE)を設定した場合、接続されているTTLサーボすべてに値を設定する。




\section cmd_detail 5 コマンド詳細
\subsection setangle 5.1 目標角度設定 SetServoAngle
<div style="margin-left:15px;">指定したサーボ ID のサーボを、指定した目標角度に移行させる。</div>
* 送信JSON形式
\code{.js}
{
	"command": "SetServoAngle",
	"cycle": 1~100,
	"servo":[
		{"sid":1~254,"angle":-180.0~180.0}
	]
}
\endcode

* 返事JSON形式
\code{.js}
{
	"type": "ack",
	"raw":"",
	"message":"..."
}
\endcode
\arg cycle 目標角度に移行するまでの時間
\n 指定範囲 1～100 (単位は10msec)
\arg servo サーボ情報配列
\arg sid 4.5.1へ参考。
\arg angle 目標角度
\n指定範囲 -180.0～180.0 (deg刻み)
\n注)RC版での設定値であり、正式版で変更される場合がある




\subsection getangle 5.2 角度取得 GetServoAngle
<div style="margin-left:15px;">指定したサーボ ID のサーボの現在角度を取得する。</div>
* 送信JSON形式
\code{.js}
{
	"command": "GetServoAngle",
	"servo":[
		sid(1~254)
	]
}
\endcode

* 返事JSON形式
\code{.js}
{
	"type": "GetServoAngle",
	"raw":"",
	"servo":[
		{
			"sid":1-254,
			"angle"-180.0~180.0
		}
	]
}
\endcode
\arg servo サーボ情報配列
\arg sid 4.5.1へ参考。
\arg angle 目標角度
\n指定範囲 -180.0～180.0 (deg刻み)
\n注)RC版での設定値であり、正式版で変更される場合がある




\subsection Compliance 5.3 コンプライアンス設定 SetServoCompliance
<div style="margin-left:15px;">指定サーボに対しコンプライアンスに関する設定を行う。</div>
* 送信JSON形式
\code{.js}
{
	"command": "SetServoCompliance",
	"servo":[
		{"sid":1~254,"cpCW":1~254,"cpCCW":1~254}
	]
}
\endcode

* 返事JSON形式
\code{.js}
{
	"type": "ack",
	"raw":"",
	"message":"...",
}
\endcode
\arg servo サーボ情報配列
\arg sid 4.5.1へ参考。
\arg cpCW 時計回りのコンプライアンススロープ値
\n指定範囲 1～254
\arg cpCCW 反時計回りのコンプライアンススロープ値
\n指定範囲 1～254





\subsection minMax 5.4 最大・最小角設定 SetServoMinMaxAngle
<div style="margin-left:15px;">サーボに対し可動範囲の最小、最大角度を指定する。</div>
* 送信JSON形式
\code{.js}
{
	"command": "SetServoMinMaxAngle",
	"servo":[
		{"sid":1~254,"min":-180.0~180.0,"max":-180.0~180.0}
	]
}
\endcode


* 返事JSON形式
\code{.js}
{
	"type": "ack",
	"raw":"",
	"message":"...",
}
\endcode
\arg servo サーボ情報配列
\arg sid 4.5.1へ参考。
\arg min サーボ最小角度
\n指定範囲 -180.0～180.0 (deg 刻み)
\n 注)RC 版での設定値であり、正式版で変更される場合がある 
\arg max サーボ最大角度
\n指定範囲 -180.0～180.0 (deg 刻み)
\n 注)RC 版での設定値であり、正式版で変更される場合がある


\subsection info 5.5 サーボ情報要求 GetServoInfo
<div style="margin-left:15px;">サーボの現在状態を取得する。</div>
* 送信JSON形式
\code{.js}
{
	"command": "GetServoInfo",
	"servo":[
		sid(1~254)
	]
	"item":[
		"all",
		"rom_model_num",
		"rom_servo_ID",
		"rom_cw_agl_lmt",
		"rom_ccw_agl_lmt",
		"rom_damper",
		"rom_cw_cmp_margin",
		"rom_ccw_cmp_margin",
		"rom_cw_cmp_slope",
		"rom_ccw_cmp_slope",
		"rom_punch",
		"ram_goal_pos",
		"ram_goal_tim",
		"ram_max_torque",
		"ram_torque_mode",
		"ram_pres_pos",
		"ram_pres_time",
		"ram_pres_spd",
		"ram_pres_curr",
		"ram_pres_temp",
		"ram_pres_volt",
		"Flags",
		"alg_ofset",
		"parents_ID",
		"connected",
		"read_time",
		"_ram_goal_pos",
		"__ram_goal_pos",
		"_ram_res_pos",
		"_send_speed",
		"_send_cmd_time",
		"flg_min_max",
		"flg_goal_pos",
		"flg_parent_inv",
		"flg_cmp_slope",
		"flg_check_angle",
		"port_type",
		"servo_type"
	]
}
\endcode


* 返事JSON形式
\code{.js}
{
	"type": "GetServoInfo",
	"raw":"",
	"servo":[
		{
			"sid":1~254,
			"rom_model_num":−32768~32767,
			"rom_servo_ID":1~254,
			"rom_cw_agl_lmt":-180.0~180.0,
			"rom_ccw_agl_lmt":-180.0~180.0,
			"rom_damper":0~254,
			"rom_cw_cmp_margin":0~254,
			"rom_ccw_cmp_margin":0~254,
			"rom_cw_cmp_slope":0~254,
			"rom_ccw_cmp_slope":0~254,
			"rom_punch":0~255,
			"ram_goal_pos":-180.0~180.0,
			"ram_goal_tim":−32768~32767,
			"ram_max_torque":0~255,
			"ram_torque_mode":0~2,
			"ram_pres_pos":-180.0~180.0,
			"ram_pres_time":−32768~32767,
			"ram_pres_spd":−32768~32767,
			"ram_pres_curr":−32768~32767,
			"ram_pres_temp":−32768~32767,
			"ram_pres_volt":−32768~32767,
			"Flags":0~255,
			"alg_ofset":-180.0~180.0,
			"parents_ID":1~254,
			"connected":0~1,
			"read_time":−32768~32767,
			"_ram_goal_pos":-180.0~180.0,
			"__ram_goal_pos":-180.0~180.0,
			"_ram_res_pos":-180.0~180.0,
			"_send_speed":0~255,
			"_send_cmd_time":0~255,
			"flg_min_max":0~1,
			"flg_goal_pos":0~1,
			"flg_parent_inv":0~255,
			"flg_cmp_slope":0~1,
			"flg_check_angle":0~1,
			"port_type":0~1,
			"servo_type":0~3
		}
	]
}
\endcode
\arg servo サーボ情報配列
\arg sid 4.5.1へ参考。
\arg 各項目については、6.2へ参考。




\subsection fbID 5.6 フィードバックID設定 SetFeedbackID
<div style="margin-left:15px;">サーボに関するフィードバック ID を設定する。</div>
* 送信JSON形式
\code{.js}
{
	"command": "SetFeedbackID",
	"servo":[
		sid(1~254)
	]
}
\endcode
* 返事JSON形式
\code{.js}
{
	"type": "ack",
	"raw":"",
	"message":"...",
}
\endcode
\arg servo サーボ情報配列
\arg sid 4.5.1へ参考。



\subsection feedback 5.7 フィードバック要求 GetServoFeedback
<div style="margin-left:15px;">フィードバック ID 設定コマンド(5.6)で登録したサーボ ID のサーボ情報を取得する。</div>
* 送信JSON形式
\code{.js}
{
	"command": "GetServoFeedback",
	"item":[
		"all",
		"rom_model_num",
		"rom_servo_ID",
		"rom_cw_agl_lmt",
		"rom_ccw_agl_lmt",
		"rom_damper",
		"rom_cw_cmp_margin",
		"rom_ccw_cmp_margin",
		"rom_cw_cmp_slope",
		"rom_ccw_cmp_slope",
		"rom_punch",
		"ram_goal_pos",
		"ram_goal_tim",
		"ram_max_torque",
		"ram_torque_mode",
		"ram_pres_pos",
		"ram_pres_time",
		"ram_pres_spd",
		"ram_pres_curr",
		"ram_pres_temp",
		"ram_pres_volt",
		"Flags",
		"alg_ofset",
		"parents_ID",
		"connected",
		"read_time",
		"_ram_goal_pos",
		"__ram_goal_pos",
		"_ram_res_pos",
		"_send_speed",
		"_send_cmd_time",
		"flg_min_max",
		"flg_goal_pos",
		"flg_parent_inv",
		"flg_cmp_slope",
		"flg_check_angle",
		"port_type",
		"servo_type"
	]
}
\endcode


* 返事JSON形式
\code{.js}
{
	"type": "GetServoFeedback",
	"raw":"",
	"servo":[
		{
			"sid":1~254,
			"rom_model_num":−32768~32767,
			"rom_servo_ID":1~254,
			"rom_cw_agl_lmt":-180.0~180.0,
			"rom_ccw_agl_lmt":-180.0~180.0,
			"rom_damper":0~254,
			"rom_cw_cmp_margin":0~254,
			"rom_ccw_cmp_margin":0~254,
			"rom_cw_cmp_slope":0~254,
			"rom_ccw_cmp_slope":0~254,
			"rom_punch":0~255,
			"ram_goal_pos":-180.0~180.0,
			"ram_goal_tim":−32768~32767,
			"ram_max_torque":0~255,
			"ram_torque_mode":0~2,
			"ram_pres_pos":-180.0~180.0,
			"ram_pres_time":−32768~32767,
			"ram_pres_spd":−32768~32767,
			"ram_pres_curr":−32768~32767,
			"ram_pres_temp":−32768~32767,
			"ram_pres_volt":−32768~32767,
			"Flags":0~255,
			"alg_ofset":-180.0~180.0,
			"parents_ID":1~254,
			"connected":0~1,
			"read_time":−32768~32767,
			"_ram_goal_pos":-180.0~180.0,
			"__ram_goal_pos":-180.0~180.0,
			"_ram_res_pos":-180.0~180.0,
			"_send_speed":0~255,
			"_send_cmd_time":0~255,
			"flg_min_max":0~1,
			"flg_goal_pos":0~1,
			"flg_parent_inv":0~255,
			"flg_cmp_slope":0~1,
			"flg_check_angle":0~1,
			"port_type":0~1,
			"servo_type":0~3
		}
	]
}
\endcode
\arg item 項目一つづつ取得。 
\arg 各項目については、6.2へ参考。
	



\subsection setvid 5.8 各種変数(VID)設定 SetVIDValue
<div style="margin-left:15px;">VID(設定値)を設定する。</div>
* 送信JSON形式
\code{.js}
{
	"command": "SetVIDValue",
	"vid":[
		{"RS485_Baudrate": "9600" ~ "57600" ~ "115200" ~ "1000000" or 0~255},
		{"TTL_Baudrate":"9600" ~ "57600" ~ "115200" ~ "1000000" or 0~255},
		{"RS232_Baudrate":"9600" ~ "57600" ~ "115200" ~ "1000000" or 0~255},
		{"IO_PA_IO_Mode":4~7},
		{"IO_PA_Analog_Mode":4~7},
		{"IO_PA_PWM":0~1},
		{"IO_PA_PWM_CYCLE":0~16383},
		{"Through_Port":0~255},
		{"Servo_Type_RS485":"None" ~ "FUTABA" ~ "ROBOTIS_v1.0" ~ ":ROBOTIS_v2.0" or 0~255},
		{"Servo_Type_TTL":"None" ~ "FUTABA" ~ "ROBOTIS_v1.0" ~ ":ROBOTIS_v2.0" or 0~255},
		{"IMU_Type": "None" or 0~255},
		{"Balancer_Flag":0~1},
		{"Theta_Th":0.1~10.0},
		{"Cycletime":1~100},
		{"Min_Cmp":1~250},
		{"Flag_Ack":0~1},
		{"Volt_Th":6.0~9.0},
		{"Initialize_Torque":0~1},
		{"Initialize_Angle":0~1},
		{"Inspection_Flag":0~1},
		{"Inspection_Type":0~1},
		{"Robot_Model":"None" ~ "GR-001" ~ "DARWIN-MINI" or 0~255},
		{"UID_Flag":0~1}
	]
}
\endcode

* 返事JSON形式
\code{.js}
{
	"type": "ack",
	"raw":"",
	"message":"..."
}
\endcode
\arg 各項目については、6.1へ参考してください。項目の数が自由に設定できる

	

	
\subsection getVid 5.9 各種変数(VID)要求 GetVIDValue	
<div style="margin-left:15px;">VID (設定値 )を要求する。</div>
* 送信JSON形式
\code{.js}
{
	"command": "GetVIDValue",
	"vid":[
		"all",
		"RS485_Baudrate",
		"TTL_Baudrate",
		"RS232_Baudrate",
		"IO_PA_IO_Mode",
		"IO_PA_Analog_Mode",
		"IO_PA_PWM",
		"IO_PA_PWM_CYCLE",
		"Through_Port",
		"Servo_Type_RS485",
		"Servo_Type_TTL",
		"IMU_Type",
		"Balancer_Flag",
		"Theta_Th",
		"Cycletime",
		"Min_Cmp",
		"Flag_Ack",
		"Volt_Th",
		"Initialize_Torque",
		"Initialize_Angle",
		"Inspection_Flag",
		"Inspection_Type",
		"Robot_Model",
		"UID_Flag"
	]
}
\endcode

* 返事JSON形式
\code{.js}
{
	"type": "GetVIDValue",
	"raw":"",
	"vid":[
		{"RS485_Baudrate": "9600" ~ "57600" ~ "115200" ~ "1000000" or 0~255},
		{"TTL_Baudrate":"9600" ~ "57600" ~ "115200" ~ "1000000" or 0~255},
		{"RS232_Baudrate":"9600" ~ "57600" ~ "115200" ~ "1000000" or 0~255},
		{"IO_PA_IO_Mode":4~7},
		{"IO_PA_Analog_Mode":4~7},
		{"IO_PA_PWM":0~1},
		{"IO_PA_PWM_CYCLE":0~16383},
		{"Through_Port":0~255},
		{"Servo_Type_RS485":"None" ~ "FUTABA" ~ "ROBOTIS_v1.0" ~ ":ROBOTIS_v2.0" or 0~255},
		{"Servo_Type_TTL":"None" ~ "FUTABA" ~ "ROBOTIS_v1.0" ~ ":ROBOTIS_v2.0" or 0~255},
		{"IMU_Type": "None" or 0~255},
		{"Balancer_Flag":0~1},
		{"Theta_Th":0.1~10.0},
		{"Cycletime":1~100},
		{"Min_Cmp":1~250},
		{"Flag_Ack":0~1},
		{"Volt_Th":6.0~9.0},
		{"Initialize_Torque":0~1},
		{"Initialize_Angle":0~1},
		{"Inspection_Flag":0~1},
		{"Inspection_Type":0~1},
		{"Robot_Model":"None" ~ "GR-001" ~ "DARWIN-MINI" or 0~255},
		{"UID_Flag":0~1}
	]
}
\endcode
\arg vid VIDの項目名 項目の数が自由に設定できる
\arg 各項目については、6.1へ参考してください。
\arg All 全項目を一括



\subsection write 5.10 フラッシュ書き込み要求 WriteFlash
<div style="margin-left:15px;">V-Sido CONNECT RCのフラッシュ領域に、現在のVID 設定値を書き込む。</div>
* 送信JSON形式
\code{.js}
{
	"command": "WriteFlash"
}
\endcode

* 返事JSON形式
\code{.js}
{
	"type": "ack",
	"raw":"",
	"message":"...",
}
\endcode
	

\subsection gpio 5.11 IO設定 SetGPIOValue
<div style="margin-left:15px;">GPIO 入出力ポートの設定を行う。</div>
* 送信JSON形式
\code{.js}
{
	"command": "SetGPIOValue",
	"gpio":[
		{"port":4~7,"val":0~1}
	]
}
\endcode

* 返事JSON形式
\code{.js}
{
	"type": "ack",
	"raw":"",
	"message":"...",
}
\endcode
\arg gpio GPIO設定配列。 項目の数が自由に設定できる
\arg port GPIO 入出力ポート
\n 指定範囲 4～7
\n 注)V-Sido CONNECT RC の基板上に記載された PA4～PA7 に対応する。
\arg val IO の出力内容
\n 指定範囲 0 or 1 


\subsection pwm 5.12 PWM設定 SetPWMPulse
<div style="margin-left:15px;">PWM 制御を行うためのポート設定を行う。</div>
* 送信JSON形式
\code{.js}
{
	"command": "SetPWMPulse",
	"pwm":[
		{"port":6~7,"width":0~65532,"duty":0.0~1.0}
	]
}
\endcode

* 返事JSON形式
\code{.js}
{
	"type": "ack",
	"raw":"",
	"message":"..."
}
\endcode
\arg pwm PWM設定配列。 項目の数が自由に設定できる
\arg port GPIO 入出力ポート
\n 指定範囲 6 or 7
\n 注)V-Sido CONNECT RC の基板上に記載された PA6、PA7 に対応する
\arg width パルス幅
\n パルス幅は　単位usec
\n 指定範囲 0~65532　4の倍数で切り捨てる
\arg duty パルスデューティ比


\subsection connected 5.13 接続確認 CheckConnectedServo
<div style="margin-left:15px;">サーボモータの接続確認を行う。</div>
* 送信JSON形式
\code{.js}
{
	"command": "CheckConnectedServo"
}
\endcode

* 返事JSON形式
\code{.js}
{
	"type": "CheckConnectedServo",
	"raw":"",
	"servo":[
		{
			"sid":1~254
			"tim": 0~254
		}
	]
}
\endcode
\arg sid 4.5.1へ参考。
\arg tim 関節角度受信までの時間(単位は micro-sec)。



\subsection setIK 5.14 IK設定 SetIK 			
<div style="margin-left:15px;">IK制御を行うためのデータ送信を行う。</div>
* 送信JSON形式
\code{.js}
{
	"command": "SetIK",
	"ikflag": {
		"position":false,"torque":false,"rotation":false
	},
	"kdt":[
		{
			"kid":"body" ~ "head" ~ "right_hand" ~ "left_hand" ~ "right_foot" ~ "left_foot" or 0 ~ 15
			"position"    :{"x" :-100~100,"y" :-100~100,"z":-100~100},
			"torque"      :{"x" :-100~100,"y" :-100~100,"z":-100~100},
			"rotation"    :{"x" :-100~100,"y" :-100~100,"z":-100~100}
		}
	]
}
\endcode

* 返事JSON形式
\code{.js}
{
	"type": "ack",
	"raw":"",
	"message":"..."
}
\endcode
\arg ikflag IK設定フラグ。
\arg torque トルク。
\arg position 位置。
\arg rotation 姿勢。
\arg kid IK 部位の ID
\n注)RC 版では、KID2～5 のみ利用可能。
\arg x (-100%～100%)x軸数字。
\arg y (-100%～100%)y軸数字。
\arg z (-100%～100%)z軸数字。




\subsection getIK 5.15 IK取得 GetIK	
<div style="margin-left:15px;">IK制御を行うためのデータ取得を行う。</div>
* 送信JSON形式
\code{.js}
{
	"command": "GetIK",
	"ikflag": {
		"position":false,"torque":false,"rotation":false
	}
	"kid":[
		"body" ~ "head" ~ "right_hand" ~ "left_hand" ~ "right_foot" ~ "left_foot" or 0 ~ 15
	]
}
\endcode

* 返事JSON形式
\code{.js}
{
	"type": "GetIK",
	"raw":"",
	"ikflag": {
		"position":false,"torque":false,"rotation":false
	},
	"kdt":[
		{
			"kid":"body" ~ "head" ~ "right_hand" ~ "left_hand" ~ "right_foot" ~ "left_foot" or 0 ~ 15
			"position"    :{"x" :-100~100,"y" :-100~100,"z":-100~100},
			"torque"      :{"x" :-100~100,"y" :-100~100,"z":-100~100},
			"rotation"    :{"x" :-100~100,"y" :-100~100,"z":-100~100}
		}
	]  
}
\endcode
\arg ikflag IK設定フラグ。
\arg torque トルク。
\arg position 位置。
\arg rotation 姿勢。
\arg kid IK 部位の ID
\n注)RC 版では、KID2～5 のみ利用可能。
\arg x (-100%～100%)x軸数字。
\arg y (-100%～100%)y軸数字。
\arg z (-100%～100%)z軸数字。



\subsection _walk 5.16 移動情報指定 Walk
<div style="margin-left:15px;">サーボモータの接続確認を行う。</div>
* 送信JSON形式
\code{.js}
{
	"command": "Walk",
	"forward":-100~100,
	"turnCW":-100~100
}
\endcode


* 返事JSON形式
\code{.js}
{
	"type": "ack",
	"raw":"",
	"message":"..."
}
\endcode
\arg forward 前後の速度-100%~100% ＋前進　-後退
\arg turnCW  旋回の速度-100%~100%　＋　時計回り　－反時計回り





\subsection acce 5.17 加速度センサ値要求 GetAcceleration
<div style="margin-left:15px;">加速度センサの情報(X/Y/Z 軸)を取得する(RC 版では機能制限あり)。</div>
* 送信JSON形式
\code{.js}
{
	"command": "GetAcceleration"
}
\endcode

* 返事JSON形式
\code{.js}
{
	"type": "GetAcceleration",
	"raw":"",
	"ax":-3.96875~3.90625,
	"ay":-3.96875~3.90625,
	"az":-3.96875~3.90625
}
\endcode
\arg ax X 軸方向の加速度※
\n 返信範囲 -3.96875~3.90625 G
\arg ay Y 軸方向の加速度※
\n 返信範囲 -3.96875~3.90625 G
\arg az Z 軸方向の加速度※
\n 返信範囲 -3.96875~3.90625 G





\subsection volt 5.18 電源電圧要求 GetVoltage	
<div style="margin-left:15px;">電源電圧を取得する(RC 版では機能未実装)。</div>
* 送信JSON形式
\code{.js}
{
	"command": "GetVoltage"
}
\endcode

* 返事JSON形式
\code{.js}
{
	"type": "GetVoltage",
	"raw":"",
	"volt":0.1~20.0
}
\endcode
\arg volt 電圧値
\n 返信範囲 0.1~20.0 (単位はv、精度0.1v)


\subsection notify 5.19 状態通知 StatusNotify	
<div style="margin-left:15px;">ロボット状態変更を通知する。(RC 版では機能未実装)。</div>
* 送信JSON形式
\code{.js}
{
	"command": "StatusNotify",
	"interval":1~,
	"action":[
		"SetServoAngle","SetIK","Walk"
	]
}
\endcode
* 通知JSON形式
\code{.js}
{
	"type": "StatusNotify",
	"raw":"",
	"action":"SetServoAngle"~"SetIK"~"Walk",
	"status":"running" ~ "success" ~ "failure",
	"diff":[
		{"sid":1~254,"angle":-360.0 ~ 360.0},
		{
			"kid":"body" ~ "head" ~ "right_hand" ~ "left_hand" ~ "right_foot" ~ "left_foot" or 0 ~ 15,
			"position":{
				"x":-200~200,"y":-200~200,"z":-200~200
			},
			"rotation":{
				"x":-200~200,"y":-200~200,"z":-200~200
			},
			"torque":{
				"x":-200~200,"y":-200~200,"z":-200~200
			}
		}
	]
}
\endcode
\arg uid 通知の識別ID
\arg interval　通知間隔　単位秒
\arg action 動作指示
\arg status 状態。
\n running　動作中、success成功完了、failure失敗完了
\arg diff 指示と実際の差分
\arg sid 4.5.1へ参考。
\n angle 指定角度の差分
\arg kids IK 部位の ID
\n注)RC 版では、KID2～5 のみ利用可能。
\n position IK位置の差分
\n rotation IK姿勢の差分
\n torque IKトルクの差分


\section appendix 6 参考

\subsection appendix_vid  6.1 V-Sido CONNECT RC VID 情報 
<table border="1" width="800" cellspacing="0" cellpadding="5" bordercolor="#333333" style="margin-left:30px;">
<tr>
<th bgcolor="#CFCFCF">名称</th>
<th bgcolor="#CFCFCF">意味</th>
<th bgcolor="#CFCFCF">範囲</th>
<th bgcolor="#CFCFCF">単位</th>
</tr>


<tr>
<td>RS485_Baudrate</td>
<td>RS485 接続のサーボとの通信 速度（単位：bps）\n 0：9600\n 1：57600\n 2：115200\n 3：1000000</td>
<td>0~3</td>
<td>-</td>
</tr>

<tr>
<td>TTL_Baudrate</td>
<td>TTL 接続サーボとの通信速度 速度（単位：bps）\n 0：9600\n 1：57600\n 2：115200\n 3：1000000</td>
<td>0~3</td>
<td>-</td>
</tr>

<tr>
<td>RS232_Baudrate</td>
<td>外部端末と RS232 接続の通信 速度（単位：bps）\n 0：9600\n 1：57600\n 2：115200\n 3：1000000</td>
<td>0~3</td>
<td>-</td>
</tr>

<tr>
<td>IO_PA_IO_Mode</td>
<td>汎用端子の IO モード</td>
<td>4～7※1</td>
<td>-</td>
</tr>

<tr>
<td>IO_PA_IO_Mode</td>
<td>汎用端子のアナログモード</td>
<td>4～7※1</td>
<td>-</td>
</tr>

<tr>
<td>IO_PA_PWM</td>
<td>汎用端子の PWM モード</td>
<td>0 or 1 </td>
<td>-</td>
</tr>

<tr>
<td>IO_PA_PWM_CYCLE(L)</td>
<td>汎用端子のPWM周期</td>
<td>0～65532</td>
<td>usec </td>
</tr>

<tr>
<td>IO_PA_PWM_CYCLE(H) </td>
<td>-</td>
<td>-</td>
<td>-</td>
</tr>


<tr>
<td>Through_Port</td>
<td>パススルーポート</td>
<td></td>
<td>-</td>
</tr>


<tr>
<td>Servo_Type_RS485</td>
<td>RS485 サーボの種類\n 0：なし\n 1：FUTABA \n 2：ROBOTIS ver1.0 \n 3：ROBOTIS ver2.0</td>
<td>0～3</td>
<td>-</td>
</tr>

<tr>
<td>Servo_Type_TTL</td>
<td>TTL サーボの種類\n 0：なし\n 1：FUTABA \n 2：ROBOTIS ver1.0 \n 3：ROBOTIS ver2.0</td>
<td>0～3</td>
<td>-</td>
</tr>

<tr>
<td>IMU_Type</td>
<td>IMU の種類\n 0：なし\n 1：センサスティック\n 2：MPU9150</td>
<td>0～2</td>
<td>-</td>
</tr>

<tr>
<td>Balancer_Flag</td>
<td>オートバランサーの ON/OFF\n 0:OFF 1:ON</td>
<td>0～1</td>
<td>-</td>
</tr>

<tr>
<td>Theta_Th</td>
<td>角度の閾値</td>
<td>0.1～10.0</td>
<td>deg</td>
</tr>

<tr>
<td>Cycletime</td>
<td>実行時間</td>
<td>1～100 </td>
<td>10msec</td>
</tr>

<tr>
<td>Min_Cmp</td>
<td>最小コンプライアンス</td>
<td>1 ～ 250</td>
<td>deg</td>
</tr>

<tr>
<td>Flag_Ack</td>
<td>Ack フラグの有無</td>
<td>0 or 1 </td>
<td>-</td>
</tr>

<tr>
<td>Volt_Th</td>
<td>電圧の閾値(未実装)</td>
<td>6.0～9.0</td>
<td>V</td>
</tr>

<tr>
<td>Initialize_Torque</td>
<td>初期化時のトルク有無</td>
<td>0 or 1 </td>
<td>-</td>
</tr>

<tr>
<td>Initialize_Angle</td>
<td>初期化時の目標角度設定</td>
<td>0 or 1 </td>
<td>-</td>
</tr>

<tr>
<td>Inspection_Flag</td>
<td>定期診断の有無</td>
<td>0 or 1 </td>
<td>-</td>
</tr>

<tr>
<td>Inspection_Type</td>
<td>定期診断の挙動</td>
<td>0 or 1 </td>
<td>-</td>
</tr>

<tr>
<td>Robot_Model</td>
<td>ロボットモデル\n 0:モデルなし※2 \n 1:GR-001 \n 2:DARWIN-MINI</td>
<td>0～2</td>
<td>-</td>
</tr>

<tr>
<td>UID_Flag</td>
<td>ユーザー設定領域の使用\n 0:使用しない\n 1:使用する</td>
<td>0 or 1 </td>
<td>-</td>
</tr>

</table>
<div style="margin-left:30px;">
※1 使用 bit は 4～7 とし、使用可能 IO ポートと bit が対応している。たとえば、
ポート 4 を使用する場合、bit4:1 になる
\n
※2 「0：モデルなし」を設定した場合、IK 機能および歩行機能を使用しない
\n
</div>





\subsection appendix_servo  6.2 V-Sido CONNECT RC サーボ情報管理テーブル
<table border="1" width="800" cellspacing="0" cellpadding="5" bordercolor="#333333" style="margin-left:30px;">
<tr>
<th bgcolor="#CFCFCF">名称</th>
<th bgcolor="#CFCFCF">意味</th>
<th bgcolor="#CFCFCF">範囲</th>
</tr>

<tr>
<td>rom_model_num</td>
<td>サーボモデル番号</td>
<td>−32768~32767</td>
</tr>

<tr>
<td>rom_servo_ID</td>
<td>ID</td>
<td>1～254</td>
</tr>



<tr>
<td>rom_cw_agl_lmt</td>
<td>時計回り回転リミット角（deg）</td>
<td>-180.0~180.0</td>
</tr>

<tr>
<td>rom_ccw_agl_lmt</td>
<td>反時計回り回転リミット角（deg）</td>
<td>-180.0~180.0</td>
</tr>


<tr>
<td>rom_damper</td>
<td>ダンパー</td>
<td>0～254</td>
</tr>


<tr>
<td>rom_cw_cmp_margin</td>
<td>コンプライアンスマージン（deg）</td>
<td>0.0～25.4</td>
</tr>


<tr>
<td>rom_ccw_cmp_margin</td>
<td>コンプライアンスマージン（deg）</td>
<td>0.0～25.4</td>
</tr>

<tr>
<td>rom_cw_cmp_slope</td>
<td>コンプライアンススコープ（deg）</td>
<td>0.0～25.4</td>
</tr>


<tr>
<td>rom_ccw_cmp_slope</td>
<td>コンプライアンススコープ（deg）</td>
<td>0.0～25.4</td>
</tr>


<tr>
<td>rom_punch</td>
<td>パンチ（最大トルクの 0.01%単位）</td>
<td>0～254</td>
</tr>

<tr>
<td>ram_goal_pos</td>
<td>目標角度</td>
<td>-180.0~180.0</td>
</tr>

<tr>
<td>ram_goal_tim</td>
<td>速度（目標到達時間を 10msec 刻みで指定）</td>
<td>−32768~32767</td>
</tr>

<tr>
<td>ram_max_torque</td>
<td>最大トルク</td>
<td>0~254</td>
</tr>

<tr>
<td>ram_torque_mode</td>
<td>トルクモード（0:off、1:on、2:break）</td>
<td>0~2</td>
</tr>

<tr>
<td>ram_pres_pos</td>
<td>現在角度（deg）</td>
<td>-180.0~180.0</td>
</tr>

<tr>
<td>ram_pres_time</td>
<td>現在時間（移動コマンド受信から 10msec 刻み）</td>
<td>−32768~32767</td>
</tr>


<tr>
<td>ram_pres_spd</td>
<td>現在速度（参考程度）</td>
<td>−32768~32767</td>
</tr>

<tr>
<td>ram_pres_curr</td>
<td>現在電流（mA）</td>
<td>−32768~32767</td>
</tr>

<tr>
<td>ram_pres_temp</td>
<td>現在温度（℃）</td>
<td>−32768~32767</td>
</tr>

<tr>
<td>ram_pres_volt</td>
<td>現在電圧（10mV）</td>
<td>−32768~32767</td>
</tr>


<tr>
<td>Flags</td>
<td>サーボのリターンフラグ（温度エラーなど）</td>
<td>0~254</td>
</tr>

<tr>
<td>alg_ofset</td>
<td>トリム角（deg）</td>
<td>−32768~32767</td>
</tr>

<tr>
<td>parents_ID</td>
<td>ダブルサーボ時の ID</td>
<td>0~254</td>
</tr>

<tr>
<td>connected</td>
<td>サーボ接続の有無（0:なし、1:あり）</td>
<td>0~1</td>
</tr>


<tr>
<td>read_time</td>
<td>関節角度の受信にかかった時間（msec）</td>
<td>−32768~32767</td>
</tr>

<tr>
<td>_ram_goal_pos</td>
<td>前回の目標角度（deg）</td>
<td>−180.0~180.0</td>
</tr>

<tr>
<td>__ram_goal_pos</td>
<td>前々回の目標角度（deg）</td>
<td>−180.0~180.0</td>
</tr>

<tr>
<td>_ram_res_pos</td>
<td>前回の現在角度（deg）</td>
<td>−180.0~180.0</td>
</tr>

<tr>
<td>_send_speed</td>
<td>前回の目標速度（0.1deg/sec）</td>
<td>0~254</td>
</tr>

<tr>
<td>_send_cmd_time</td>
<td>前回の long packet コマンド送信時間（10msec）</td>
<td>0~254</td>
</tr>

<tr>
<td>flg_min_max</td>
<td>現在角＞最大角のとき 1 現在角＜最小角のとき 1 通常 0</td>
<td>0~1</td>
</tr>

<tr>
<td>flg_goal_pos</td>
<td>目標角度に変化があったとき 1、変化がないとき 0</td>
<td>0~1</td>
</tr>


<tr>
<td>flg_parent_inv</td>
<td>ダブルサーボ時の逆転（実装予定）</td>
<td>0~1</td>
</tr>

<tr>
<td>flg_cmp_slope</td>
<td>コンプライアンススロープに変化があったとき 1、変化がないとき 0</td>
<td>0~1</td>
</tr>

<tr>
<td>flg_check_angle</td>
<td>常に角度情報を監視するか否か \n 0:監視しない\n 1:監視する</td>
<td>0~1</td>
</tr>

<tr>
<td>port_type</td>
<td>TTL 接続のとき 1、RS485 接続のとき 0</td>
<td>0~1</td>
</tr>

<tr>
<td>servo_type</td>
<td>サーボメーカー\n 0：モデルなし \n 1：FUTABA\n 2：ROBOTIS（Dynamixel Communication1.0）\n 3：ROBOTIS（Dynamixel Communication2.0）</td>
<td>0~3</td>
</tr>


</table>





*/
