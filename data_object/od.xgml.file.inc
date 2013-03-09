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

require_once("data_object/od.xml.file.inc");

class xgml_file_object extends xml_file_object{
	var $classname = "xgml_file_object";

//	function xgml_file_object($file_name) {
//		xml_file_object::xml_file_object($file_name);
//	}

	function xgml_die($strFILEError)
	{
		echo "<br><br>";
		echo "<table border='0' cellpadding='1' cellspacing='1' width='100%'><tr><td bgcolor='#514537'>";
		echo "<table border='0' cellpadding='5' cellspacing='0' width='100%'>";
		echo "<tr bgcolor='#EEEEEE'>";
		echo "<td width='20' valign='top'><img src='rcrs/gif/error.gif' hspace='3'></td>";
		echo "<td valign='middle'>";
		echo "<font face='Verdana,Arial' size=2><i>&lt;/data_object/od.xml.file.inc&gt;</i> <b>$strFILEError</b></font><br><br>";
		echo "</td>";
		echo "</tr></table>";
		echo "</td></tr></table>";
		echo "<br><br>";

		exit;
	}

}
?>
