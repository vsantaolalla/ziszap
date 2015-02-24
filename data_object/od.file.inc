<?

//
// Nynpha Multimedia Portal System Directories.
//
// Sistema de directorios. API para el acceso a trav�s del portal de todos los accesos
// a traves de LDAP, en concretp OpenLDAP 2.3.
//
// Propiedad de Mankind Capital, S.L -- Todos los derechos reservados
// Nynpha Multimedia y sus respectivos logos son marcas registradas de Mankind Capital SL
//

class file_object {

	var $classname = "file_object";
	var $file_name;

	function file_object($server_data){
		if (!isset($_SERVER["DOCUMENT_ROOT"])) {
			$current_path = $_SERVER["DOCUMENT_ROOT"];
		} else if (!isset($GLOBALS["_SERVER"]["DOCUMENT_ROOT"])) {
			$current_path = getcwd();
		} 
		else {
			$current_path = $GLOBALS["_SERVER"]["DOCUMENT_ROOT"];
		}

		if (file_exists($current_path.$server_data)) {
			$this->file_name = $current_path.$server_data;
		}
		else if (!file_exists($server_data)) $this->file_die("No existe el fichero ".$server_data);
		else $this->file_name = $server_data;
	}

	function file_get_contents() {
		ob_start();
		$retval = @readfile($this->file_name) or $this->file_die("Error intentando abrir el fichero: ".$this->file_name);
		if (false !== $retval) { // no readfile error
			$retval = ob_get_contents();
		}
		ob_end_clean();
		return $retval;
	}

	function file_separa_token(&$Z, $s_split, $i_count) {
		if ($s_split["serialize.s".$i_count] != "") {
			$Z = explode($s_split["serialize.s".$i_count], $Z);
			for($j=0; $j<sizeof($Z); $j++) {
				$X = $Z[$j];
				$this->file_separa_token($X, $s_split, $i_count+1);
				$Z[$j] = $X;
			}
			$Z["count"] = $j;
		}
	}

	function serialize($option_serialize, $params_serialize, &$data_serialize){
		if ($option_serialize == "output"){
			$data_serialize = $this->file_get_contents();
			$this->file_separa_token($data_serialize, $params_serialize, 1);
		}
	}

	function file_die($strFILEError)
	{
		echo "<br><br>";
		$func_backtrace = debug_backtrace();
		echo "<table border='0' cellpadding='1' cellspacing='1' width='100%'><tr><td bgcolor='#514537'>";
		echo "<table border='0' cellpadding='5' cellspacing='0' width='100%'>";
		echo "<tr bgcolor='#EEEEEE'>";
		echo "<td width='20' valign='top'><img src='rcrs/gif/error.gif' hspace='3'></td>";
		echo "<td valign='middle'>";
		echo "<font face='Verdana,Arial' size=2><i>&lt;/data_object/od.file.inc&gt;</i> <b>$strFILEError</b></font><br><br>";
		echo "<font face='Verdana,Arial' size=2><u>Function callback</u>:</font><br>";
		for ($i=0; $i<count($func_backtrace); $i++){
			echo "<font face='Verdana,Arial' size=2><i>&lt;$i&gt;</i> <b>".$func_backtrace[$i]["file"]."=>".$func_backtrace[$i]["function"]."(".$func_backtrace[$i]["line"].")</b>";
			echo " <font color='#666666'> params: ";
			var_dump($func_backtrace[$i]["args"]);
			echo "</font></font><br>";
		}
		echo "</td>";
		echo "</tr></table>";
		echo "</td></tr></table>";
		echo "<br><br>";

		exit;
	}

}
?>
