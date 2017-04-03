<?php

//
// Ziszap Portal System Objects.
//
// Sistema de generacion de codigo automatica de objetos genericos contra la database.
// Instalación de ziszap, asi como actualización al XML que se refleje en el codigo
//

require_once "data_object/o.module.php";

class install_ziszap extends database_object {
	public $classname = "";
	protected $object_module;

	function __construct () {
		global $db_host;
		global $db_user;
		global $db_passwd;
		global $database_name;

		$path = getcwd();

		$path_zzinterface = $path."/zz-interface/";
		if ((! is_dir($path_zzinterface)) && ($path_zzinterface != "")) {
			mkdir ($path_zzinterface);
		}
		$this->object_module = new object_module();
	}

	function install_object ($object) {
		$path = get_directory();

		if (! is_file($path."/zz-object".$object."/data.xml")) {
			return FALSE;
		}

		$path_object = $path."/zz-object/".$object."/thread/";
		if ((! is_dir($path_object)) && ($path_object != "")) {
			mkdir ($path_object);
		}

		$path_object = $path."/zz-object/".$object."/test/";
		if ((! is_dir($path_object)) && ($path_object != "")) {
			mkdir ($path_object);
		}

		if (! $this->object_module->create_class($object)) return FALSE;
		if (! $this->object_module->create_init($object)) return FALSE;
		if (! $this->object_module->create_class($object, true)) return FALSE;
		return TRUE;
	}

	function install_all_object() {
		$path = get_directory();
		$objects = scandir($path);
		foreach ($objects as $key => $object) {
			if (! in_array($object, array(".","..")))
				if (is_file($path."/".$object."/data.xml"))
					if (! $this->install_object($object)) return FALSE;
		}
		return TRUE;
	}
}
?>