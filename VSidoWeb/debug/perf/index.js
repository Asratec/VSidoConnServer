var sent_num = 0;
var received_num = 0;




var vc = new vsido.Connect();
vc.onOpen = function(evt) {
    console.log("robot connected.");
//    var int_walk = setInterval(send_walk_command, 150);
};

var walk_CB = function (evt) {
  try {
    console.log(evt);
    received_num++;
    console.log('received_num:',received_num);
  } catch(e) {
  }
};

function send_walk_command() {
    var walk = new vsido.Walk(sent_num%100, 0);
    console.log(JSON.stringify(walk));
    vc.send(walk,walk_CB);
    sent_num++;
    console.log('sent_num:',sent_num);
}

var setIK_CB = function (evt) {
  try {
    console.log(evt);
    received_num++;
    console.log('received_num:',received_num);
  } catch(e) {
  }
};
function send_set_ik_command() {
    var ik = new vsido.SetIK({'position':true});
	ik.setPosition (1,100,100,0);
	ik.setPosition (2,100,100,0);
	ik.setPosition (3,100,100,0);
	ik.setPosition (4,100,100,0);
	ik.setPosition (5,100,100,0);
    console.log(JSON.stringify(ik));
    vc.send(ik,setIK_CB);
    sent_num++;
    console.log('sent_num:',sent_num);
}


var readAngle_CB = function (evt) {
  try {
    console.log(evt);
    received_num++;
    console.log('received_num:',received_num);
  } catch(e) {
  }
};




function send_read_angle_command() {
    var angle = new vsido.GetServoAngle();
	angle.addSID(2);
    console.log(JSON.stringify(angle));
    vc.send(angle,readAngle_CB);
    sent_num++;
    console.log('sent_num:',sent_num);
}

var readIK_CB = function (evt) {
  try {
    console.log(evt);
    received_num++;
    console.log('received_num:',received_num);
  } catch(e) {
  }
};

function send_read_ik_command() {
    var ik = new vsido.GetIK({'position':true});
	ik.addKID(2);
    console.log(JSON.stringify(ik));
    vc.send(ik,readIK_CB);
    sent_num++;
    console.log('sent_num:',sent_num);
}


/*
var ws = new WebSocket('ws://192.168.11.5:20080', 'vsido-cmd');

ws.onopen = function() {
    console.log("robot connected.");
    var command = JSON.stringify({command: "GetVIDValue", vid: ["all"]});
    var int_walk = setInterval(send_walk_command, 100);
};

ws.onmessage = function (evt) {
  try {
    var response = JSON.parse(evt.data);
    console.log(response);
    received_num++;
    console.log('received_num:',received_num);
  } catch(e) {
  }
};
*/
