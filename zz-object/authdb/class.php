<?php 
require_once "data_object/i.object.inc";

class h_authdb extends Object {
	var $classname = "h_authdb";

	protected	$usuid;	/** @var int ID usuario */
	protected	$usulgn;	/** @var varchar Nombre del usuario */
	protected	$usupwd;	/** @var varchar Password del usuario */
	protected	$usuema;	/** @var varchar E-Mail del usuario registrado */

	protected	$tables = Array ('zUser' => 'usuid,usulgn,usupwd,usuema');
	protected	$pKey_tables = Array ('zUser' => 'usuid');
	protected	$pExt_tables = Array ();
	protected	$fieldsRequired = Array ('usuid','usulgn','usupwd','usuema');
	protected	$fieldsSize = Array ('usulgn' => 30,'usupwd' => 50,'usuema' => 40);
	protected	$fieldsValidate = Array ('usuid' => 'int','usulgn' => 'varchar','usupwd' => 'varchar','usuema' => 'varchar');
	protected	$fieldsText = Array ('usuid' => 'ID usuario','usulgn' => 'Nombre del usuario','usupwd' => 'Password del usuario','usuema' => 'E-Mail del usuario registrado');
	protected	$ext_form;

	function authdb () {
		$this->ext_form = new ValidateForm();
		$this->ext_form->tables = $this->tables;
		$this->ext_form->fieldsRequired = $this->fieldsRequired;
		$this->ext_form->fieldsSize = $this->fieldsSize;
		$this->ext_form->fieldsValidate = $this->fieldsValidate;
	}

	protected function flush_data () { 
		if (isset($this->usuid)) unset($this->usuid);
		if (isset($this->usulgn)) unset($this->usulgn);
		if (isset($this->usupwd)) unset($this->usupwd);
		if (isset($this->usuema)) unset($this->usuema);
	}

	function get_data ( &$data_out ) { 
		if (isset($this->usuid)) $data_out["usuid"] = $this->usuid;
		if (isset($this->usulgn)) $data_out["usulgn"] = $this->usulgn;
		if (isset($this->usupwd)) $data_out["usupwd"] = $this->usupwd;
		if (isset($this->usuema)) $data_out["usuema"] = $this->usuema;
	}

	function set_data ( $data_in ) { 
		if (isset($data_in["usuid"])) $this->usuid = $data_in["usuid"];
		if (isset($data_in["usulgn"])) $this->usulgn = $data_in["usulgn"];
		if (isset($data_in["usupwd"])) $this->usupwd = $data_in["usupwd"];
		if (isset($data_in["usuema"])) $this->usuema = $data_in["usuema"];
	}

	function auth_user( $data_in, &$data_out ) {
		// TODO: Funcion auth_user
		$this->flush_data();
		$this->begin_transaction();
		/** Verify INPUT usulgn,usupwd  */
		$fieldsRequiredFunc = Array ('usulgn' => 'Nombre del usuario','usupwd' => 'Password del usuario');
		if ($this->create_html_input($fieldsRequiredFunc, $data_in)) {
			$this->end_transaction();
			return FALSE;
		}
		$fieldsSQL = Array ('usulgn');
		if (! $this->sql_scan ($fieldsSQL, "zUser", $data_in)) {
			$this->end_transaction();
			return FALSE;
		}
		$this->login( $data_in, $data_out );
		$fieldsSQL = Array ('usulgn','usupwd');
		$this->end_transaction();
		return TRUE;
	}

	function lost_pwd_user( $data_in, &$data_out ) {
		// TODO: Funcion lost_pwd_user
		$this->flush_data();
		$this->begin_transaction();
		/** Verify INPUT usulgn,usuema  */
		$fieldsRequiredFunc = Array ('usulgn' => 'Nombre del usuario','usuema' => 'E-Mail del usuario registrado');
		if ($this->create_html_input($fieldsRequiredFunc, $data_in)) {
			$this->end_transaction();
			return FALSE;
		}
		$fieldsSQL = Array ('usuema');
		if (! $this->sql_scan ($fieldsSQL, "zUser", $data_in)) {
			$this->end_transaction();
			return FALSE;
		}
		$this->lostpwd( $data_in, $data_out );
		$fieldsSQL = Array ('usulgn','usuema');
		$this->end_transaction();
		return TRUE;
	}

	function new_user( $data_in, &$data_out ) {
		// TODO: Funcion new_user
		$this->flush_data();
		$this->begin_transaction();
		/** Verify INPUT usulgn,usupwd,usuema  */
		$fieldsRequiredFunc = Array ('usulgn' => 'Nombre del usuario','usupwd' => 'Password del usuario','usuema' => 'E-Mail del usuario registrado');
		if ($this->create_html_input($fieldsRequiredFunc, $data_in)) {
			$this->end_transaction();
			return FALSE;
		}
		$fieldsSQL = Array ('usulgn');
		if (! $this->sql_scan ($fieldsSQL, "zUser", $data_in)) {
			$this->end_transaction();
			return FALSE;
		}
		$this->create_object( $data_in, $data_out );
		$fieldsSQL = Array ('usulgn','usupwd','usuema');
		if (! $this->sql_create ($fieldsSQL, "zUser", $data_out)) {
			$this->end_transaction();
			return FALSE;
		}
		$this->end_transaction();
		return TRUE;
	}

	function logout( $data_in, &$data_out ) {
		// TODO: Funcion logout
		$this->flush_data();
		$this->begin_transaction();
		$fieldsSQL = Array ('usulgn');
		if (! $this->sql_read ($fieldsSQL, "zUser", $data_in)) {
			$this->end_transaction();
			return FALSE;
		}
		$this->logout( $data_in, $data_out );
		$this->end_transaction();
		return TRUE;
	}

}
?>
