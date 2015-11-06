var CONST_SERVO_MAX = 20;
var CONST_ITEM_MAX = 37;

function getValue(id){
	var checkName = ":input[name='s";
	checkName = checkName + id;
	checkName = checkName + "']:checked";
	//console.log(checkName);
	
	
	if($(checkName).val()) {
		return id;
	}
	return null;
}

function checkOn(id){
	var checkName = ":input[name='s";
	checkName = checkName + id;
	checkName = checkName + "']";
	//console.log(checkName);
	$(checkName).attr("checked", true); 
}

function checkOn2(id){
	var checkName = ":input[name='v";
	checkName = checkName + id;
	checkName = checkName + "']";
	//console.log(checkName);
	$(checkName).attr("checked", true); 
}

function getValue2(id){
	var checkName = ":input[name='v";
	checkName = checkName + id;
	checkName = checkName + "']:checked";
	//console.log(checkName);
	var name = ":input[name='v";
	name = name + id;
	name = name + "']";
	//console.log(name);
	//console.log($(name).val());
	
	
	if($(checkName).val()) {
		return $(name).val();
	}
	return null;
}


function createServoTable() {
	var htmlStr = '<tr>';
	for (var couter = 1; couter < CONST_SERVO_MAX + 1; couter = couter +1){
		var mod = (couter % 10);
		if(1 == mod && 1 != couter){
			htmlStr += '</tr><tr>';
		}
		htmlStr += '<td><p>Servo';
		htmlStr += couter;
		htmlStr += '<input type="checkbox" class="checkbox" name="s';
		htmlStr += couter;
		htmlStr += '" value="1">';
		htmlStr += '</p></td>';
	}
	htmlStr += '</tr>';
	$('#servoTable > tbody:last-child').append(htmlStr);
}


function createItemTable() {
	var ITEM_NAME = [
		'rom_model_num',
		'rom_servo_ID',
		'rom_cw_agl_lmt',
		'rom_ccw_agl_lmt',
		'rom_damper',
		'rom_cw_cmp_margin',
		'rom_ccw_cmp_margin',
		'rom_cw_cmp_slope',
		'rom_ccw_cmp_slope',
		'rom_punch',
		'ram_goal_pos',
		'ram_goal_tim',
		'ram_max_torque',
		'ram_torque_mode',
		'ram_pres_pos',
		'ram_pres_time',
		'ram_pres_spd',
		'ram_pres_curr',
		'ram_pres_temp',
		'ram_pres_volt',
		'Flags',
		'alg_ofset',
		'parents_ID',
		'connected',
		'read_time',
		'_ram_goal_pos',
		'__ram_goal_pos',
		'_ram_res_pos',
		'_send_speed',
		'_send_cmd_time',
		'flg_min_max',
		'flg_goal_pos',
		'flg_parent_inv',
		'flg_cmp_slope',
		'flg_check_angle',
		'port_type',
		'servo_type',
	];
	var couter = 1;
	var htmlStr = '<tr>';
	for (var item in ITEM_NAME) {
		var mod = (couter % 6);
		if(1 == mod && 1 != couter){
			htmlStr += '</tr><tr>';
		}
		htmlStr += '<td><p>';
		htmlStr += ITEM_NAME[item];
		htmlStr += '<input type="checkbox" class="checkbox" name="v';
		htmlStr += couter;
		htmlStr += '" value="';
		htmlStr += ITEM_NAME[item];
		htmlStr += '">';
		htmlStr += '</p></td>';
		couter = couter +1;
	}
	htmlStr += '</tr>';
	$('#InfoItemsTable > tbody:last-child').append(htmlStr);
}









function getVIDValue(id,cmd){
	var checkName = ":input[name='";
	checkName = checkName + id;
	checkName = checkName + "']:checked";
	//console.log(checkName);
	
	var valName = ":input[name='";
	valName = valName + id;
	valName = valName + "_val']";
	//console.log(valName);
	
	
	if($(checkName).val()) {
		cmd.addValue(id,parseInt($(valName).val()));
	}
	return null;
}

function getVIDValueID(id){
	var checkName = ":input[name='";
	checkName = checkName + id;
	checkName = checkName + "']:checked";
	//console.log(checkName);

	var name = ":input[name='";
	name = name + id;
	name = name + "']";
	
	if($(checkName).val()) {
		return $(name).val();
	}
	return null;
}

function setVIDValue(vars){
	for(var key in vars) {
		var valName = ":input[name='";
		valName = valName + key;
		valName = valName + "_val']";
		//console.log(valName);
		$(valName).val(vars[key]);
	}
}



function checkVIDOn(id){
	var checkName = ":input[name='";
	checkName = checkName + id;
	checkName = checkName + "']";
	//console.log(checkName);
	$(checkName).attr("checked", true); 
}



var VARS_ITEMS = [
	'RS485_Baudrate',
	'TTL_Baudrate',
	'RS232_Baudrate',
	'IO_PA_IO_Mode',
	'IO_PA_Analog_Mode',
	'IO_PA_PWM',	
	'IO_PA_PWM_CYCLE',
	'Through_Port',
	'Servo_Type_RS485',
	'Servo_Type_TTL',
	'IMU_Type',	
	'Balancer_Flag',
	'Theta_Th',	
	'Cycletime',	
	'Min_Cmp',		
	'Flag_Ack',	
	'Volt_Th',		
	'Initialize_Torque',
	'Initialize_Angle',
	'Inspection_Flag',
	'Inspection_Type',
	'Robot_Model',	
	'UID_Flag'
];
function createVidTable() {
	var couter = 1;
	var htmlStr = '<tr>';
	for (var item in VARS_ITEMS) {
		var mod = (couter % 4);
		if(1 == mod && 1 != couter){
			htmlStr += '</tr><tr>';
		}
		htmlStr += '<td><p>';
		htmlStr += VARS_ITEMS[item];
		htmlStr += '<input type="checkbox" class="checkbox" name="';
		htmlStr += VARS_ITEMS[item];
		htmlStr += '" value="';
		htmlStr += VARS_ITEMS[item];
		htmlStr += '">';

		htmlStr += '<input type="text" class="val" name="';
		htmlStr += VARS_ITEMS[item];
		htmlStr += '_val" value="0">';

		htmlStr += '</p></td>';
		couter = couter +1;
	}
	htmlStr += '</tr>';
	$('#vidTable > tbody:last-child').append(htmlStr);
}


function createServoAngleTable() {
	var htmlStr = '<tr>';
	for (var couter = 1; couter < CONST_SERVO_MAX +1; couter = couter +1){
		var mod = (couter % 6);
		if(1 == mod && 1 != couter){
			htmlStr += '</tr><tr>';
		}
		htmlStr += '<td><p>Servo';
		htmlStr += couter;
		htmlStr += '<input type="checkbox" class="checkbox" name="s';
		htmlStr += couter;
		htmlStr += '" value="1">';

		htmlStr += '<input type="text" class="val" name="s';
		htmlStr += couter;
		htmlStr += '_val" value="0">';

		htmlStr += '</p></td>';
	}
	htmlStr += '</tr>';
	$('#servoTable > tbody:last-child').append(htmlStr);
}

function getAngleValue(id,cmd){
	var checkName = ":input[name='s";
	checkName = checkName + id;
	checkName = checkName + "']:checked";
	//console.log(checkName);
	
	var valName = ":input[name='s";
	valName = valName + id;
	valName = valName + "_val']";
	//console.log(valName);
	
	var name = ":input[name='s";
	name = name + id;
	name = name + "']";

	
	if($(checkName).val()) {
		return {'sid':id,'angle':parseFloat($(valName).val())};
	}
	return null;
}
function setAngleValue(id,angle){
	var valName = ":input[name='s";
	valName = valName + id;
	valName = valName + "_val']";
	//console.log(valName);
	$(valName).val(angle);
}

var CONST_KID_MIN = 1
var CONST_KID_MAX = 5

function createIKTable() {
	var htmlStr = '<tr>';
	for (var couter = CONST_KID_MIN; couter < CONST_KID_MAX +1; couter = couter +1){
		var mod = (couter % 6);
		if(1 == mod && 1 != couter){
			htmlStr += '</tr><tr>';
		}
		htmlStr += '<td><p>';
		htmlStr += vsido.KID_ITEM[couter];
		htmlStr += '<input type="checkbox" class="checkbox" name="s';
		htmlStr += vsido.KID_ITEM[couter];;
		htmlStr += '" value="1"><br>';

		htmlStr += '<label>x:</label><input type="text" class="val" name="s';
		htmlStr += vsido.KID_ITEM[couter];;
		htmlStr += '_x" value="0">';

		htmlStr += '<br/>';
		
		htmlStr += '<label>y:</label><input type="text" class="val" name="s';
		htmlStr += vsido.KID_ITEM[couter];;
		htmlStr += '_y" value="0">';

		htmlStr += '<br/>';

		htmlStr += '<label>z:</label><input type="text" class="val" name="s';
		htmlStr += vsido.KID_ITEM[couter];;
		htmlStr += '_z" value="0">';

		htmlStr += '<br/>';

		htmlStr += '<label>Tx:</label><input type="text" class="val" name="s';
		htmlStr += vsido.KID_ITEM[couter];;
		htmlStr += '_Tx" value="0">';

		htmlStr += '<br/>';

		htmlStr += '<label>Ty:</label><input type="text" class="val" name="s';
		htmlStr += vsido.KID_ITEM[couter];;
		htmlStr += '_Ty" value="0">';

		htmlStr += '<br/>';

		htmlStr += '<label>Tz:</label><input type="text" class="val" name="s';
		htmlStr += vsido.KID_ITEM[couter];;
		htmlStr += '_Tz" value="0">';

		htmlStr += '<br/>';

		htmlStr += '<label>Rx:</label><input type="text" class="val" name="s';
		htmlStr += vsido.KID_ITEM[couter];;
		htmlStr += '_Rx" value="0">';

		htmlStr += '<br/>';

		htmlStr += '<label>Ry:</label><input type="text" class="val" name="s';
		htmlStr += vsido.KID_ITEM[couter];;
		htmlStr += '_Ry" value="0">';

		htmlStr += '<br/>';

		htmlStr += '<label>Rz:</label><input type="text" class="val" name="s';
		htmlStr += vsido.KID_ITEM[couter];;
		htmlStr += '_Rz" value="0">';
		
		
		htmlStr += '</p></td>';
	}
	htmlStr += '</tr>';
	$('#ikTable > tbody:last-child').append(htmlStr);
}
function getKID(id){
	var checkName = ":input[name='s";
	checkName = checkName + vsido.KID_ITEM[id];
	checkName = checkName + "']:checked";
	console.log(checkName);
	
	
	if($(checkName).val()) {
		return id;
	}
	return null;
}

function getIKValue(id,cmd){
	var checkName = ":input[name='s";
	checkName = checkName + vsido.KID_ITEM[id];;
	checkName = checkName + "']:checked";
	console.log(checkName);
	
	var valX = ":input[name='s";
	valX = valX + vsido.KID_ITEM[id];
	valX = valX + "_x']";
	console.log(valX);

	var valY = ":input[name='s";
	valY = valY + vsido.KID_ITEM[id];
	valY = valY + "_y']";
	console.log(valY);

	var valZ = ":input[name='s";
	valZ = valZ + vsido.KID_ITEM[id];
	valZ = valZ + "_z']";
	console.log(valZ);
	
	var valTX = ":input[name='s";
	valTX = valTX + vsido.KID_ITEM[id];
	valTX = valTX + "_Tx']";
	console.log(valTX);

	var valTY = ":input[name='s";
	valTY = valTY + vsido.KID_ITEM[id];
	valTY = valTY + "_Ty']";
	console.log(valTY);

	var valTZ = ":input[name='s";
	valTZ = valTZ + vsido.KID_ITEM[id];
	valTZ = valTZ + "_Tz']";
	console.log(valTZ);

	var valRX = ":input[name='s";
	valRX = valRX + vsido.KID_ITEM[id];
	valRX = valRX + "_Rx']";
	console.log(valRX);

	var valRY = ":input[name='s";
	valRY = valRY + vsido.KID_ITEM[id];
	valRY = valRY + "_Ry']";
	console.log(valRY);

	var valRZ = ":input[name='s";
	valRZ = valRZ + vsido.KID_ITEM[id];
	valRZ = valRZ + "_Rz']";
	console.log(valRZ);


	if($(checkName).val()) {
		cmd.setPosition(vsido.KID_ITEM[id],parseFloat($(valX).val()),parseFloat($(valY).val()),parseFloat($(valZ).val()));
		cmd.setTorque(vsido.KID_ITEM[id],parseFloat($(valTX).val()),parseFloat($(valTY).val()),parseFloat($(valTZ).val()));
		cmd.setRotation(vsido.KID_ITEM[id],parseFloat($(valRX).val()),parseFloat($(valRY).val()),parseFloat($(valRZ).val()));
	}
	return null;
}
function setIKValue(kdt){
	var id = kdt['kid'];
	if(kdt['position']) {
		var valX = ":input[name='s";
		valX = valX + id;
		valX = valX + "_x']";
		//console.log(valX);
		$(valX).val(kdt['position']['x']);
		
		var valY = ":input[name='s";
		valY = valY + id;
		valY = valY + "_y']";
		//console.log(valY);
		$(valY).val(kdt['position']['y']);

		var valZ = ":input[name='s";
		valZ = valZ + id;
		valZ = valZ + "_z']";
		//console.log(valZ);
		$(valZ).val(kdt['position']['z']);
		
	}
	if(kdt['rotation']) {
		var valRX = ":input[name='s";
		valRX = valRX + id;
		valRX = valRX + "_Rx']";
		//console.log(valRX);
		$(valRX).val(kdt['rotation']['x']);
		
		var valRY = ":input[name='s";
		valRY = valRY + id;
		valRY = valRY + "_Ry']";
		//console.log(valRY);
		$(valRY).val(kdt['rotation']['y']);

		var valRZ = ":input[name='s";
		valRZ = valRZ + id;
		valRZ = valRZ + "_Rz']";
		//console.log(valRZ);
		$(valRZ).val(kdt['rotation']['z']);
		
	}
	if(kdt['torque']) {
		var valTX = ":input[name='s";
		valTX = valTX + id;
		valTX = valTX + "_Tx']";
		//console.log(valTX);
		$(valTX).val(kdt['torque']['x']);
		
		var valTY = ":input[name='s";
		valTY = valTY + id;
		valTY = valTY + "_Ty']";
		//console.log(valTY);
		$(valTY).val(kdt['torque']['y']);

		var valTZ = ":input[name='s";
		valTZ = valTZ + id;
		valTZ = valTZ + "_Tz']";
		//console.log(valTZ);
		$(valTZ).val(kdt['torque']['z']);
		
	}

}

