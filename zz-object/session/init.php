<?php 
require_once "zz-object/session/class.php";

class session extends z_session {
	var $classname = "session";
	
	function register_session ($data_in, &$data_out) {
		global $func_debug;
		$arr_out = Array();
		
//		setcookie("project", $data_in["proj"]);
		$_SESSION["project"] = $data_in["proj"];
		$_SESSION["username"] = $data_in["usulgn"];
		$arr_in = $data_in;
//		var_dump($data_in);
//		var_dump($data_out);
		
		$SQL["field"] = Array ('ses_id');
		$SQL["table"] = Array ('SESSION');
		
		if (! $this->sql_read ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}
		if (! $this->view_object( $data_in, $arr_out )) {
			$this->failed_transaction();
			return FALSE;
		}
		
		if (! $arr_out) {
			z_session::register_session($data_in, $data_out);
		} else {
			z_session::update_session($data_in, $data_out);
		}
	}

	function whoami() {
		$data_in = Array();
		$data_out = Array();
		
		$data_in["ses_id"] = session_id();
		$this->is_registered_session($data_in, $data_out);
		return $data_out["usulgn"];
	}

}
?>
