<?PHP

//
// Ziszap Portal System Objects.
//
// Sistema de objetos genericos de database.
// Llamadas gen�ricas de base de datos, de forma que se llama al objeto gen�rico
// de cada base de datos de data_object
//

require_once("data_object/ag.object.inc");

class object_module extends generical_object {
//	var $classname = "object_module";

	function object_module () {
		parent::generical_object ();
	}
	
	function create_class ($object) {
		global $clase;
		global $GLOBALS;
		
		$path = $GLOBALS["_SERVER"]["DOCUMENT_ROOT"]."/zz-object/".$object."/init.php";
		$object_class = $object;
		$data = "<?php \n";
		$data .= "require_once \"data_object/i.object.inc\";\n\n";
		if (file_exists($path)) {
			$path = $GLOBALS["_SERVER"]["DOCUMENT_ROOT"]."/zz-object/".$object."/class.php";
			$object_class = "h_".$object;
		}
		
		// Recreamos la variable de $cmd para generar l'objecte.
		$cmd = "class ".$object." extends generical_object {\n\tvar \$classname = \"".$object."\";\n }\n\$clase=new ".$object."();\n";
//		echo $cmd;
		eval($cmd);
		
		$data .= "class ".$object_class." extends Object {\n";
		$data .= "\tvar \$classname = \"".$object_class."\";\n\n";

		foreach ($clase->list_fields() as $key => $value) {
//			print_r($value);
			if (isset($value["TYPE"])) {
				$data .= "\tprotected	\$".$value["NAME"].";";
				$data .= "\t/** @var ".$value["TYPE"]." ".$value["TEXT"]." */\n";
			}
		}
		$data .= "\n";

		$data .= "\tprotected	\$tables = Array (";
		$first_time = TRUE;
		foreach ($clase->list_all_tables() as $key => $value) {
			if (!$first_time){
				$data .= ",";
			} else {
				$first_time = FALSE;
			}
			$data .= "'".$value["NAME"]."' => '".$value["FIELDS"]."'";
		}
		$data .= ");\n";

		$data .= "\tprotected	\$pKey_tables = Array (";
		$first_time = TRUE;
		foreach ($clase->list_all_tables() as $key => $value) {
			if (!$first_time){
				$data .= ",";
			} else {
				$first_time = FALSE;
			}
			$data .= "'".$value["NAME"]."' => '".$value["PKEY"]."'";
		}
		$data .= ");\n";
		
		$data .= "\tprotected	\$pExt_tables = Array (";
		$first_time = TRUE;
		foreach ($clase->list_all_tables() as $key => $value) {
			if (isset($value["EXTERNAL"])) {
				if (!$first_time){
					$data .= ",";
				} else {
					$first_time = FALSE;
				}
				$data .= "'".$value["NAME"]."' => '".$value["EXTERNAL"]."'";
			}
		}
		$data .= ");\n";

		$array_cardinals = Array();
		$array_required = Array();
		$array_validate = Array();
		
		foreach ($clase->list_enum_fields() as $key => $value) {
			$data .= "\tprotected	\$enum_".$key." = Array(";
			$first_time = TRUE;
			foreach ($value as $keyi => $valuei) {
				if (!$first_time){
					$data .= ",";
				} else {
					$first_time = FALSE;
				}
				$data .= "'".$valuei["VALUE"]."' => '".$valuei["TEXT"]."'";
			}
			$data .= "); \n";
//			print_r($value);
		}

		foreach ($clase->list_fields() as $key => $value) {
//			print_r($value);
			if (isset($value["CARDINAL"])) {
				if ($value["TYPE"] == "varchar") 	
					$array_cardinals[$value["NAME"]] = $value["CARDINAL"];
			}
			if (isset($value["NULL"])) {
				if ($value["NULL"] == "no") {
					$array_required[] = $value["NAME"];
				}
			}
			if (isset($value["TYPE"])) {
				$array_validate[$value["NAME"]] = $value["TYPE"];
			}
		}
		$data .= "\tprotected	\$fieldsRequired = Array (";
		$first_time = TRUE;
		foreach ($array_required as $key => $value) {
			if (!$first_time){
				$data .= ",";
			} else {
				$first_time = FALSE;
			}
			$data .= "'".$value."'";
		}
		$data .= ");\n";

		$data .= "\tprotected	\$fieldsSize = Array (";
		$first_time = TRUE;
		foreach ($array_cardinals as $key => $value) {
			if (!$first_time){
				$data .= ",";
			} else {
				$first_time = FALSE;
			}
			$data .= "'".$key."' => ".$value;
		}
		$data .= ");\n";

		$data .= "\tprotected	\$fieldsValidate = Array (";
		$first_time = TRUE;
		foreach ($array_validate as $key => $value) {
			if (!$first_time){
				$data .= ",";
			} else {
				$first_time = FALSE;
			}
			$data .= "'".$key."' => '".$value."'";
		}
		$data .= ");\n";

		$data .= "\tprotected	\$fieldsText = Array (";
		$first_time = TRUE;
		foreach ($clase->list_fields() as $key => $value) {
			if (!$first_time){
				$data .= ",";
			} else {
				$first_time = FALSE;
			}
			$data .= "'".$value["NAME"]."' => '".$value["TEXT"]."'";
		}
		$data .= ");\n";

		$data .= "\tprotected	\$ext_form;\n";

		$data .= "\n";
		
		$data .= "\tfunction ".$object." () {\n\t\t\$this->ext_form = new ValidateForm();\n\t\t\$this->ext_form->tables = \$this->tables;\n";
		$data .= "\t\t\$this->ext_form->fieldsRequired = \$this->fieldsRequired;\n";
		$data .= "\t\t\$this->ext_form->fieldsSize = \$this->fieldsSize;\n\t\t\$this->ext_form->fieldsValidate = \$this->fieldsValidate;\n";
		$data .= "\t}\n\n";

		$data .= "\tprotected function flush_data () { \n";
		foreach ($clase->list_fields() as $key => $value) {
			$data .= "\t\tif (isset(\$this->".$value["NAME"].")) unset(\$this->".$value["NAME"].");\n";
		}
		$data .= "\t}\n\n";
		
		$data .= "\tfunction get_data ( &\$data_out ) { \n";
		foreach ($clase->list_fields() as $key => $value) {
			$data .= "\t\tif (isset(\$this->".$value["NAME"].")) \$data_out[\"".$value["NAME"]."\"] = \$this->".$value["NAME"].";\n";
		}
		$data .= "\t}\n\n";

		$data .= "\tfunction set_data ( \$data_in ) { \n";
		foreach ($clase->list_fields() as $key => $value) {
			$data .= "\t\tif (isset(\$data_in[\"".$value["NAME"]."\"])) \$this->".$value["NAME"]." = \$data_in[\"".$value["NAME"]."\"];\n";
		}
		$data .= "\t}\n\n";

		foreach ($clase->list_methods() as $key => $value) {
			$data .= "\tfunction ".$value["NAME"]."( \$data_in, &\$data_out ) {\n";

			$list_global_vars = Array();
			$list_global_vars = $this->def_method_arrays($value);
			foreach ($list_global_vars as $key_lgv => $value_lgv) {
				$data .= "\t\tglobal \$".$key_lgv.";\n\n";
			}

			$data .= "\t\t// TODO: Funcion ".$value["NAME"]."\n";
			$data .= "\t\t\$this->flush_data();\n";
			$data .= "\t\t\$this->begin_transaction();\n";
			// Aqui procesamos las entradas input
			for ($index = 0; isset($value[$index]); $index++) {
				$func_method = $value[$index];
				if ($func_method["NAME"] == "INPUT") {
					$data .= $this->def_method_input ($func_method["FIELD"]);
					$data .= "\t\tif (\$this->create_html_input(\$fieldsRequiredFunc, \$data_in)) {\n";
					$data .= "\t\t\t\$this->failed_transaction();\n";
					$data .= "\t\t\treturn FALSE;\n\t\t}\n";
				} else if ($func_method["NAME"] == "STORE_INPUT") {
					$data .= $this->def_method_store_input ($func_method["FIELD"],$func_method["TABLE"],$func_method["ACTION"]);
				} else if ($func_method["NAME"] == "GET_VAR") {
					$data .= $this->def_method_get_var($func_method["FIELDS"], $func_method["ARRAY"]);
				} else if ($func_method["NAME"] == "STORE_OUTPUT") {
					continue;
				} else if ($func_method["NAME"] == "SET_VAR") {
					continue;
				} else if ($func_method["NAME"] == "OUTPUT") {
					continue;
				}
//				else print_r($value[$index]);
			}
			// Aqui ejecutamos el proceso una vez tenemos todos los datos en INPUT
			$data .= "\t\tif (! \$this->".$value["FUNCTION"]."( \$data_in, \$data_out )) {\n";
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";

			// Aqui procesamos las entradas output
			for ($index = 0; isset($value[$index]); $index++) {
				$func_method = $value[$index];
				if ($func_method["NAME"] == "INPUT") {
					continue;
				} else if ($func_method["NAME"] == "STORE_INPUT") {
					continue;
				} else if ($func_method["NAME"] == "GET_VAR") {
					continue;
				} else if ($func_method["NAME"] == "STORE_OUTPUT") {
					$data .= $this->def_method_store_output ($func_method["FIELD"],$func_method["TABLE"],$func_method["ACTION"]);
				} else if ($func_method["NAME"] == "SET_VAR") {
					$data .= $this->def_method_set_var($func_method["FIELDS"], $func_method["ARRAY"]);
				} else if ($func_method["NAME"] == "OUTPUT") {
					$data .= $this->def_method_output ($func_method["FIELD"]);
				}
//				else print_r($value[$index]);
			}
			$data .= "\t\t\$this->end_transaction();\n";
			$data .= "\t\treturn TRUE;\n";
			$data .= "\t}\n\n";
		}
		
		$data .= "}\n?>\n";
		echo $data;
		file_put_contents($path, $data);
	}

	function create_test ($object) {
		global $GLOBALS;
		
		$path = $GLOBALS["_SERVER"]["DOCUMENT_ROOT"]."/zz-object/".$object."/test.php";
		if (! class_exists($object)) { 
			// Recreamos la variable de $cmd para generar l'objecte.
			$cmd = "class ".$object." extends generical_object {\n\tvar \$classname = \"".$object."\";\n }\n";
			eval($cmd);
		}
		if (! class_exists("clase")) {
			$cmd = "\$clase=new ".$object."();\n";
			eval($cmd);
		}

		$data =  "<html><title>".$object." test</title>\n\n";
		$data .= "<?php \n";
		$data .= "\trequire_once \"zz-content/header.form.php\";\n";
		$data .= "\tinclude \"zz-object/".$object."/init.php\";\n";
		$data .= "\trequire_once \"data_object/o.validate.form.inc\";\n\n";
		$data .= "\t\$data_in = \$_GET;\n";
		$data .= "\t\$data_out = Array();\n\n";
		$data .= "\tif (isset(\$data_in[\"func\"])) {\n";
		$data .= "\t\t\$T = new ".$object."();\n";

		foreach ($clase->list_methods() as $key => $value) {
			$data .= "\t\tif (\$data_in[\"func\"] == \"".$value["NAME"]."\") {\n";
			$data .= "\t\t\t\$T->".$value["NAME"]."(\$data_in, \$data_out);\n";
			$data .= "\t\t}\n";
		}
		
		$data .= "?>\n";
		$data .= "\t<input type=\"hidden\" name=\"func\" value=\"<?php echo \$data_in[\"func\"]; ?>\" />\n\n";
		$data .= "<?php\n\t}\n";
		$data .= "\trequire_once \"zz-content/footer.form.php\";\n?>\n";	

		foreach ($clase->list_methods() as $key => $value) {
			$data .= "\t<a href=\"?func=".$value["NAME"]."\">".$value["NAME"]."</a><br>\n";
		}
		
		$data .= "\n</html>\n";
		echo $data;
		file_put_contents($path, $data);
	}
	
	function shuttle($object, $method, $p_input, &$p_output) {
		$arr_method = Array();
		
//		$object = new database_object("","","","");	
		foreach ($object->list_methods() as $key => $value) {
			if ($value["NAME"] == $method) {
				$arr_method = $value;
			}
		}
//		print_r($arr_method);
		
		for ($index = 0; isset($arr_method[$index]); $index++) {
			$method_func = $arr_method[$index];
//			print_r($method_func);
			if ($method_func["NAME"] == "INPUT") {
				$this->method_input(explode(",",$method_func["FIELD"]),$p_input);
			}
			if ($method_func["NAME"] == "OUTPUT") {}
			if ($method_func["NAME"] == "STORE_INPUT") {}
			if ($method_func["NAME"] == "STORE_OUTPUT") {}  
			if ($method_func["NAME"] == "METHOD_INCLUDE") {}  
		}
	}
	
	function def_method_input ($fields) {
//		print_r($fields);
		global $clase;
		
		$data = "\t\t/** Verify INPUT ".$fields."  */\n";
		$data .= "\t\t\$fieldsRequiredFunc = Array (";
		$fields_array = explode(",", $fields);
		$first_time = TRUE;
		foreach ($fields_array as $key => $value) {
			if (!$first_time){
				$data .= ",";
			} else {
				$first_time = FALSE;
			}
			$now_field = $clase->get_field($value);
			$data .= "'".$value."' => '".$now_field["TEXT"]."'";
		}
		$data .= ");\n";
		return $data;
	}
	
	function def_method_store_input ($fields, $table, $action) {

		$data = "\t\t\$fieldsSQL = Array (";
		$fields_array = explode(",", $fields);
		$first_time = TRUE;
		foreach ($fields_array as $key => $value) {
			if (!$first_time){
				$data .= ",";
			} else {
				$first_time = FALSE;
			}
			$data .= "'".$value."'";
		}
		$data .= ");\n";
		// Aqui definim diferents comandes per diferents accions.
		if ($action == "scan") {
			$data .= "\t\tif (! \$this->sql_scan (\$fieldsSQL, \"".$table."\", \$data_in)) {\n";
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";
		} else if ($action == "noscan") {
			$data .= "\t\tif (\$this->sql_scan (\$fieldsSQL, \"".$table."\", \$data_in)) {\n";
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";
		} else if ($action == "read") {
			$data .= "\t\tif (! \$this->sql_read (\$fieldsSQL, \"".$table."\", \$data_in)) {\n";
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";
		}
		return $data;
	}
	
	function def_method_store_output ($fields, $table, $action) {

		$data = "\t\t\$fieldsSQL = Array (";
		$fields_array = explode(",", $fields);
		$first_time = TRUE;
		foreach ($fields_array as $key => $value) {
			if (!$first_time){
				$data .= ",";
			} else {
				$first_time = FALSE;
			}
			$data .= "'".$value."'";
		}
		$data .= ");\n";
		// Aqui definim diferents comandes per diferents accions.
		if ($action == "create") {
			$data .= "\t\tif (! \$this->sql_create (\$fieldsSQL, \"".$table."\", \$data_out)) {\n";
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";
		} else if ($action == "write") {
			$data .= "\t\tif (! \$this->sql_write (\$fieldsSQL, \"".$table."\", \$data_out)) {\n";
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";
		} else if ($action == "delete") {
			$data .= "\t\tif (! \$this->sql_delete (\$fieldsSQL, \"".$table."\", \$data_out)) {\n";
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";
		}
		return $data;
	}

	function def_method_output ($fields) {
		
	}

	function def_method_get_var ($fields, $namearray) {
		$list_fields = explode(",", $fields);
		$data = "";
		foreach ($list_fields as $key => $value) {
			$data .= "\t\tif (! isset(\$".$namearray."[\"".$value."\"])) {\n";
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t} else {\n";
			$data .= "\t\t\t\$data_in[\"".$value."\"] = \$".$namearray."[\"".$value."\"];\n\t\t}\n";
		}
		return $data;
	}
	
	function def_method_set_var ($fields, $namearray) {
		$list_fields = explode(",", $fields);
		$data = "";
		foreach ($list_fields as $key => $value) {
			$data .= "\t\tif (! isset(\$data_out[\"".$value."\"])) {\n";
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t} else {\n";
			$data .= "\t\t\t\$".$namearray."[\"".$value."\"] = \$data_out[\"".$value."\"];\n\t\t}\n";
		}
		return $data;
	}

	function def_method_arrays ($list_methods_func) {
		$list_arrays = Array();
		
		foreach ($list_methods_func as $key => $value) {
			if (is_numeric($key)) {
				if ((isset($value["NAME"])) && (isset($value["ARRAY"])) && (isset($value["FIELDS"]))) {
					$new_fields = explode(",", $value["FIELDS"]);
					if (isset($list_arrays[$value["ARRAY"]])) {
						$list_arrays[$value["ARRAY"]] = array_unique(array_merge($list_arrays[$value["ARRAY"]], $new_fields));
					} else {
						$list_arrays[$value["ARRAY"]] = $new_fields;
					}
				}
			}
		}
		return $list_arrays;
	}
	
	function method_input ($fields, &$p_input) {
//		print_r($fields);
//		print_r($p_input);
	}
}
?>
