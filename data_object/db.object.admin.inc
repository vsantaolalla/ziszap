<?PHP

//
// Nynpha Multimedia Portal System Objects.
//
// Sistema de objetos genericos de database con sistema de administracion.
// Llamadas genéricas de base de datos, de forma que se llama al objeto genérico de cada base de datos de data_object
//
// Propiedad de Mankind Capital, S.L -- Todos los derechos reservados
// Nynpha Multimedia y sus respectivos logos son marcas registradas de Mankind Capital SL
//

require_once("data_object/db.object.inc");

class generical_object_admin_database extends generical_object_database {
	var $classname = "generical_object_admin_database";
	var $l_moderator;
	var $l_admin;
	var $l_create;
	var $l_write;
	var $l_read;
	var $l_delete;

	function generical_object_admin_database () {
		global $PORTAL;

		require_once($PORTAL["file"]["root"]."/data_object/od.file.inc");
		if (file_exists($PORTAL["file"]["modules"]."/".$this->modulename."/data/moderators.txt")) {
			$page_file = new file_object($PORTAL["file"]["modules"]."/".$this->modulename."/data/moderators.txt");
			$params["serialize.s1"] = "\n";
			$page_file->serialize("output",$params,$tokens_file);
			for ($i_vars = 0; $i_vars < $tokens_file["count"]; $i_vars++) {
				if (trim($tokens_file[$i_vars]) != "")
					$this->l_moderator[count($this->l_moderator)] = trim($tokens_file[$i_vars]);
			}
		}
		generical_object_database::generical_object_database();
	}

	function is_moderator() {
		global $vSESSION;

		// Buscamos si encontramos en la lista de moderadores el usuario de la sesión.
		for ($i_vars = 0; $i_vars < count($this->l_moderator); $i_vars++)
			if ($this->l_moderator[$i_vars] == $vSESSION["user"]["login"]) return true;

		// Si no encontramos el usuario en la lista de moderadores retornamos falso
		return false;
	}

}
?>
