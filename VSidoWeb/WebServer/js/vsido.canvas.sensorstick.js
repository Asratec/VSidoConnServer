/* Copyright 2013 Intelligent Technology Inc.
 *
 * Released under the MIT license
 * See http://opensource.org/licenses/mit-license.php for full text.
 */
var SensorStick = {
  canvas : null, 
  context : null, 

  drawing : false,
  startX : 150,
  startY : 150,
  centerX : 150,
  centerY : 150,
  R : 0,
  r_touch:15,
  offset : {},
  cb_start:null,
  cb_end:null,
  inertval:200,
  timerCentering:null,
  inertvalMove:200,
  timerMoving:null,
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
  action:false,

  initialize : function(move) {
    this.canvas = $("#VSidoCanvasSensorStick");
    this.initCanvas();
	this.cb_move = move;
	this.axArray = new Array();
	this.ayArray = new Array(); 
	this.azArray = new Array(); 

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
		var interval = e.originalEvent.timeStamp - self.lastTimeStamp;
		self.lastTimeStamp = e.originalEvent.timeStamp;
//		$("#vsido_output_request").text('send:<' + interval +'>');
//		console.log(interval);
		if(interval > 1000){
//			$("#vsido_output_request").text('send:< interval > 1000>');
			return;
		}
		
		var ax = ((10 *e.originalEvent.acceleration.x)| 0) /10;
		var ay = ((10 *e.originalEvent.acceleration.y)| 0) /10;
		var az = ((10 *e.originalEvent.acceleration.z)| 0) /10;
		// filter out noise.
		var threshold = 2;
		var accABS = Math.abs(ax) + Math.abs(ay) + Math.abs(az);
		//$("#vsido_output_request").text('send:<' + accABS +'>');
		if(  accABS < threshold ) {
			if(self.action) {
				if(self.axArray.length > 5) {
					self.calcOffset();
					self.reDraw();
					self.actionEnd();
				}
				self.axArray = new Array();
				self.ayArray = new Array();
				self.azArray = new Array();
				self.action = false;
			}
			return ;
		}
		self.action = true;
		self.axArray.push(interval * ax);
		self.ayArray.push(interval * ay);
		self.azArray.push(interval * az);
		//$("#vsido_output_request").text('send:<' + self.axArray.join(',\n') + ';' + self.ayArray.join(',\n') + ';' + self.azArray.join(',\n') +'>');

	});
	},



  initCanvas : function() {
    this.canvas.get(0).width = this.canvas.width();
    this.canvas.get(0).height = this.canvas.height();
    this.centerX = this.canvas.width()/2;
    this.centerY = this.canvas.height()/2;
    this.R = this.canvas.width()/2 - 20;
    this.context = this.canvas.get(0).getContext("2d");
  },
  
  reDraw : function() {

      console.log('startX =<' + this.startX + '>startY=<' + this.startY + '>' );

	  this.context.fillStyle = '#ffffff';
	  this.context.fillRect(0,0,this.centerX*2,this.centerY*2);

	  
	  this.context.beginPath();
	  this.context.strokeStyle = '#00cc00';
	  this.context.font = '10px Arial';
	  this.context.strokeText('forword',this.centerX,10);
	  this.context.strokeText('backword',this.centerX,this.centerY*2 -10 );
	  this.context.strokeText('left',0,this.centerY);
	  this.context.strokeText('right',this.centerX*2-20, this.centerY);
	  this.context.closePath();
	  

	  var grad = this.context.createRadialGradient( this.centerX, this.centerY, this.R/10, 
											this.centerX, this.centerY, this.R );
      grad.addColorStop( 0, '#ffffff');
      grad.addColorStop( 1, '#0000cc');
	  this.context.fillStyle = grad;
	  this.context.beginPath();
	  this.context.arc(this.centerX, this.centerY, this.R, 0, Math.PI*2, false);
	  this.context.fill();
	  //this.context.stroke();
      this.context.closePath();
	  var r_move = Math.pow((this.startX - this.centerX),2)
				+  Math.pow((this.startY - this.centerY),2);
	  if(Math.pow(this.R,2) >r_move) {
		  this.context.beginPath();
		  this.context.arc(this.startX, this.startY, this.r_touch, 0, Math.PI*2, false);
	//	  this.context.stroke();
		  this.context.fillStyle = '#ff0000';
		  this.context.fill();
		  this.context.closePath();
	  }

  },


	actionEnd : function(ev) {
		this.drawing = false;
		this.reDraw();
		console.log(this.timerMoving);
		if(this.timerMoving!= null) {
			clearInterval(this.timerMoving);
			this.timerMoving = null;
		}
		console.log(this.timerCentering);
		if(this.timerCentering == null) {
			this.timerCentering = setInterval( function( self) {
				var dX = (self.centerX - self.startX);
				var dY = (self.centerY - self.startY);
				self.startX = self.startX + dX/4;
				self.startY = self.startY + dY/4;
				if(  dX * dX + dY * dY < 25) {
					self.startX = self.centerX;
					self.startY = self.centerY;			
					clearInterval(self.timerCentering);
					self.timerCentering = null;
				}
				self.reDraw();
				if(self.cb_move != null) {
					self.cb_move((self.startX - self.centerX)*100/self.R,
						0 - (self.startY - self.centerY)*100/self.R
					);
				}
			}, this.inertval,this);
		}
	},
	calcOffset: function() {
		var xMinus = false;
		var counterXMinus = 0;
		var counterXPlus = 0;
		for(var idx=0; idx < this.axArray.length/2; idx++){
			if(this.axArray[idx] > 0) {
				counterXPlus += 1;
			} else {
				counterXMinus += 1;			
			}
		}
		if(counterXPlus < counterXMinus) {
			xMinus = false;
		} else {
			xMinus = true;
		}

		var axValue = 0;
		for(var idx=0; idx < this.axArray.length; idx++){
			axValue += Math.abs(this.axArray[idx]);
		}
		
		
		
		var yMinus = false;		
		var counterYMinus = 0;
		var counterYPlus = 0;
		for(var idx=0; idx < this.ayArray.length/2; idx++){
			if(this.ayArray[idx] > 0) {
				counterYPlus += 1;
			} else {
				counterYMinus += 1;			
			}
		}
		if(counterYPlus > counterYMinus) {
			yMinus = false;
		} else {
			yMinus = true;
		}
		
		
		var ayValue = 0;
		for(var idx=0; idx < this.ayArray.length; idx++){
			ayValue += Math.abs(this.ayArray[idx]);
		}
		/*
		var zMinus = this.azArray[0] < 0;
		var azValue = 0;
		for(var idx=0; idx < this.azArray.length; idx++){
			azValue += Math.abs(this.azArray[idx]);
		}
		*/
		this.startX = this.R*(1 - 1/axValue );
		if(xMinus) {
			this.startX = 0 - this.startX;
		}
		this.startY = this.R*(1 - 1/ayValue);
		if(yMinus) {
			this.startY = 0 - this.startY;
		}
		if(Math.abs(this.startX) < Math.abs(this.startY)) {
			this.startX = 0;
		} else {
			this.startY = 0;
		}
		this.startX = this.startX +this.centerX;
		this.startY = this.startY +this.centerY;
		//$("#vsido_output_request").text('startX =<' + this.startX + '>startY=<' + this.startY + '>' );
	}
};

