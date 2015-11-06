/* Copyright 2013 Intelligent Technology Inc.
 *
 * Released under the MIT license
 * See http://opensource.org/licenses/mit-license.php for full text.
 */
var SensorAcc = {
  canvas : null, 
  context : null, 
  centerX : 150,
  centerY : 150,
  radX:0,
  radY:0,
  radZ:0,
  compH:null,
  compAcc:null,
  maxAx:0,
  maxAy:0,
  maxAz:0,
  axArray:null,
  ayArray:null,
  azArray:null,
  lastTimeStamp:0,

  initialize : function(move) {
    this.canvas = $("#VSidoCanvasSensorAcc");
    this.initCanvas();
	this.cb_move = move;
	
	this.axArray = new Array();
	this.ayArray = new Array(); 
	this.azArray = new Array(); 
	for(var idx=0; idx < 100; idx++){
		this.axArray.push(0);
		this.ayArray.push(0);
		this.azArray.push(0);
	}

	this.offset = this.canvas.offset();
    this.context = this.canvas.get(0).getContext("2d");

    var self = this;

    $(window).orientationchange(function(ev) {
      self.initCanvas();
      self.reDraw();
    });

    $(window).resize(function(ev) {
      self.initCanvas();
      self.reDraw();
    });
	$(document).ready(function(){
      self.initCanvas();
      self.reDraw();
	});


	$(window).bind('deviceorientation', function(e){
		console.log(e);
		self.compH = e.originalEvent.webkitCompassHeading | 0;
		self.compAcc = e.originalEvent.webkitCompassAccuracy | 0;
	});


	$(window).bind('devicemotion', function(e){
		console.log(e);
/*		
		var gx = e.originalEvent.accelerationIncludingGravity.x | 0;
		var gy = e.originalEvent.accelerationIncludingGravity.y | 0;
		var gz = e.originalEvent.accelerationIncludingGravity.z | 0;
*/		
/*
		if((e.originalEvent.timeStamp - self.lastTimeStamp) < 100) {
			console.log(self.lastTimeStamp);
			return;
		}
*/
		self.lastTimeStamp = e.originalEvent.timeStamp;

		
		var ax = ((10 *e.originalEvent.acceleration.x)| 0) /10;
		var ay = ((10 *e.originalEvent.acceleration.y)| 0) /10;
		var az = ((10 *e.originalEvent.acceleration.z)| 0) /10;
/*
		// filter out noise.
		var threshold = 0.1;
		if(Math.abs(ax)  < threshold && Math.abs(ay)  < threshold && Math.abs(az)  < threshold ) {
			return ;
		}
*/		
		
		self.axArray.shift();
		self.ayArray.shift();
		self.azArray.shift();
		
		self.axArray.push(ax);
		self.ayArray.push(ay);
		self.azArray.push(az);
		
		
		console.log(self.axArray);
		console.log(self.ayArray);
		console.log(self.azArray);
		self.reDraw();
	});
	},



  initCanvas : function() {
    this.canvas.get(0).width = this.canvas.width();
    this.canvas.get(0).height = this.canvas.height();
    this.centerX = this.canvas.width()/2;
    this.centerY = this.canvas.height()/2;
    this.context = this.canvas.get(0).getContext("2d");
  },
  
  reDraw : function() {

		this.context.fillStyle = '#ffffff';
		this.context.fillRect(0,0,this.centerX*2,this.centerY*2);
  
		var factor = 5.0;
		var factorX = 300/this.axArray.length;
		this.context.beginPath();
		this.context.strokeStyle = 'cc0000';
		var baseline = this.centerY/2;
		this.context.moveTo(0,baseline);
		for(var idx=0; idx < this.axArray.length; idx++) {
			var x = factorX*idx;
			var y = baseline + factor *this.axArray[idx];
			this.context.lineTo(x ,y);
			this.context.moveTo(x ,y);
		}
		this.context.stroke();
		this.context.closePath();

		this.context.beginPath();
		this.context.strokeStyle = '00cc00';
		this.context.lineWidth = 1;
		var baseline = 2*this.centerY/2;
		this.context.moveTo(0,baseline);
		for(var idx=0; idx < this.ayArray.length; idx++) {
			var x = factorX*idx;
			var y = baseline + factor *this.ayArray[idx];
			this.context.lineTo(x ,y);
			this.context.moveTo(x ,y);
		}
		this.context.stroke();
		this.context.closePath();
		
		this.context.beginPath();
		this.context.strokeStyle = '0000cc';
		var baseline = 3*this.centerY/2;
		this.context.moveTo(0,baseline);
		for(var idx=0; idx < this.azArray.length; idx++) {
			var x = factorX*idx;
			var y = baseline + factor *this.azArray[idx];
			this.context.lineTo(x ,y);
			this.context.moveTo(x ,y);
		}
		this.context.stroke();
		this.context.closePath();
			
		
		$("#vsido_output_request").text('send:<' + this.axArray.join('\n') +'>');

	}
};

