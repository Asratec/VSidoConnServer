/* Copyright 2013 Intelligent Technology Inc.
 *
 * Released under the MIT license
 * See http://opensource.org/licenses/mit-license.php for full text.
 */
var Hand = {
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
  inertvalMove:15,
  timerMoving:null,
  img:null,

  initialize : function(move) {
    this.canvas = $("#VSidoCanvasHand");
    this.initCanvas();
	this.cb_move = move;

	this.offset = this.canvas.offset();
    this.context = this.canvas.get(0).getContext("2d");


    var self = this;
	var image = new Image;
	image.src = '/images/handmove.png';
	image.target = '_self';
	console.log(image);
	
	if (image.complete) {
		console.log(image);
	} else {
		image.onload = function () {
			console.log(image);
			self.reDraw();
		};
	}
	this.img = image;


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
      //self.reDraw();
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
        self.touchEnd(ev); // finish drawing.
        ev.preventDefault();
      }
    });

  },

  initCanvas : function() {
    this.canvas.get(0).width = this.canvas.width();
    this.canvas.get(0).height = this.canvas.height();
    this.centerX = this.canvas.width()/2;
    this.R = this.canvas.width()/2;
    this.centerY = this.canvas.height()/2;
    this.context = this.canvas.get(0).getContext("2d");
  },
  
  reDraw : function() {

      console.log('startX =<' + this.startX + '>startY=<' + this.startY + '>' );


	  this.context.fillStyle = '#ffffff';
	  this.context.fillRect(0,0,this.centerX*2,this.centerY*2);



	  this.context.beginPath();
	  this.context.moveTo(this.centerX,0);
	  this.context.lineTo(this.centerX,2*this.centerY);
	  this.context.moveTo(0,this.centerY);
	  this.context.lineTo(2*this.centerX,this.centerY);
	  this.context.stroke();
	  this.context.closePath();

	  this.context.beginPath();
	  this.context.strokeStyle = '00cc00';
	  this.context.font = '12px Arial';
	  this.context.strokeText('(0,0)',this.centerX + 20 ,this.centerY -20);
	  this.context.strokeText('- x',10 ,this.centerY);
	  this.context.strokeText('+ x',2*this.centerX - 20 ,this.centerY);
	  this.context.strokeText('+ y',this.centerY,10);
	  this.context.strokeText('- y',this.centerY,2*this.centerX-10);
	  this.context.closePath();
	  
	  
	  
	  
	  this.context.beginPath();
	  console.log(this.img);
	  console.log(this.startX);
	  console.log(this.startY);
	  console.log("caller is " + arguments.callee.caller.toString());
	  this.context.drawImage(this.img,this.startX -16, this.startY-16);
	  this.context.closePath();
	  if(this.cb_move && this.drawing) {
		this.cb_move((this.startX - this.centerX)*100/this.centerX,
					 0 - (this.startY - this.centerY)*100/this.centerY
		);
	  }
  },

	touchStart : function(ev) {
		this.drawing = true;
		this.startX = ev.pageX - this.offset.left;
		this.startY = ev.pageY - this.offset.top;
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
	}
};

