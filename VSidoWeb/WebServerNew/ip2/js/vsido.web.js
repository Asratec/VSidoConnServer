/**
* V-Sido CONNECTとの通信クラス。
* @class VSidoWeb
* @constructor
* @param {json} config 設定情報、IPアドレスのみ。
* 設定されなかった場合、window.location.hostnameを使用する。
* @example
* <pre> <code>
* var vsido = new VSidoWeb({'ip':'192.168.11.2'});
* var vsido = new VSidoWeb();
* </code></pre>
**/
var VSidoWebNotifyCallBack = null;
var VSidoWeb = function(config){
	if(config && config['ip']) {
		this.ip = config['ip'];
	}
	else {
		this.ip = window.location.hostname;
	}
	this.ws = null;
	this.cb = null;
	this.ready = false;
	this.requestDate = new Date();

	// open websocket 
	function openWS(self) {
		self.ws = new WebSocket('ws://' + self.ip +':20080','vsido-cmd');
		self.ws.onerror = function (error) {
			console.error(error);
		};
		// Log messages from the server
		self.ws.onmessage = function (evt) {
			var remoteMsg = JSON.parse( evt.data );
			var responseDate = new Date();
			var timeResponse = responseDate.getTime() - self.requestDate.getTime();
			if(self.cb){
				self.cb(remoteMsg,timeResponse);
			}
			if(VSidoWebNotifyCallBack && remoteMsg['type'] == 'StatusNotify'){
				VSidoWebNotifyCallBack(remoteMsg);
			}
		};
		self.ws.onopen = function (evt) {
			self.ready = true;
		};
		self.ws.onclose = function(evt){
			var remoteMsg = JSON.parse( evt.data );
			self.ready = false;
		}
	}
	openWS(this,this.ip);
	
	/**
	* V-Sido CONNECTにコマンドを送信する。
	* @method send
	* @param {json} 　　　　cmd コマンド
	* @param {function} 　cb 返事のコールバック
    * @example
    * <pre><code>
	* var angle = {"cmd":"servoAngle","cycle":1,"servo":[{"sid":2,"angle":-26}]};
	* vsido.send(angle,function(response){
	*   console.log(JSON.stringify(response));
	* });
	* </code></pre>
	**/
	this.send = function(cmd,cb) {
		this.requestDate = new Date();
		this.cb = cb;
		if(null != this.ws && false!=this.ready) {
			this.ws.send(JSON.stringify(cmd));
		}
		else{
			openWS(this,this.ip)
		}
	};

	
	
	/**
	* This method is deprecated. Use `SetServoAngle`
	<br/>
	* 目標角度設定コマンドを生成する。
	* @deprecated 
	* @method servoAngle
    * @example
    * <pre><code>
	* var angle = vsido.servoAngle();
	* angle["cycle"] = 2;
	* angle["servo"].push({"sid":2,"angle":100});
	* </code></pre>
	**/
	this.servoAngle = function() {
		var jsonMsg = {
			"cmd": "servoAngle",
			"cycle":  1,/*step*/
			"servo":[
				/*
				array of {'sid':1~254,'angle':-180.0~180.0}
				*/
			]
		};
		return jsonMsg;
	}
	
	/**
	* This method is deprecated. Use `GetServoInfo`
	<br/>
	* サーボ情報読み込みコマンドを生成する。
	* @deprecated
	* @method readServoInfo
    * @example
    * <pre><code>
	* var info = vsido.readServoInfo();
	* info["servo"].push({"sid":2,"address":100,"length": 2});
	* </code></pre>
	**/
	this.readServoInfo = function() {
		var jsonMsg = {
			'cmd': 'servo_info',
			'servo':[
			/*
				see this.readServoAngle
			*/
			]
		};
		return jsonMsg;
	}
	
	/**
	* This method is deprecated. Use `GetServoAngle`
	<br/>
	* サーボ情報読み込みコマンド用のパラメータを生成する。
	* 現在角度読み込むためパラメータ。
	* @method readServoAngle
	* @deprecated
    * @example
    * <pre><code>
	* var info = vsido.readServoInfo();
	* var readAngle = vsido.readServoAngle();
	* info['servo'].push(readAngle);
	* </code></pre>
	**/
	this.readServoAngle = function() {
		var jsonMsg = {
			'sid': 2,
			'address': 19,
			'length': 2,
		};
		return jsonMsg;
	}

	
	/**
	* This method is deprecated. Use `SetIK`
	<br/>
	* IK設定コマンドを生成する。
	* @method ik
	* @deprecated Use
    * @example
    * <pre><code>
	* var ik = vsido.ik();
	* ik['ikf']['dist']['pos'] = true;
	* var kdt = vsido.kdt();
	* ik['kdts'].push(kdt);
	* </code></pre>
	**/
	this.ik = function() {
		var jsonMsg = {
			'cmd': 'ik',
			'ikf': {
			  'dist'    :{'torq':false,'pos':false,'rot':false}
			},
			'kdts':[
			/*
				see this.kdt
			*/
			]
		};
		return jsonMsg;
	}

	/**
	* IK設定コマンドのパラメータを生成する。
	* This method is deprecated. Use `SetIK`
	<br/>
	* @method kdt
	* @deprecated
    * @example
    * <pre><code>
	* var ik = vsido.ik();
	* ik['ikf']['dist']['pos'] = true;
	* var kdt = vsido.kdt();
	* kdt['kid'] = 2;
	* kdt['kdt']['pos']['x'] = 35;
	* kdt['kdt']['pos']['y'] = 21;
	* kdt['kdt']['pos']['z'] = 68;
	* ik['kdts'].push(kdt);
	* </code></pre>
	**/
	this.kdt = function() {
		var jsonMsg = {
			'kid':0,/* 0~15 */
			'kdt': {
			  'pos'    :{'x':0,'y':0,'z':0},/* -100 ~ 100 percentage */
			  'rot'    :{'Rx':0,'Ry':0,'Rz':0},/* -100 ~ 100 percentage */
			  'torq'   :{'Tx':0,'Ty':0,'Tz':0},/* -100 ~ 100 percentage */
			}
		};
		return jsonMsg;
	}

	/**
	* This method is deprecated. Use `GetIK`
	<br/>
	* IK読み込みコマンドを生成する。 
	* @method readIK
	* @deprecated
    * @example
    * <pre><code>
	* var ik = vsido.readIK();
	* ik['ikf']['cur']['pos'] = true;
	* ik['kids'].push(2);
	* </code></pre>
	**/
	this.readIK = function() {
		var jsonMsg = {
			'cmd': 'ik',
			'ikf': {
			  'cur' :{'torq':false,'pos':false,'rot':false}
			},
			'kids':[
				/*
					0~15
				*/
			]
		};
		return jsonMsg;
	}
	

	/**
	* This method is deprecated. Use `Walk`
	<br/>
	* 歩行設定コマンドを生成する。 
	* @method walk
	* @deprecated
    * @example
    * <pre><code>
	* var walk = vsido.walk();
	* walk['forward']= 50;
	* walk['turn'] = 0;
	* </code></pre>
	**/
	this.walk = function() {
		var jsonMsg = {
			'cmd': 'walk',
			'forward':0,/* -100 ~ 100 percentage */
			'turn':0/* -100 ~ 100 percentage */
		};
		return jsonMsg;
	}

	/**
	* RAW コマンド(pass through)を生成する。
	* RAW コマンドはV-Sido CONNECTにそのまま送られる。
	* @method execRaw
    * @example
    * <pre><code>
	* var raw = vsido.execRaw();
	* raw['exec'].push(0xff);
	* raw['exec'].push(0x6f);
	* raw['exec'].push(0x08);
	* raw['exec'].push(0x01);
	* raw['exec'].push(0x02);
	* raw['exec'].push(0xc8);
	* raw['exec'].push(0x00);
	* raw['exec'].push(0x53);
	* </code></pre>
	**/
	this.execRaw = function() {
		var jsonMsg = {
			'cmd': 'raw',
			'exec' :[
				/*
				 Hex data spilted by [,]
				 example 0xff,0x74,0x4,0xaa
				*/
			]
		};
		return jsonMsg;
	}

	
	
	/**
	* 目標角度設定コマンドを生成する。
	* @method SetServoAngle
    * @example
    * <pre><code>
	* var angleCmd = new vsido.SetServoAngle();
	* angleCmd['cycle'] = 10;
	* var angle = new angleCmd.servoAngle(2,100.5);
	* angleCmd['servo'].push(angle);
	* </code></pre>
	**/
	this.SetServoAngle = function() {
		this.cmd = 'SetServoAngle';
		this.cycle = 1;
		this.servo = new Array();
		/**
		* 角度データを生成する。
		* @method SetServoAngle.servoAngle
		* @param {int} sid　1~254
		* @param {float} angle -180.0~180.0
		**/
		this.servoAngle = function(sid,angle) {
			this.sid = sid ? sid:0;
			this.angle = angle ? angle:0;
		}
	}

	/**
	* 現在角度取得コマンドを生成する。
	* @method GetServoAngle
    * @example
    * <pre><code>
	* var angleCmd = new vsido.GetServoAngle();
	* angleCmd['servo'].push(2);
	* </code></pre>
	**/
	this.GetServoAngle = function() {
		this.cmd = 'GetServoAngle';
		this.servo = new Array(); /* sid(1~254) */
	}

	
	
	/**
	* コンプライアンス設定コマンドを生成する。
	* @method SetServoCompliance
    * @example
    * <pre><code>
	* var compCmd = new vsido.SetServoCompliance();
	* var compServo = new compCmd.comp(2,35,21);
	* compCmd['servo'].push(compServo);
	* </code></pre>
	**/
	this.SetServoCompliance = function() {
		this.cmd = 'SetServoCompliance';
		this.servo = new Array();
		/**
		* コンプライアンスデータを生成する。
		* @method SetServoCompliance.comp
		* @param {int} sid　1~254
		* @param {int} cpCW 1~254
		* @param {int} cpCCW 1~254
		**/
		this.comp = function(sid,cpCW,cpCCW) {
			this.sid = sid ? sid:0;
			this.cpCW = cpCW ? cpCW:0;
			this.cpCCW = cpCCW ? cpCCW:0;
		}
	}

	/**
	* サーボ最小最大可動範囲設定コマンドを生成する。
	* @method SetServoMinMaxAngle
    * @example
    * <pre><code>
	* var minMaxCmd = new vsido.SetServoMinMaxAngle();
	* var minMax = new minMaxCmd.minMax(2,-100,100);
	* minMaxCmd['servo'].push(minMax);
	* </code></pre>
	**/
	this.SetServoMinMaxAngle = function() {
		this.cmd = 'SetServoMinMaxAngle';
		this.servo = new Array();
		/**
		* サーボ最小最大可動範囲データを生成する。
		* @method SetServoMinMaxAngle.minMax
		* @param {int} sid サーボID。
		* @param {float} min サーボ最小可動範囲。
		* @param {float} max サーボ最大可動範囲
		**/
		this.minMax = function(sid,min,max) {
			this.sid = sid ? sid:0;/* 1~254 */
			this.min = min ? min:0;/* -180.0~180.0*/
			this.max = max ? max:0;/* -180.0~180.0*/
		}
	}

	/**
	* サーボ情報読み込みコマンドを生成する。
	* @method GetServoInfo
    * @example
    * <pre><code>
	* var infoCmd = new vsido.GetServoInfo();
	* infoCmd['servo'].push(2);
	* </code></pre>
	**/
	this.GetServoInfo = function() {
		this.cmd = 'GetServoInfo';
		this.servo = new Array(); /* sid(1~254) */
	}

	
	/**
	* フィードバックID設定コマンドを生成する。
	* @method SetFeedbackID
    * @example
    * <pre><code>
	* var fbID = new vsido.SetFeedbackID();
	* fbID['servo'].push(2);
	* </code></pre>
	**/
	this.SetFeedbackID = function() {
		this.cmd = 'SetFeedbackID';
		this.servo = new Array(); /* sid(1~254) */
	}

	/**
	* フィードバック取得コマンドを生成する。
	* @method GetServoFeedback
    * @example
    * <pre><code>
	* var feedCmd = new vsido.GetServoFeedback();
	* feedCmd.showItems();
	* feedCmd.item.push('rom_model_num');
	* </code></pre>
	**/
	this.GetServoFeedback = function() {
		this.cmd = 'GetServoFeedback';
		this.item = new Array();
		/**
		* 取得できる項目名を表示する。
		* @method GetServoFeedback.showItems
		**/
		this.showItems = function() {
			console.log('rom_model_num');
			console.log('rom_servo_ID');
			console.log('rom_cw_agl_lmt');
			console.log('rom_ccw_agl_lmt');
			console.log('rom_damper');
			console.log('rom_cw_cmp_margin');
			console.log('rom_ccw_cmp_margin');
			console.log('rom_cw_cmp_slope');
			console.log('rom_ccw_cmp_slope');
			console.log('rom_punch');
			console.log('ram_goal_pos');
			console.log('ram_goal_tim');
			console.log('ram_max_torque');
			console.log('ram_torque_mode');
			console.log('ram_pres_pos');
			console.log('ram_pres_time');
			console.log('ram_pres_spd');
			console.log('ram_pres_curr');
			console.log('ram_pres_temp');
			console.log('ram_pres_volt');
			console.log('Flags');
			console.log('alg_ofset');
			console.log('parents_ID');
			console.log('connected');
			console.log('read_time');
			console.log('_ram_goal_pos');
			console.log('__ram_goal_pos');
			console.log('_ram_res_pos');
			console.log('_send_speed');
			console.log('_send_cmd_time');
			console.log('flg_min_max');
			console.log('flg_goal_pos');
			console.log('flg_parent_inv');
			console.log('flg_cmp_slope');
			console.log('flg_check_angle');
			console.log('port_type');
			console.log('servo_type');
		}
	}

	/**
	* VID設定コマンドを生成する。
	* @method SetVIDValue
    * @example
    * <pre><code>
	* var vidCmd = new vsido.SetVIDValue();
    * vidCmd.showVars();
    * vidCmd['var'].push({'RS485_Baudrate','115200'});
	* </code></pre>
	**/
	this.SetVIDValue = function() {
		this.cmd = 'SetVIDValue';
		this.var = new Array();

		/**
		* 設定できる項目名を表示する。
		* @method GetVIDValue.showItems
		**/
		this.showVars = function() {
			console.log('"RS485_Baudrate": "9600" ~ "57600" ~ "115200" ~ "1000000" or 0~255');
			console.log('"TTL_Baudrate":"9600" ~ "57600" ~ "115200" ~ "1000000" or 0~255');
			console.log('"RS232_Baudrate":"9600" ~ "57600" ~ "115200" ~ "1000000" or 0~255');
			console.log('"IO_PA_IO_Mode":4~7');
			console.log('"IO_PA_Analog_Mode":4~7');
			console.log('"IO_PA_PWM":0~1');
			console.log('"IO_PA_PWM_CYCLE":0~16383');
			console.log('"Through_Port":0~255');
			console.log('"Servo_Type_RS485":"None" ~ "FUTABA" ~ "ROBOTIS_v1.0" ~ "ROBOTIS_v2.0" or 0~255');
			console.log('"Servo_Type_TTL":"None" ~ "FUTABA" ~ "ROBOTIS_v1.0" ~ "ROBOTIS_v2.0" or 0~255');
			console.log('"IMU_Type": "None" or 0~255');
			console.log('"Balancer_Flag":0~1');
			console.log('"Theta_Th":0.1~10.0');
			console.log('"Cycletime":1~100');
			console.log('"Min_Cmp":1~250');
			console.log('"Flag_Ack":0~1');
			console.log('"Volt_Th":6.0~9.0');
			console.log('"Initialize_Torque":0~1');
			console.log('"Initialize_Angle":0~1');
			console.log('"Inspection_Flag":0~1');
			console.log('"Inspection_Type":0~1');
			console.log('"Robot_Model":"None" ~ "GR-001" ~ "DAWIN-MINI" or 0~255');
			console.log('"UID_Flag":0~1');
		}
	}

	/**
	* VID取得コマンドを生成する。
	* @method GetVIDValue
    * @example
    * <pre><code>
	* var vidCmd = new vsido.GetVIDValue();
	* vidCmd.showVars();
	* vidCmd['var'].push('RS485_Baudrate');
	* </code></pre>
	**/
	this.GetVIDValue = function() {
		this.cmd = 'GetVIDValue';
		this.var = new Array();
		/**
		* 取得できる項目名を表示する。
		* @method GetVIDValue.showItems
		**/
		this.showVars = function() {
			console.log('all');
			console.log('RS485_Baudrate');
			console.log('TTL_Baudrate');
			console.log('RS232_Baudrate');
			console.log('IO_PA_IO_Mode');
			console.log('IO_PA_Analog_Mode');
			console.log('IO_PA_PWM');
			console.log('IO_PA_PWM_CYCLE');
			console.log('Through_Port');
			console.log('Servo_Type_RS485');
			console.log('Servo_Type_TTL');
			console.log('IMU_Type');
			console.log('Balancer_Flag');
			console.log('Theta_Th');
			console.log('Cycletime');
			console.log('Min_Cmp');
			console.log('Flag_Ack');
			console.log('Volt_Th');
			console.log('Initialize_Torque');
			console.log('Initialize_Angle');
			console.log('Inspection_Flag');
			console.log('Inspection_Type');
			console.log('Robot_Model');
			console.log('UID_Flag');
		}
	}

	/**
	* フラッシュ書き込みコマンドを生成する。
	* @method WriteFlash
    * @example
    * <pre><code>
	* var flash = new vsido.WriteFlash();
	* </code></pre>
	**/
	this.WriteFlash = function() {
		this.cmd = 'WriteFlash';
	}

	/**
	* IO設定コマンドを生成する。
	* @method SetGPIOConfig
    * @example
    * <pre><code>
	* var ioConfigCmd = new vsido.SetGPIOConfig();
	* var config = new ioConfigCmd.config(4,0);
	* ioConfigCmd['gpio'].push(config);
	* </code></pre>
	**/
	this.SetGPIOConfig = function() {
		this.cmd = 'SetGPIOConfig';
		this.gpio = new Array();
		/**
		* GPIO Configデータを生成する。
		* @method SetGPIOConfig.config
		* @param {int} port ポートID。
		* @param {int} val 0:output 1:input
		**/
		this.config = function(port,val) {
			this.port = port ? port:0;
			this.val = val ? val:0;
		}
	}

	/**
	* PWM設定コマンドを生成する。
	* @method SetPWMPulseWidth
    * @example
    * <pre><code>
	* var pwmCmd = vsido.SetPWMPulseWidth();
	* var pulse = new pwmCmd.pulse(6,2000);
	* pwmCmd['pwm'].push(pulse);
	* </code></pre>
	**/
	this.SetPWMPulseWidth = function() {
		this.cmd = 'SetPWMPulseWidth';
		this.pwm = new Array();
		/**
		* PWM パルス幅データを生成する。
		* @method SetPWMPulseWidth.pulse
		* @param {int} port ポートID。
		* @param {int} pul パルス幅
		**/
		this.pulse = function(port,pul) {
			this.port = port ? port:0;
			this.pul = pul ? pul:0;
		}
	}
	
	/**
	* 接続確認コマンドを生成する。
	* @method CheckConnectedServo
    * @example
    * <pre><code>
	* var connectedInfo = new vsido.CheckConnectedServo();
	* </code></pre>
	**/
	this.CheckConnectedServo = function() {
		this.cmd = 'CheckConnectedServo';
	}

	/**
	* IK設定コマンドを生成する。
	* @method SetIK
    * @example
    * <pre><code>
	* var ik = new vsido.SetIK();
	* ik['ikf']['pos'] = true;
	* var kdt = new vsido.kdt(10,-10,20,10,10,10,20,20-20);
	* ik['kdts'].push(kdt);
	* </code></pre>
	**/
	this.SetIK = function() {
		this.cmd = 'SetIK';
		this.ikf = {
			'pos':false,'rot':false,'torq':false
		};
		this.kdts = new Array();
		/**
		* PWM パルス幅データを生成する。
		* @method SetIK.kdt
		* @param {int,String} kid 
		* @param {int} x  位置X軸
		* @param {int} y  位置Y軸
		* @param {int} z  位置Z軸
		* @param {int} Rx 姿勢X軸
		* @param {int} Ry 姿勢Y軸
		* @param {int} Rz 姿勢Z軸
		* @param {int} Tx トルコX軸
		* @param {int} Ty トルコY軸
		* @param {int} Tz トルコZ軸
		**/
		this.kdt = function(kid,x,y,z,Rx,Ry,Rz,Tx,Ty,Tz) {
			this.kid = kid ? kid:0;

			this.kdt = {};
			this.kdt['pos'] = {};
			this.kdt['pos']['x'] = x?x:0;
			this.kdt['pos']['y'] = y?y:0;
			this.kdt['pos']['z'] = z?z:0;

			this.kdt['rot'] = {};
			this.kdt['pos']['Rx'] = Rx?Rx:0;
			this.kdt['pos']['Ry'] = Ry?Ry:0;
			this.kdt['pos']['Rz'] = Rz?Rz:0;
			
			this.kdt['torq'] = {};
			this.kdt['torq']['Tx'] = Tx?Tx:0;
			this.kdt['torq']['Ty'] = Ty?Ty:0;
			this.kdt['torq']['Tz'] = Tz?Tz:0;
			
		}
	}


	/**
	* IK読み込みコマンドを生成する。 
	* @method GetIK
    * @example
    * <pre><code>
	* var ikCmd = new vsido.GetIK();
	* ikCmd['ikf']['pos'] = true;
	* ik['kids'].push(2);
	* </code></pre>
	**/
	this.GetIK = function() {
		this.cmd = "GetIK";
		this.ikf = {
			'pos':false,'rot':false,'torq':false
		};
		this.kids = new Array();
	}

	/**
	* 歩行設定コマンドを生成する。 
	* @method Walk
    * @example
    * <pre><code>
	* var walk = new vsido.Walk();
	* walk['forward']= 50;
	* walk['turnCW'] = 0;
	* </code></pre>
	**/
	this.Walk = function() {
		this.cmd = 'Walk';
		this.forward = 0;/* -100 ~ 100 percentage */
		this.turnCW = 0;/* -100 ~ 100 percentage */
	}
	
	/**
	* 加速度センサ値取得コマンドを生成する。
	* @method GetAcceleration
    * @example
    * <pre><code>
	* var aSensor = new vsido.GetAcceleration();
	* </code></pre>
	**/
	this.GetAcceleration = function() {
		this.cmd = 'GetAcceleration';
	}

	/**
	* 電源電圧取得コマンドを生成する。
	* @method GetVoltage
    * @example
    * <pre><code>
	* var power = new vsido.GetVoltage();
	* </code></pre>
	**/
	this.GetVoltage = function() {
		this.cmd = 'GetVoltage';
	}

	/**
	* ロボット状態変更を通知する。
	* @method StatusNotify
    * @example
    * <pre><code>
	* var notify = new vsido.StatusNotify();
	* </code></pre>
	**/
	this.StatusNotify = function(callback) {
		this.cmd = 'StatusNotify';
		this.interval = 0.1;
		this.action = new Array();
		VSidoWebNotifyCallBack = callback;
		/**
		* 通知できる指示を表示する。
		* @method StatusNotify.showCommand
		**/
		this.showCommand = function() {
			console.log('SetServoAngle');
			console.log('SetIK');
			console.log('Walk');
		}
	}
	
	/**
	* 任意バイナリコマンドを生成する。
	* @method Binary
    * @example
    * <pre><code>
	* var uart = new vsido.Binary();
	* uart['op'] = 0x63;
	* uart['bin'].push([0x0,0x11]);
	* </code></pre>
	**/
	this.Binary = function() {
		this.cmd = 'Binary';
		this.op = 0;
		this.bin = new Array();
	}
	
	/**
	* Bluetoothデバイスをスキャンする。
	* Only @Bluetooth Connection。
	* @method scanBT
	* @param {function} 　cb デバイス名、Macアドレス一覧
    * @example
    * <pre><code>
	* vsido.scanBT(function(response){
	*   console.log(JSON.stringify(response));
	* });
	* </code></pre>
	**/
	this.scanBT = function(cb) {
		this.cb = cb;
		var scanMsg = {'cmd':'scan_bt'};
		if(null != this.ws && false!=this.ready) {
			this.ws.send(JSON.stringify(scanMsg));
		}
		else{
			openWS(this,this.ip)
		}
	}

	/**
	* Bluetoothデバイスをぺアリングする。
	* Only @Bluetooth Connection。
	* @method bindBT
	* @param {json} device Macアドレス
    * @example
    * <pre><code>
	* var device = {'mac':'00:1B:DC:09:53:C3'};
	* vsido.bindBT(device);
	* </code></pre>
	**/
	this.bindBT = function(device) {
		var bindMsg = {'cmd':'bind_bt','device':device};
		if(null != this.ws && false!=this.ready) {
			this.ws.send(JSON.stringify(bindMsg));
		}
		else{
			openWS(this,this.ip)
		}
	}
};
