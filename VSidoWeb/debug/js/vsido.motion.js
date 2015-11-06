/**
* V-Sido CONNECTとのモーションクラス。
* @class VSidoMotion
* @constructor
* @param {json} config 設定情報、IPアドレスのみ。
* 設定されなかった場合、window.location.hostnameを使用する。
* @example
* <pre> <code>
* var vsido = new VSidoMotion({'ip':'192.168.11.2'});
* var vsido = new VSidoMotion();
* </code></pre>
**/
var VSidoMotionNotifyCallBack = null;
var VSidoMotion = function(config){
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
		self.ws = new WebSocket('ws://' + self.ip +':20080','vsido-motion');
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
			if(VSidoMotionNotifyCallBack && remoteMsg['type'] == 'StatusNotify'){
				VSidoMotionNotifyCallBack(remoteMsg);
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
	* var start = {"motion":"start"};
	* vsido.send(start,function(response){
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
	* モーション再生を開始する。 
	* @method Start
	* @param {String} 　　　　motionID モーションID
    * @example
    * <pre><code>
	* var start = new vsido.Start();
	* </code></pre>
	**/
	this.Start = function(motionID) {
		this.motion = 'start';
		this.id = motionID;
		this.loop = false;
	}
	/**
	* モーション再生を停止する。 
	* @method Stop
    * @example
    * <pre><code>
	* var stop = new vsido.Stop();
	* </code></pre>
	**/
	this.Stop = function() {
		this.motion = 'stop';
	}

	/**
	* 内臓のモーションIDを取得する。 
	* @method List
    * @example
    * <pre><code>
	* var list = new vsido.List();
	* </code></pre>
	**/
	this.List = function() {
		this.motion = 'list';
	}

	/**
	* V-Sido CONNECTにモーションデータをアップロードする。
	* @method UploadMotion
	* @param {string} 　　　　motionID コマンド
	* @param {xml}         data コマンド
	* @param {function} 　  cb 返事のコールバック
    * @example
    * <pre><code>
	* vsido.UploadMotion(motionID,motion,function(response){
	*   console.log(JSON.stringify(response));
	* });
	* </code></pre>
	**/
	this.UploadMotion = function(motionID,data,cb) {
		this.cb = cb;
		if(null != this.ws && false!=this.ready) {
			var motionCMD = {};
			motionCMD.motion = 'upload';
			motionCMD.status = 'begin';
			motionCMD.id = motionID;
			motionCMD.data = ''
			this.ws.send(JSON.stringify(motionCMD));
			
			var remainData = data;
			while(remainData.length > 0) {
				motionCMD.status = 'continue';
				var dataSent = remainData.substr(0,1024);
				motionCMD.data = dataSent;
				remainData = remainData.substr(1024);
				this.ws.send(JSON.stringify(motionCMD));
			}
			motionCMD.status = 'end';
			motionCMD.data = ''
			this.ws.send(JSON.stringify(motionCMD));
			
		}
		else{
			openWS(this,this.ip)
		}
	};
};
