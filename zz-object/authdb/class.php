<?php 
require_once "data_object/i.object.inc";

class z_authdb extends Object {
	var $classname = "z_authdb";

	protected	$usuid;		/** @var int ID usuario */
	protected	$usulgn;	/** @var varchar Nombre del usuario */
	protected	$usupwd;	/** @var varchar Password del usuario */
	protected	$usuema;	/** @var varchar E-Mail del usuario registrado */
	protected	$grpid;		/** @var int ID grupo */
	protected	$grpnam;	/** @var varchar Nombre del grupo */
	protected	$grpmem;	/** @var text Miembros del grupo */

	protected	$tables = Array ('zUser' => 'usuid,usulgn,usupwd,usuema','zGroup' => 'grpid,grpnam,grpmem');
	protected	$pKey_tables = Array ('zUser' => 'usuid','zGroup' => 'grpid');
	protected	$pExt_tables = Array ();
	protected	$fieldsRequired = Array ('usuid','usulgn','usupwd','usuema','grpid','grpnam','grpmem');
	protected	$fieldsSize = Array ('usulgn' => 30,'usupwd' => 50,'usuema' => 40,'grpnam' => 30);
	protected	$fieldsValidate = Array ('usuid' => 'int','usulgn' => 'varchar','usupwd' => 'varchar','usuema' => 'varchar','grpid' => 'int','grpnam' => 'varchar','grpmem' => 'text');
	protected	$fieldsText = Array ('usuid' => 'ID usuario','usulgn' => 'Nombre del usuario','usupwd' => 'Password del usuario','usuema' => 'E-Mail del usuario registrado','grpid' => 'ID grupo','grpnam' => 'Nombre del grupo','grpmem' => 'Miembros del grupo');
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
		if (isset($this->grpid)) unset($this->grpid);
		if (isset($this->grpnam)) unset($this->grpnam);
		if (isset($this->grpmem)) unset($this->grpmem);
	}

	function get_data ( &$data_out ) { 
		if (isset($this->usuid)) $data_out["usuid"] = $this->usuid;
		if (isset($this->usulgn)) $data_out["usulgn"] = $this->usulgn;
		if (isset($this->usupwd)) $data_out["usupwd"] = $this->usupwd;
		if (isset($this->usuema)) $data_out["usuema"] = $this->usuema;
		if (isset($this->grpid)) $data_out["grpid"] = $this->grpid;
		if (isset($this->grpnam)) $data_out["grpnam"] = $this->grpnam;
		if (isset($this->grpmem)) $data_out["grpmem"] = $this->grpmem;
	}

	function set_data ( $data_in ) { 
		if (isset($data_in["usuid"])) $this->usuid = $data_in["usuid"];
		if (isset($data_in["usulgn"])) $this->usulgn = $data_in["usulgn"];
		if (isset($data_in["usupwd"])) $this->usupwd = $data_in["usupwd"];
		if (isset($data_in["usuema"])) $this->usuema = $data_in["usuema"];
		if (isset($data_in["grpid"])) $this->grpid = $data_in["grpid"];
		if (isset($data_in["grpnam"])) $this->grpnam = $data_in["grpnam"];
		if (isset($data_in["grpmem"])) $this->grpmem = $data_in["grpmem"];
	}

	// Funcion: auth_user ... Conectar el usuario al sistema
	function auth_user( $data_in, &$data_out ) {

		// AUTO: Funcion auth_user
		$this->flush_data();
		$this->begin_transaction();
		/** Verify INPUT usulgn,usupwd  */
		$SQL["field"] = Array ('usulgn' => 'Nombre del usuario','usupwd' => 'Password del usuario');
		if ($this->create_html_input($SQL["field"], $data_in)) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('usulgn');
		$SQL["table"] = Array ('zUser');
		if (! $this->sql_scan ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('usulgn','usupwd');
		if (! $this->sql_read ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}
		if (! $this->login( $data_in, $data_out )) {
			$this->failed_transaction();
			return FALSE;
		}
		$this->end_transaction();
		return TRUE;
	}

	// Funcion: lost_pwd_user ... Recuperar password del usuario
	function lost_pwd_user( $data_in, &$data_out ) {

		// AUTO: Funcion lost_pwd_user
		$this->flush_data();
		$this->begin_transaction();
		/** Verify INPUT usulgn,usuema  */
		$SQL["field"] = Array ('usulgn' => 'Nombre del usuario','usuema' => 'E-Mail del usuario registrado');
		if ($this->create_html_input($SQL["field"], $data_in)) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('usuema');
		$SQL["table"] = Array ('zUser');
		if (! $this->sql_scan ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('usulgn','usuema');
		if (! $this->sql_read ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}
		if (! $this->lostpwd( $data_in, $data_out )) {
			$this->failed_transaction();
			return FALSE;
		}
		$this->end_transaction();
		return TRUE;
	}

	// Funcion: new_user ... Crear usuario
	function new_user( $data_in, &$data_out ) {

		// AUTO: Funcion new_user
		$this->flush_data();
		$this->begin_transaction();
		/** Verify INPUT usulgn,usupwd,usuema  */
		$SQL["field"] = Array ('usulgn' => 'Nombre del usuario','usupwd' => 'Password del usuario','usuema' => 'E-Mail del usuario registrado');
		if ($this->create_html_input($SQL["field"], $data_in)) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('usulgn');
		$SQL["table"] = Array ('zUser');
		if ($this->sql_scan ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}
		if (! $this->create_user( $data_in, $data_out )) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('usulgn','usupwd','usuema');
		if (! $this->sql_create ($SQL, $data_out)) {
			$this->failed_transaction();
			return FALSE;
		}
		$this->end_transaction();
		return TRUE;
	}

	// Funcion: delete_user ... Eliminar usuario
	function delete_user( $data_in, &$data_out ) {

		// AUTO: Funcion delete_user
		$this->flush_data();
		$this->begin_transaction();
		/** Verify INPUT usulgn  */
		$SQL["field"] = Array ('usulgn' => 'Nombre del usuario');
		if ($this->create_html_input($SQL["field"], $data_in)) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('usulgn');
		$SQL["table"] = Array ('zUser');
		if (! $this->sql_scan ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}
		if (! $this->not_find_user_to_group( $data_in, $data_out )) {
			$this->failed_transaction();
			return FALSE;
		}

		if (! $this->sql_delete ($SQL, $data_out)) {
			$this->failed_transaction();
			return FALSE;
		}
		$this->end_transaction();
		return TRUE;
	}

	// Funcion: new_group ... Crear un grupo
	function new_group( $data_in, &$data_out ) {

		// AUTO: Funcion new_group
		$this->flush_data();
		$this->begin_transaction();
		/** Verify INPUT grpnam  */
		$SQL["field"] = Array ('grpnam' => 'Nombre del grupo');
		if ($this->create_html_input($SQL["field"], $data_in)) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('grpnam');
		$SQL["table"] = Array ('zGroup');
		if ($this->sql_scan ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}
		if (! $this->create_object( $data_in, $data_out )) {
			$this->failed_transaction();
			return FALSE;
		}

		if (! $this->sql_create ($SQL, $data_out)) {
			$this->failed_transaction();
			return FALSE;
		}
		$this->end_transaction();
		return TRUE;
	}

	// Funcion: add_user_to_group ... Añadir usuario al grupo
	function add_user_to_group( $data_in, &$data_out ) {

		// AUTO: Funcion add_user_to_group
		$this->flush_data();
		$this->begin_transaction();
		/** Verify INPUT usulgn,grpnam  */
		$SQL["field"] = Array ('usulgn' => 'Nombre del usuario','grpnam' => 'Nombre del grupo');
		if ($this->create_html_input($SQL["field"], $data_in)) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('usulgn');
		$SQL["table"] = Array ('zUser');
		if (! $this->sql_scan ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('grpnam');
		$SQL["table"] = Array ('zGroup');
		if (! $this->sql_scan ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('grpid','grpnam','grpmem');
		if (! $this->sql_read ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}
		if (! $this->add_member_to_list( $data_in, $data_out )) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('grpmem');
		if (! $this->sql_write ($SQL, $data_out)) {
			$this->failed_transaction();
			return FALSE;
		}
		$this->end_transaction();
		return TRUE;
	}

	// Funcion: del_user_to_group ... Eliminar usuario al grupo
	function del_user_to_group( $data_in, &$data_out ) {

		// AUTO: Funcion del_user_to_group
		$this->flush_data();
		$this->begin_transaction();
		/** Verify INPUT usulgn,grpnam  */
		$SQL["field"] = Array ('usulgn' => 'Nombre del usuario','grpnam' => 'Nombre del grupo');
		if ($this->create_html_input($SQL["field"], $data_in)) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('usulgn');
		$SQL["table"] = Array ('zUser');
		if (! $this->sql_scan ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('grpnam');
		$SQL["table"] = Array ('zGroup');
		if (! $this->sql_scan ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('grpid','grpnam','grpmem');
		if (! $this->sql_read ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}
		if (! $this->del_member_to_list( $data_in, $data_out )) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('grpmem');
		if (! $this->sql_write ($SQL, $data_out)) {
			$this->failed_transaction();
			return FALSE;
		}
		$this->end_transaction();
		return TRUE;
	}

	// Funcion: is_user_to_group ... Verificar si el usuario pertenece al grupo
	function is_user_to_group( $data_in, &$data_out ) {

		// AUTO: Funcion is_user_to_group
		$this->flush_data();
		$this->begin_transaction();
		/** Verify INPUT usulgn,grpnam  */
		$SQL["field"] = Array ('usulgn' => 'Nombre del usuario','grpnam' => 'Nombre del grupo');
		if ($this->create_html_input($SQL["field"], $data_in)) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('usulgn');
		$SQL["table"] = Array ('zUser');
		if (! $this->sql_scan ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('grpnam');
		$SQL["table"] = Array ('zGroup');
		if (! $this->sql_scan ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('grpid','grpnam','grpmem');
		if (! $this->sql_read ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}
		if (! $this->is_member_to_list( $data_in, $data_out )) {
			$this->failed_transaction();
			return FALSE;
		}
		$this->end_transaction();
		return TRUE;
	}

	// Funcion: find_user_to_group ... Encontrar un usuario al grupo
	function find_user_to_group( $data_in, &$data_out ) {

		// AUTO: Funcion find_user_to_group
		$this->flush_data();
		$this->begin_transaction();
		/** Verify INPUT usulgn  */
		$SQL["field"] = Array ('usulgn' => 'Nombre del usuario');
		if ($this->create_html_input($SQL["field"], $data_in)) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('usulgn');
		$SQL["table"] = Array ('zUser');
		if (! $this->sql_scan ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('grpnam','grpmem');
		$SQL["table"] = Array ('zGroup');
		if (! $this->sql_read ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}
		if (! $this->find_member_to_list( $data_in, $data_out )) {
			$this->failed_transaction();
			return FALSE;
		}
		$this->end_transaction();
		return TRUE;
	}

	// Funcion: delete_group ... Eliminar el grupo
	function delete_group( $data_in, &$data_out ) {

		// AUTO: Funcion delete_group
		$this->flush_data();
		$this->begin_transaction();
		/** Verify INPUT grpnam  */
		$SQL["field"] = Array ('grpnam' => 'Nombre del grupo');
		if ($this->create_html_input($SQL["field"], $data_in)) {
			$this->failed_transaction();
			return FALSE;
		}

		$SQL["field"] = Array ('grpnam');
		$SQL["table"] = Array ('zGroup');
		if (! $this->sql_scan ($SQL, $data_in) ) {
			$this->failed_transaction();
			return FALSE;
		}
		if (! $this->delete_object( $data_in, $data_out )) {
			$this->failed_transaction();
			return FALSE;
		}

		if (! $this->sql_delete ($SQL, $data_out)) {
			$this->failed_transaction();
			return FALSE;
		}
		$this->end_transaction();
		return TRUE;
	}

	// Funcion: off_user ... Desconetar el usuario
	function off_user( $data_in, &$data_out ) {

		// AUTO: Funcion off_user
		$this->flush_data();
		$this->begin_transaction();
		if (! $this->logout( $data_in, $data_out )) {
			$this->failed_transaction();
			return FALSE;
		}
		$this->end_transaction();
		return TRUE;
	}

}
?>
