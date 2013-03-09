<?
class data_object {

	var $classname = "data_object";
	var $directory;

	function data_object($dir){
		global $directory;
		$directory = $dir;
	}

	function file_data($file_name){
		ob_start();
		$retval = @readfile($file_name);
		if (false !== $retval) { // no readfile error
			$retval = ob_get_contents();
		}
		ob_end_clean();
	}

	function directory_data($n_elem, $dir, $file, &$data){
		global $directory;
		$name_file = explode(".",$file);
		if (is_dir($dir."/".$file) && ($directory == $dir)){
			$data[$n_elem]["o"][0] = $name_file[1];
		} else if (is_dir($dir."/".$file)){
			$data[$n_elem]["ou"][0] = $name_file[1];
		} else {
			$data[$n_elem]["cn"][0] = $name_file[1];
			/// Aqui se extraen todos los datos del fichero.
			/// La estructura del fichero de datos es: <campo>: datos.
			$linea = explode("\n",file_data($dir."/".$file));
			asort($linea);

			$campo_num = 0;
			$campo_ant = "";
			foreach ($linea as $elem_linea){
				list($campo,$data) = explode(":",$elem_linea);
				if ($campo == $campo_ant){
					$campo_num = $campo_num + 1;
					$data[$n_elem][$campo][$campo_num] = $data;
				} else {
					$campo_ant = $campo;
					$campo_num = 0;
					$data[$n_elem][$campo][0] = $data;
				}
			}
		}
		for ($i=2; $i<=count($name_file); $i++)
			$data[$n_elem]["objectclass"][$i-2] = $name_file[$i];

	}

	function directory_search($directory, $condition, &$data){
		$handle = opendir($directory);
		$elem = 0;
		while ($file = readdir($handle)) {
			$elem = $elem + 1;
			directory_data($elem, $directory, $file, $data);
		}
		closedir($handle);
	}

	function serialize($option_serialize, $params_serialize, &$data_serialize){
		global $ds;
		$parts_dn = explode(".", $params_serialize["serialize.dn"]);
		if (count($parts_dn) > 1){
			$dn_complete = "";
			for ($i=1; $i <= count($parts_dn); $i++)
				$dn_complete = $dn_complete . "/" . $parts_dn[$i];
		}
		else $dn_complete = $params_serialize["serialize.dn"];
		if ($option_serialize == "output"){
    	directory_search($dn_complete, $params_serialize["serialize.conditions"], $data_serialize);
		}
	}
}
?>
