/**
* V-Sido Server設定。
* @namespace vsido
**/
var vsido = vsido || {
	
};

/**
* Bluetoothスキャンコマンドを生成する。
* @class ScanBluetooth
* @constructor
* @example
* <pre><code>
* var scan =  new vsido.ScanBluetooth();
* var connect = new vsido.Connect({'ip':'192.168.11.5'});
* connect.send(scan,function(response){
*   console.log(JSON.stringify(response));
* });
* </code></pre>
**/
vsido.ScanBluetooth = function (){
	this.cmd='ScanBluetooth';
};

/**
* Bluetoothペアリングコマンドを生成する。
* @class PairBluetooth
* @constructor
* @param {String} mac　BluetoothデバイスMACアドレス
* @example
* <pre><code>
* var pair =  new vsido.PairBluetooth('08:00:27:d0:61:59');
* var connect = new vsido.Connect({'ip':'192.168.11.5'});
* connect.send(pair,function(response){
*   console.log(JSON.stringify(response));
* });
* </code></pre>
**/
vsido.PairBluetooth = function (mac){
	this.cmd='PairBluetooth';
	if(mac){
		this.device={'mac':mac};
	}else{
		this.device={'mac':''};
	}
};

/**
* シリアルポート一覧コマンドを生成する。
* @class ListSerialPort
* @constructor
* @example
* <pre><code>
* var list =  new vsido.ListSerialPort();
* var connect = new vsido.Connect({'ip':'192.168.11.5'});
* connect.send(list,function(response){
*   console.log(JSON.stringify(response));
* });
* </code></pre>
**/
vsido.ListSerialPort = function (){
	this.cmd='ListSerialPort';
};

/**
* シリアルポート選択コマンドを生成する。
* @class SelectSerialPort
* @constructor
* @param {String} port　ポート名
* @param {int} baudrate　ボーレート
* @example
* <pre><code>
* var select =  new vsido.SelectSerialPort(port,baudrate);
* var connect = new vsido.Connect({'ip':'192.168.11.5'});
* connect.send(select,function(response){
*   console.log(JSON.stringify(response));
* });
* </code></pre>
**/
vsido.SelectSerialPort = function (port,baudrate){
	this.cmd='SelectSerialPort';
	if(port){
		this.port=port;
	}else{
		this.port='';
	}
	if(baudrate){
		this.baudrate=baudrate;
	}else{
		this.baudrate=115200;
	}
};
