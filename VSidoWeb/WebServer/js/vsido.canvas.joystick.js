/* Copyright 2013 Intelligent Technology Inc.
 *
 * Released under the MIT license
 * See http://opensource.org/licenses/mit-license.php for full text.
 */
var JoyStick = {
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

  initialize : function(move) {
    this.canvas = $("#VSidoCanvasJoyStick");
    this.initCanvas();
	this.cb_move = move;

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

    this.canvas.on({
      vmousedown: function(ev) {
        self.touchStart(ev); // start drawing.
        ev.preventDefault();
      },
      vmousemove: function(ev) {
        self.touchMove(ev); // continue drawing while dragging the pointer.
        ev.preventDefault();
      },
      vmouseup: function(ev) {
	    console.log(ev);
        self.touchEnd(ev); // finish drawing.
        ev.preventDefault();
      }
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
      grad.addColorStop( 1, '#000000');
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

	touchStart : function(ev) {
		this.drawing = true;
		this.startX = ev.pageX - this.offset.left;
		this.startY = ev.pageY - this.offset.top;
		if(this.timerCentering != null) {
			clearInterval(this.timerCentering);
			this.timerCentering = null;
		}
		if(this.timerMoving != null) {
			clearInterval(this.timerMoving);
			this.timerMoving = null;
		}
		console.log(this.timerCentering);
		console.log(this.timerMoving);
	},

	touchMove : function(ev) {
		if (this.drawing) {
			var offsetX = ev.pageX - this.offset.left;
			var offsetY = ev.pageY - this.offset.top;
			this.startX = offsetX;
			this.startY = offsetY;
			this.reDraw();
			if(this.timerCentering != null) {
				clearInterval(this.timerCentering);
				this.timerCentering = null;
			}
			if(this.timerMoving == null) {
				this.timerMoving = setInterval( function( self) {
					if(self.cb_move) {
						self.cb_move((self.startX - self.centerX)*100/self.R,
						0 - (self.startY - self.centerY)*100/self.R);
					}
				}, this.inertvalMove,this);
			}
			console.log(this.timerCentering);
			console.log(this.timerMoving);
		}
	},

	touchEnd : function(ev) {
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
					self.cb_move((self.startX - self.centerX)*255/self.R,
						0 - (self.startY - self.centerY)*255/self.R
					);
				}
			}, this.inertval,this);
		}
	}
};

