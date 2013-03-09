<?

//
// Nynpha Multimedia Portal System Directories.
//
// Sistema de directorios. API para el acceso a través del portal de todos los accesos
// a traves de LDAP, en concretp OpenLDAP 2.3.
//
// Propiedad de Mankind Capital, S.L -- Todos los derechos reservados
// Nynpha Multimedia y sus respectivos logos son marcas registradas de Mankind Capital SL
//

class directory_object {

	var $classname = "directory_object";
	var $ldap_root = "cn=root,o=mk";
	var $ldap_passwd = "zip08970";
	var $ldap_base = "o=mk";
	var $ldap_host;
	var $ds;

	function directory_object($server_data){
		global $ds, $ldap_root, $ldap_passwd;
		$this->ldap_host = $server_data;
		$this->ds = @ldap_connect($server_data) or dir_ldap_die(ldap_error()." intentamdo conectarse al directorio");  // must be a valid LDAP server!
		$r = @ldap_bind($this->ds, $this->ldap_root, $this->ldap_passwd) or dir_ldap_die(ldap_error()." intentando autentificarse en el directorio");
		ldap_set_option($this->ds, LDAP_OPT_PROTOCOL_VERSION, 3);
	}

	function serialize($option_serialize, $params_serialize, &$data_serialize){
		if ($option_serialize == "output"){
			$sr = @ldap_search($this->ds, $params_serialize["serialize.dn"], $params_serialize["serialize.condition"]) or $this->dir_ldap_die(ldap_error($this->ds).".Haciendo la busqueda en [".$params_serialize["serialize.dn"]."] con la condición: ".$params_serialize["serialize.condition"]);
			$data_serialize = @ldap_get_entries($this->ds, $sr) or $this->dir_ldap_die(ldap_error($this->ds)." intentando obtener las entradas del directorio");
		}
		else if ($option_serialize == "output.container"){
			$sr = @ldap_list($this->ds, $params_serialize["serialize.dn"], $params_serialize["serialize.condition"]) or $this->dir_ldap_die(ldap_error($this->ds).".Haciendo la busqueda en [".$params_serialize["serialize.dn"]."] con la condición: ".$params_serialize["serialize.condition"]);
			$data_serialize = @ldap_get_entries($this->ds, $sr) or $this->dir_ldap_die(ldap_error()." intentando obtener las entradas del directorio");
		}
		else if ($option_serialize == "publish"){
			$sr = @ldap_add($this->ds, $params_serialize["serialize.dn"], $data_serialize) or $this->dir_ldap_die(ldap_error($this->ds).".Haciendo la inserción en [".$params_serialize["serialize.dn"]."]");
		}
		else if ($option_serialize == "update"){
			$sr=@ldap_modify($this->ds, $params_serialize["serialize.dn"], $data_serialize) or $this->dir_ldap_die(ldap_error($this->ds).".Haciendo la modificación en [".$params_serialize["serialize.dn"]."]");
			if (($params_serialize["serialize.newdn"] != "") && ($params_serialize["serialize.newparent"] != ""))
				$sr=@ldap_rename($this->ds, $params_serialize["serialize.dn"], $params_serialize["serialize.newdn"], $params_serialize["serialize.newparent"], true) or $this->dir_ldap_die(ldap_error($this->ds).".Haciendo la modificación del RDN en [".$params_serialize["serialize.dn"]."]");
		}
	}

	function dir_ldap_die($strLDAPError)
	{

		echo "<br><br>";
		echo "<table border='0' cellpadding='1' cellspacing='1' width='100%'><tr><td bgcolor='#514537'>";
		echo "<table border='0' cellpadding='5' cellspacing='0' width='100%'>";
		echo "<tr bgcolor='#EEEEEE'>";
		echo "<td width='20' valign='top'><img src='rcrs/gif/error.gif' hspace='3'></td>";
		echo "<td valign='middle'>";
		echo "<font face='Verdana,Arial' size=2><b>$strLDAPError</b></font><br><br>";
		echo "</td>";
		echo "</tr></table>";
		echo "</td></tr></table>";
		echo "<br><br>";

		exit;
	}

	function _directory_object(){
		ldap_close($this->ds);
	}
}
?>
