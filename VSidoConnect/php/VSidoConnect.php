<?php

/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.5
 *
 * This file is not intended to be easily readable and contains a number of
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG
 * interface file instead.
 * ----------------------------------------------------------------------------- */

// Try to load our extension if it's not already loaded.
if (!extension_loaded('VSidoConnect')) {
  if (strtolower(substr(PHP_OS, 0, 3)) === 'win') {
    if (!dl('php_VSidoConnect.dll')) return;
  } else {
    // PHP_SHLIB_SUFFIX gives 'dylib' on MacOS X but modules are 'so'.
    if (PHP_SHLIB_SUFFIX === 'dylib') {
      if (!dl('VSidoConnect.so')) return;
    } else {
      if (!dl('VSidoConnect.'.PHP_SHLIB_SUFFIX)) return;
    }
  }
}



abstract class VSidoConnect {
	static function connect($dist=null) {
		switch (func_num_args()) {
		case 0: connect(); break;
		default: connect($dist);
		}
	}

	static function disConnect() {
		disConnect();
	}

	static function execWithJsonResult($req) {
		return execWithJsonResult($req);
	}
}

/* PHP Proxy Classes */
abstract class Request {
	public $_cPtr=null;
	protected $_pData=array();

	function __set($var,$value) {
		if ($var === 'thisown') return swig_VSidoConnect_alter_newobject($this->_cPtr,$value);
		$this->_pData[$var] = $value;
	}

	function __get($var) {
		if ($var === 'thisown') return swig_VSidoConnect_get_newobject($this->_cPtr);
		return $this->_pData[$var];
	}

	function __isset($var) {
		if ($var === 'thisown') return true;
		return array_key_exists($var, $this->_pData);
	}
	function __construct($h) {
		$this->_cPtr=$h;
	}
}

class AngleRequest extends Request {
	public $_cPtr=null;

	function __set($var,$value) {
		if ($var === 'thisown') return swig_VSidoConnect_alter_newobject($this->_cPtr,$value);
		Request::__set($var,$value);
	}

	function __get($var) {
		if ($var === 'thisown') return swig_VSidoConnect_get_newobject($this->_cPtr);
		return Request::__get($var);
	}

	function __isset($var) {
		if ($var === 'thisown') return true;
		return Request::__isset($var);
	}

	function __construct($res=null) {
		if (is_resource($res) && get_resource_type($res) === '_p_VSido__AngleRequest') {
			$this->_cPtr=$res;
			return;
		}
		$this->_cPtr=new_AngleRequest();
	}

	function cycle($cyc) {
		AngleRequest_cycle($this->_cPtr,$cyc);
	}

	function angle($arg1,$deg) {
		AngleRequest_angle($this->_cPtr,$arg1,$deg);
	}
}

class ComplianceRequest extends Request {
	public $_cPtr=null;

	function __set($var,$value) {
		if ($var === 'thisown') return swig_VSidoConnect_alter_newobject($this->_cPtr,$value);
		Request::__set($var,$value);
	}

	function __get($var) {
		if ($var === 'thisown') return swig_VSidoConnect_get_newobject($this->_cPtr);
		return Request::__get($var);
	}

	function __isset($var) {
		if ($var === 'thisown') return true;
		return Request::__isset($var);
	}

	function __construct($res=null) {
		if (is_resource($res) && get_resource_type($res) === '_p_VSido__ComplianceRequest') {
			$this->_cPtr=$res;
			return;
		}
		$this->_cPtr=new_ComplianceRequest();
	}

	function comp($sid,$cp1,$cp2) {
		ComplianceRequest_comp($this->_cPtr,$sid,$cp1,$cp2);
	}
}

class MinMaxAngleRequest extends Request {
	public $_cPtr=null;

	function __set($var,$value) {
		if ($var === 'thisown') return swig_VSidoConnect_alter_newobject($this->_cPtr,$value);
		Request::__set($var,$value);
	}

	function __get($var) {
		if ($var === 'thisown') return swig_VSidoConnect_get_newobject($this->_cPtr);
		return Request::__get($var);
	}

	function __isset($var) {
		if ($var === 'thisown') return true;
		return Request::__isset($var);
	}

	function __construct($res=null) {
		if (is_resource($res) && get_resource_type($res) === '_p_VSido__MinMaxAngleRequest') {
			$this->_cPtr=$res;
			return;
		}
		$this->_cPtr=new_MinMaxAngleRequest();
	}

	function minMax($sid,$min,$max) {
		MinMaxAngleRequest_minMax($this->_cPtr,$sid,$min,$max);
	}
}

class ServoInfoRequest extends Request {
	public $_cPtr=null;

	function __set($var,$value) {
		if ($var === 'thisown') return swig_VSidoConnect_alter_newobject($this->_cPtr,$value);
		Request::__set($var,$value);
	}

	function __get($var) {
		if ($var === 'thisown') return swig_VSidoConnect_get_newobject($this->_cPtr);
		return Request::__get($var);
	}

	function __isset($var) {
		if ($var === 'thisown') return true;
		return Request::__isset($var);
	}

	function __construct($res=null) {
		if (is_resource($res) && get_resource_type($res) === '_p_VSido__ServoInfoRequest') {
			$this->_cPtr=$res;
			return;
		}
		$this->_cPtr=new_ServoInfoRequest();
	}

	function info($sid,$address,$length) {
		ServoInfoRequest_info($this->_cPtr,$sid,$address,$length);
	}
}

class Response {
	public $_cPtr=null;
	protected $_pData=array();

	function __set($var,$value) {
		if ($var === 'thisown') return swig_VSidoConnect_alter_newobject($this->_cPtr,$value);
		$this->_pData[$var] = $value;
	}

	function __get($var) {
		if ($var === 'thisown') return swig_VSidoConnect_get_newobject($this->_cPtr);
		return $this->_pData[$var];
	}

	function __isset($var) {
		if ($var === 'thisown') return true;
		return array_key_exists($var, $this->_pData);
	}
	function __construct($h) {
		$this->_cPtr=$h;
	}

	function JSON() {
		return Response_JSON($this->_cPtr);
	}
}

class AckResponse extends Response {
	public $_cPtr=null;

	function __set($var,$value) {
		if ($var === 'thisown') return swig_VSidoConnect_alter_newobject($this->_cPtr,$value);
		Response::__set($var,$value);
	}

	function __get($var) {
		if ($var === 'thisown') return swig_VSidoConnect_get_newobject($this->_cPtr);
		return Response::__get($var);
	}

	function __isset($var) {
		if ($var === 'thisown') return true;
		return Response::__isset($var);
	}

	function __construct($uart,$req) {
		if (is_resource($uart) && get_resource_type($uart) === '_p_VSido__AckResponse') {
			$this->_cPtr=$uart;
			return;
		}
		$this->_cPtr=new_AckResponse($uart,$req);
	}

	function msg() {
		return AckResponse_msg($this->_cPtr);
	}

	function JSON() {
		return AckResponse_JSON($this->_cPtr);
	}
}

class ServoInfoResponse extends Response {
	public $_cPtr=null;

	function __set($var,$value) {
		if ($var === 'thisown') return swig_VSidoConnect_alter_newobject($this->_cPtr,$value);
		Response::__set($var,$value);
	}

	function __get($var) {
		if ($var === 'thisown') return swig_VSidoConnect_get_newobject($this->_cPtr);
		return Response::__get($var);
	}

	function __isset($var) {
		if ($var === 'thisown') return true;
		return Response::__isset($var);
	}

	function __construct($uart,$req) {
		if (is_resource($uart) && get_resource_type($uart) === '_p_VSido__ServoInfoResponse') {
			$this->_cPtr=$uart;
			return;
		}
		$this->_cPtr=new_ServoInfoResponse($uart,$req);
	}

	function JSON() {
		return ServoInfoResponse_JSON($this->_cPtr);
	}

	function info() {
		return ServoInfoResponse_info($this->_cPtr);
	}
}


?>
