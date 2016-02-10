<?php 
require_once "data_object/i.object.inc";

class z_session extends Object {
	var $classname = "z_session";

	protected	$ses_id;	/** @var varchar ID sessio */
	protected	$usulgn;	/** @var varchar Nombre del usuario */

	protected	$tables = Array ('SESSION' => 'ses_id,usulgn');
	protected	$pKey_tables = Array ('SESSION' => 'ses_id');
	protected	$pExt_tables = Array ();
	protected	$fieldsRequired = Array ('ses_id','usulgn');
	protected	$fieldsSize = Array ('ses_id' => 30,'usulgn' => 30);
	protected	$fieldsValidate = Array ('ses_id' => 'varchar','usulgn' => 'varchar');
	protected	$fieldsText = Array ('ses_id' => 'ID sessio','usulgn' => 'Nombre del usuario');
	protected	$ext_form;

	function session () {
		$this->ext_form = new ValidateForm();
		$this->ext_form->tables = $this->tables;
		$this->ext_form->fieldsRequired = $this->fieldsRequired;
		$this->ext_form->fieldsSize = $this->fieldsSize;
		$this->ext_form->fieldsValidate = $this->fieldsValidate;
	}

	protected function flush_data () { 
		if (isset($this->ses_id)) unset($this->ses_id);
		if (isset($this->usulgn)) unset($this->usulgn);
	}

	function get_data ( &$data_out ) { 
		if (isset($this->ses_id)) $data_out["ses_id"] = $this->ses_id;
		if (isset($this->usulgn)) $data_out["usulgn"] = $this->usulgn;
	}

	function set_data ( $data_in ) { 
		if (isset($data_in["ses_id"])) $this->ses_id = $data_in["ses_id"];
		if (isset($data_in["usulgn"])) $this->usulgn = $data_in["usulgn"];
	}

	function register_session( $data_in, &$data_out ) {

		// AUTO: Funcion register_session
		$this->flush_data();
		$this->begin_transaction();
		/** Verify INPUT ses_id,usulgn  */
		$SQL["field"] = Array ('ses_id' => 'ID sessio','usulgn' => 'Nombre del usuario');
		if ($this->create_html_input($SQL["field"], $data_in)) {
			$this->failed_transaction();
			return FALSE;
		}
		if (! $this->create_object( $data_in, $data_out )) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('ses_id','usulgn');
		$SQL["table"] = Array ('SESSION');
		if (! $this->sql_create ($SQL, $data_out)) {
			$this->failed_transaction();
			return FALSE;
		}
		$this->end_transaction();
		return TRUE;
	}

	function update_session( $data_in, &$data_out ) {

		// AUTO: Funcion update_session
		$this->flush_data();
		$this->begin_transaction();
		/** Verify INPUT ses_id,usulgn  */
		$SQL["field"] = Array ('ses_id' => 'ID sessio','usulgn' => 'Nombre del usuario');
		if ($this->create_html_input($SQL["field"], $data_in)) {
			$this->failed_transaction();
			return FALSE;
		}
		if (! $this->create_object( $data_in, $data_out )) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('ses_id','usulgn');
		$SQL["table"] = Array ('SESSION');
		$this->end_transaction();
		return TRUE;
	}

	function is_registered_session( $data_in, &$data_out ) {

		// AUTO: Funcion is_registered_session
		$this->flush_data();
		$this->begin_transaction();
		/** Verify INPUT ses_id  */
		$SQL["field"] = Array ('ses_id' => 'ID sessio');
		if ($this->create_html_input($SQL["field"], $data_in)) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('ses_id','usulgn');
		$SQL["table"] = Array ('SESSION');
		if (! $this->sql_read ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}
		if (! $this->view_object( $data_in, $data_out )) {
			$this->failed_transaction();
			return FALSE;
		}
		$this->end_transaction();
		return TRUE;
	}

	function unregister_session( $data_in, &$data_out ) {

		// AUTO: Funcion unregister_session
		$this->flush_data();
		$this->begin_transaction();
		/** Verify INPUT ses_id  */
		$SQL["field"] = Array ('ses_id' => 'ID sessio');
		if ($this->create_html_input($SQL["field"], $data_in)) {
			$this->failed_transaction();
			return FALSE;
		}
		if (! $this->delete_object( $data_in, $data_out )) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('ses_id');
		$SQL["table"] = Array ('SESSION');
		if (! $this->sql_delete ($SQL, $data_out)) {
			$this->failed_transaction();
			return FALSE;
		}
		$this->end_transaction();
		return TRUE;
	}

}
?>
