<?php

//
// Ziszap Portal System Objects.
//
// Sistema de gestion de usuarios en el portal.
// Llamadas gen�ricas de acceso a los usuarios: verificacion de contraseñas, pertenencia a grupos, ...
//
	require_once ("zz-object/authdb/init.php");
	
	function is_user_allow ($users) {
		global $sesion;
		
		$username = $sesion->whoami();
		
		foreach ($users as $key => $user) {
			if ($username === $user) {
				return TRUE;
			}
		}
		return FALSE;
	}
	
	function is_user_belong_group ($groups) {
		global $sesion;
		
		$auth = new authdb();
		foreach ($groups as $key => $group) {
			$data_in = Array();
			$data_out = Array();
			
			$data_in["usulgn"] = $sesion->whoami();
			$data_in["grpnam"] = $group;
			if ($auth->is_user_to_group($data_in, $data_out)) {
				return TRUE;
			}
		}
		return FALSE;
	}
?>