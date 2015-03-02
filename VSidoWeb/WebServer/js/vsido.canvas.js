/* Copyright 2013 Intelligent Technology Inc.
 *
 * Released under the MIT license
 * See http://opensource.org/licenses/mit-license.php for full text.
 */
var Draw = {
  canvas : null, 
  context : null, 

  drawing : false,
  startX : 0,
  startY : 0,
  offset : {},
  cb_start:null,
  cb_end:null,

  initialize : function(canvas,cbStart,cbEnd) {
    this.canvas = canvas;
    this.initCanvas();
	this.cb_start = cbStart;
	this.cb_end = cbEnd;

	this.offset = this.canvas.offset();
    this.context = this.canvas.get(0).getContext("2d");

    var self = this;

    $(window).orientationchange(function(ev) {
      self.initCanvas();
    });

    $(window).resize(function(ev) {
      self.initCanvas();
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
      'vmouseup vmouseout': function(ev) {
        self.touchEnd(ev); // finish drawing.
        ev.preventDefault();
      }
    });

  },

  initCanvas : function() {
    this.canvas.get(0).width = this.canvas.width();
    this.canvas.get(0).height = this.canvas.height();
    this.context = this.canvas.get(0).getContext("2d");
  },
/*
  setPencil : function() {
    this.context.globalCompositeOperation = "source-over";
    this.context.strokeStyle = "#ff0000";
    this.context.lineWidth = 4;
    this.context.lineCap = "round";
  },

  setEraser : function() {
    this.context.globalCompositeOperation = "destination-out";
    this.context.strokeStyle = "#000000";
    this.context.lineWidth = 30;
    this.context.lineCap = "round";
  },
*/
  touchStart : function(ev) {
    this.drawing = true;

    this.startX = ev.pageX - this.offset.left;
    this.startY = ev.pageY - this.offset.top;
	if(this.cb_start) {
		this.cb_start(this.startX,this.startY);
	}
  },

  touchMove : function(ev) {
    if (this.drawing) {
      var offsetX = ev.pageX - this.offset.left;
      var offsetY = ev.pageY - this.offset.top;

      this.context.beginPath();
      this.context.moveTo(this.startX, this.startY);
      this.context.lineTo(offsetX, offsetY);
      this.context.stroke();
      this.context.closePath();

      this.startX = offsetX;
      this.startY = offsetY;
    }
  },

  touchEnd : function(ev) {
    this.drawing = false;
	if(this.cb_end) {
		this.cb_end(this.startX,this.startY);
	}
  }
};

