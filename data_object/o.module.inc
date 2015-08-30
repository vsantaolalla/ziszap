<?PHP

//
// Ziszap Portal System Objects.
//
// Sistema de generacion de codigo automatica de objetos genericos contra la database.
// Crear el codigo de llamadas gen�ricas contra el objeto hacia la base de datos, de forma que se llama 
// a las funciones objeto gen�rico de cada base de datos de data_object.
//

require_once("data_object/ag.object.inc");

// Inicializacion variables de comprovación de los comandos SQL.
$SQL_EXEC = Array();

class object_module extends generical_object {
//	var $classname = "object_module";

	function object_module () {
		parent::generical_object ();
	}
	
	function create_init ($object) {
		$path = get_directory()."/zz-object/".$object."/init.php";
		$object_class = $object;
		global $clase;
		global $clase_init;
		
		if (! file_exists($path)) {
			// Recreamos la variable de $cmd para generar l'objecte.
			global $db_host;
			global $db_user;
			global $db_passwd;
			global $database_name;

			require_once "zz-object/".$object."/class.php";
			$cmd = "global \$clase_init;\n\$clase_init=new z_".$object."();\n";
			eval($cmd);
			
			$data = "<?php \n";
			$data .= "require_once \"zz-object/".$object."/class.php\";\n\n";

			$data .= "class ".$object." extends z_".$object." {\n";
			$data .= "\tvar \$classname = \"".$object."\";\n\n";
			$list_functions = Array();
			foreach ($clase->list_methods() as $key => $value) {
				if (! method_exists($clase_init, $value["FUNCTION"])) {
					if (! isset($list_functions[$value["FUNCTION"]])) {
						$data .= "\t/**\n\t *\n";
						$data .= "\t * Funcion: ".$value["FUNCTION"];
						if (isset($value["TEXT"])) {
							$data .= " ... ".$value["TEXT"];
						}
						$data .= "\n";
						$list_input_parametres = Array();
						$list_output_parametres = Array();
						for ($index = 0; isset($value[$index]); $index++) {
							if ($value[$index]["NAME"] == "INPUT") {
								$parametres = explode(",", $value[$index]["FIELD"]);
								foreach ($parametres as $keyparam => $valueparam) {
									if (! isset($list_input_parametres[$valueparam])) {
										$data .= "\t * @input \t".$clase_init->get_fieldType($valueparam)."\t".$valueparam."\t".$clase_init->get_fieldText($valueparam)."\n";
										$list_input_parametres[$valueparam] = $clase_init->get_fieldType($valueparam);
									} 
								}
							}
							if (($value[$index]["NAME"] == "STORE_INPUT") && ($value[$index]["ACTION"] == "read")) {
								$parametres = explode(",", $value[$index]["FIELD"]);
								foreach ($parametres as $keyparam => $valueparam) {
									if (! isset($list_input_parametres[$valueparam])) {
										$data .= "\t * @input \t".$clase_init->get_fieldType($valueparam)."\t".$valueparam."\t".$clase_init->get_fieldText($valueparam)."\n";
										$list_input_parametres[$valueparam] = $clase_init->get_fieldType($valueparam);
									}
								}
							}
							if (($value[$index]["NAME"] == "STORE_OUTPUT") && (($value[$index]["ACTION"] == "write") || ($value[$index]["ACTION"] == "create"))) {
								$parametres = explode(",", $value[$index]["FIELD"]);
								foreach ($parametres as $keyparam => $valueparam) {
									if (! isset($list_output_parametres[$valueparam])) {
										$data .= "\t * @output\t".$clase_init->get_fieldType($valueparam)."\t".$valueparam."\t".$clase_init->get_fieldText($valueparam)."\n";
										$list_output_parametres[$valueparam] = $clase_init->get_fieldType($valueparam);
									}
								}
							}
						}			
						$data .= "\t *\n\t*/\n";
						$data .= "\tfunction ".$value["FUNCTION"]." ( \$data_in, &\$data_out ) {\n";
						$data .= "\t\t\n";
						$data .= "\t\t// TODO: Funcion ".$value["FUNCTION"]."\n";
						$data .= "\t\treturn TRUE;\n";
						$data .= "\t}\n\n";
						$list_functions[$value["FUNCTION"]] = 1;
					}
//					echo $value["FUNCTION"]."\n";
				} 
			}
			$data .= "}\n?>\n";
			echo $data;
			file_put_contents($path, $data);
		}
	}

	function create_class ($object) {
		global $clase;
		global $GLOBALS;
		
		$path = get_directory()."/zz-object/".$object."/class.php";
		$object_class = "z_".$object;
		$data = "<?php \n";
		$data .= "require_once \"data_object/i.object.inc\";\n\n";
		
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
			if (isset($value["PKEY"])) {
				$data .= "'".$value["NAME"]."' => '".$value["PKEY"]."'";
			} else {
				$data .= "'".$value["NAME"]."' => ''";
			}
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
		
		$data .= "\tfunction ".$object." () {\n";
		$data .= "\t\t\$this->ext_form = new ValidateForm();\n\t\t\$this->ext_form->tables = \$this->tables;\n";
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
			if (isset($value["TEXT"])) {
				$data .= "\t// Funcion: ".$value["NAME"]." ... ".$value["TEXT"]."\n";
			}
			$data .= "\tfunction ".$value["NAME"]."( \$data_in, &\$data_out ) {\n";

			$list_global_vars = Array();
			$list_access_groups = Array();
			$list_deny_groups = Array();
			$list_access_users = Array();
			$list_deny_users = Array();
			$enable_acls_users = FALSE;
			$enable_acls_groups = FALSE;
			$list_global_vars = $this->def_method_arrays($value);
			for ($index = 0; isset($value[$index]); $index++) {
				$value_value = $value[$index];
//				print_r($value_value);
				if (isset($value_value["NAME"])) {
					if (($value_value["NAME"] == "ACCESS") || ($value_value["NAME"] == "NOACCESS")) {
						if (isset($value_value["USER"])) {
							$enable_acls_users = TRUE;
							$list_global_vars["_SESSION"] = "ARRAY";
							$list_value_fields = explode(",", $value_value["USER"]);
							foreach ($list_value_fields as $key_vfield => $value_vfield) {
								if ($value_value["NAME"] == "ACCESS") {
									$list_access_users[$value_vfield] = $value_value["NAME"];
								} else {
									$list_deny_users[$value_vfield] = $value_value["NAME"];
								}	
							}
						}
						if (isset($value_value["GROUP"])) {
							$enable_acls_groups = TRUE;
							$list_global_vars["_SESSION"] = "ARRAY";
							$list_value_fields = explode(",", $value_value["GROUP"]);
							foreach ($list_value_fields as $key_vfield => $value_vfield) {
								if ($value_value["NAME"] == "ACCESS") {
									$list_access_groups[$value_vfield] = $value_value["NAME"];
								} else {
									$list_deny_groups[$value_vfield] = $value_value["NAME"];
								}
							}
						}
					}
					if (($value_value["NAME"] == "SET_VAR") || ($value_value["NAME"] == "GET_VAR")) {
						// Si en el SET_VAR o GET_VAR entran precedidas de un array.
						if (isset($value_value["ARRAY"])) {
							$list_global_vars[$value_value["ARRAY"]] = "ARRAY";
						} else if (isset($value_value["FIELD"])) {
							$list_value_fields = explode(",", $value_value["FIELD"]);
							foreach ($list_value_fields as $key_vfield => $value_vfield) {
								$list_global_vars[$value_vfield] = "VARIABLE";
							}
						}
					}
				}
			}
			foreach ($list_global_vars as $key_lgv => $value_lgv) {
				$data .= "\t\tglobal \$".$key_lgv.";\n";
			}

			$data .= "\n";
			if ($enable_acls_groups || $enable_acls_users) {
				$data .= "\t\trequire_once \"data_object/o.users.inc\";\n";
			}
			if ($enable_acls_groups) {
				$data .= "\t\t// Enable ACLS groups.\n";
				$data .= $this->def_acls_groups(implode(",", array_keys($list_access_groups)), implode(",", array_keys($list_deny_groups)));
			}
			if ($enable_acls_users) {
				$data .= "\t\t// Enable ACLS users.\n";
				$data .= $this->def_acls_users(implode(",", array_keys($list_access_users)), implode(",", array_keys($list_deny_users))); 
			}
			$data .= "\t\t// AUTO: Funcion ".$value["NAME"]."\n";
			$data .= "\t\t\$this->flush_data();\n";
			$data .= "\t\t\$this->begin_transaction();\n";

			// Inicializamos el SQL_EXEC
			global $SQL_EXEC;
			$SQL_EXEC["field"] = "";
			$SQL_EXEC["table"] = "";
			$SQL_EXEC["array"] = "";
			
			// Aqui procesamos las entradas input
			for ($index = 0; isset($value[$index]); $index++) {
				$func_method = $value[$index];
				if ($func_method["NAME"] == "INPUT") {
					$data .= $this->def_method_input ($func_method);
					if (isset($func_method["FIELD"])) {
						$data .= "\t\tif (\$this->create_html_input(\$SQL[\"field\"], \$data_in)) {\n";
						$data .= "\t\t\t\$this->failed_transaction();\n";
						$data .= "\t\t\treturn FALSE;\n\t\t}\n";
					} else if ($func_method["ARRAY_FIELD"]) {
						$data .= "\t\tforeach (\$data_in[\"".$func_method["ARRAY_NAME"]."\"] as \$key => \$value) {\n";
						$data .= "\t\t\tif (\$this->create_html_input(\$SQL[\"field\"], \$value)) {\n";
						$data .= "\t\t\t\t\$this->failed_transaction();\n";
						$data .= "\t\t\t\treturn FALSE;\n\t\t\t}\n\t\t}\n";
					}
				} else if ($func_method["NAME"] == "STORE_INPUT") {
					$data .= $this->def_method_store_input ($func_method);
				} else if ($func_method["NAME"] == "GET_VAR") {
					if (isset($func_method["ARRAY"])) {
						$data .= $this->def_method_get_var($func_method["FIELD"], $func_method["ARRAY"]);
					} else {
						$data .= $this->def_method_get_var($func_method["FIELD"]);
					}
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
					$data .= $this->def_method_store_output ($func_method);
				} else if ($func_method["NAME"] == "SET_VAR") {
					if (isset($func_method["ARRAY"])) {
						$data .= $this->def_method_set_var($func_method["FIELD"], $func_method["ARRAY"]);
					} else {
						$data .= $this->def_method_set_var($func_method["FIELD"]);
					}
				} else if ($func_method["NAME"] == "OUTPUT") {
					$data .= $this->def_method_output ($func_method["FIELD"]);
				}
//				else print_r($value[$index]);
			}
			$data .= "\t\t\$this->end_transaction();\n";
			$data .= "\t\treturn TRUE;\n";
			$data .= "\t}\n\n";
		}
		
		foreach ($clase->list_transactions() as $key => $value) {
			if (isset($value["TEXT"])) {
				$data .= "\t// Transaction: ".$value["NAME"]." ... ".$value["TEXT"]."\n";
			}
			$data .= "\tfunction ".$value["NAME"]."( \$data_in, &\$data_out ) {\n";
			$data .= "\t\tglobal \$_SESSION;\n\n";
			$data .= "\t\tif (session_id() == \"\") session_start();\n";
			$data .= "\t\tif (! isset(\$_SESSION[\"transaction.back\"]) ) \$_SESSION[\"transaction.back\"] = \"::start::\";\n\n";
			for ($index = 0; isset($value[$index]); $index++) {
				$value_t = $value[$index];
				$data .= "\t\tif (\$_SESSION[\"transaction.back\"] == \"".$value_t["BACK"]."\") {\n";
				$data .= "\t\t\tif (! \$this->".$value_t["NAME"]."( \$data_in, \$data_out ) ) {\n";
				$data .= "\t\t\t\t\$this->failed_transaction();\n";
				$data .= "\t\t\t\treturn FALSE;\n";
				$data .= "\t\t\t} else {\n";
				$data .= "\t\t\t\t\$_SESSION[\"transaction.now\"] = \"".$value_t["NAME"]."\";\n";
				$data .= "\t\t\t}\n\t\t}\n";
			}

			$data .= "\n\t\t\$_SESSION[\"transaction.back\"] = \$_SESSION[\"transaction.now\"];\n";
			$data .= "\t\treturn TRUE;\n";
			$data .= "\t}\n\n";
		}
				
		foreach ($clase->list_interfaces() as $key => $value) {
			$this->create_interface ($object, $value);
		}

		foreach ($clase->list_controllers() as $key => $value) {
			$this->create_controller ($object, $value);
		}
		$data .= "}\n?>\n";
		echo $data;
		file_put_contents($path, $data);
	}

	function create_test ($object) {
		global $GLOBALS;
		
		$path = get_directory()."/zz-object/".$object."/test.php";
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
		
		foreach ($clase->list_transactions() as $key => $value) {
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
		
		foreach ($clase->list_transactions() as $key => $value) {
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
		
		if (isset($fields["FIELD"])) {
			$data = "\t\t/** Verify INPUT ".$fields["FIELD"]."  */\n";
			$data .= "\t\t\$SQL[\"field\"] = Array (";
			$fields_array = explode(",", $fields["FIELD"]);
		} elseif (isset($fields["ARRAY_FIELD"])) {
			$data = "\t\t/** Verify ARRAY_INPUT ".$fields["ARRAY_FIELD"]."  */\n";
			$data .= "\t\t\$SQL[\"field\"] = Array (";
			$fields_array = explode(",", $fields["ARRAY_FIELD"]);
		}
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
	
	function def_method_store_input ($SQL) {
		global $SQL_EXEC;
		
		$data = "\n";
		if (isset($SQL["FIELD"])) {
			if ($SQL_EXEC["field"] != $SQL["FIELD"]) {
				$fields_array = explode(",", $SQL["FIELD"]);
				$data .= "\t\t\$SQL[\"field\"] = Array ('".implode("','", $fields_array )."');\n";
				$SQL_EXEC["field"] = $SQL["FIELD"];			
			}
		} elseif ($SQL["ARRAY_FIELD"]) {
			if ($SQL_EXEC["field"] != $SQL["ARRAY_FIELD"]) {
				$fields_array = explode(",", $SQL["ARRAY_FIELD"]);
				$data .= "\t\t\$SQL[\"field\"] = Array ('".implode("','", $fields_array )."');\n";
				$SQL_EXEC["field"] = $SQL["ARRAY_FIELD"];
				if ($SQL_EXEC["array"] != $SQL["ARRAY_NAME"]) {
					$data .= "\t\t\$SQL[\"array\"] = \"".$SQL["ARRAY_NAME"]."\";\n";
					$SQL_EXEC["array"] = $SQL["ARRAY_NAME"];		
				}
			}
		}

		if ($SQL_EXEC["table"] != $SQL["TABLE"]) {
			$fields_TABLE = explode(",", $SQL["TABLE"]);
			$data .= "\t\t\$SQL[\"table\"] = Array ('".implode("','", $fields_TABLE )."');\n";
			$SQL_EXEC["table"] = $SQL["TABLE"];
		}
		
		// Aqui definim diferents comandes per diferents accions.
		$action = $SQL["ACTION"];
		if ($action == "scan") {
			$data .= "\t\tif (! \$this->sql_scan (\$SQL, \$data_in) ) {\n";
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";
		} else if ($action == "noscan") {
			$data .= "\t\tif (\$this->sql_scan (\$SQL, \$data_in) ) {\n";
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";
		} else if ($action == "read" && isset($SQL["FIELD"])) {
			$data .= "\t\tif (! \$this->sql_read (\$SQL, \$data_in) ) {\n";
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";
		} else if ($action == "read" && isset($SQL["ARRAY_FIELD"])) {
			$data .= "\t\tif (! \$this->sql_array_read (\$SQL, \$data_in) ) {\n";
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";
		} else if ($action == "search") {
			if (isset($SQL["NEEDLE"])) {
				$data .= "\t\t\$SQL[\"needle\"]    = \$data_in[\"".$fields["NEEDLE"]."\"];\n";
			}
			if (isset($SQL["HAYSTACK"])) {
				$data .= "\t\t\$SQL[\"haystack\"]  = Array (";
				$fields_array = explode(",", $SQL["HAYSTACK"]);
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
			}
			$data .= "\t\tif (! \$this->sql_search (\$SQL, \$data_in) ) {\n";
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";
		}
		return $data;
	}
	
	function def_method_store_output ($SQL) {
		global $SQL_EXEC;
		
		$data = "\n";
		if (isset($SQL["FIELD"])) {
			if ($SQL_EXEC["field"] != $SQL["FIELD"]) {
				$fields_array = explode(",", $SQL["FIELD"]);
				$data .= "\t\t\$SQL[\"field\"] = Array ('".implode("','", $fields_array )."');\n";
				$SQL_EXEC["field"] = $SQL["FIELD"];			
			}
		} elseif ($SQL["ARRAY_FIELD"]) {
			if ($SQL_EXEC["field"] != $SQL["ARRAY_FIELD"]) {
				$fields_array = explode(",", $SQL["ARRAY_FIELD"]);
				$data .= "\t\t\$SQL[\"field\"] = Array ('".implode("','", $fields_array )."');\n";
				$SQL_EXEC["field"] = $SQL["ARRAY_FIELD"];
				if ($SQL_EXEC["array"] != $SQL["ARRAY_NAME"]) {
					$data .= "\t\t\$SQL[\"array\"] = \"".$SQL["ARRAY_NAME"]."\";\n";
					$SQL_EXEC["array"] = $SQL["ARRAY_NAME"];		
				}
			}
		}

		if ($SQL_EXEC["table"] != $SQL["TABLE"]) {
			$fields_TABLE = explode(",", $SQL["TABLE"]);
			$data .= "\t\t\$SQL[\"table\"] = Array ('".implode("','", $fields_TABLE )."');\n";
			$SQL_EXEC["table"] = $SQL["TABLE"];
		}

		// Aqui definim diferents comandes per diferents accions.
		$action = $SQL["ACTION"];
		if ($action == "create") {
			$data .= "\t\tif (! \$this->sql_create (\$SQL, \$data_out)) {\n";
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";
		} else if ($action == "write" && isset($SQL["FIELD"])) {
			$data .= "\t\tif (! \$this->sql_write (\$SQL, \$data_out)) {\n";
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";
		} else if ($action == "write" && isset($SQL["ARRAY_FIELD"])) {
			$data .= "\t\tif (! \$this->sql_array_write (\$SQL, \$data_out)) {\n";
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";
		} else if ($action == "delete") {
			$data .= "\t\tif (! \$this->sql_delete (\$SQL, \$data_out)) {\n";
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";
		}
		return $data;
	}

	function def_method_output ($fields) {
		
	}

	function def_method_get_var ($fields, $namearray = "") {
		$list_fields = explode(",", $fields);
		$data = "";
		foreach ($list_fields as $key => $value) {
			if ($namearray != "") {
				$data .= "\t\tif (! isset(\$".$namearray."[\"".$value."\"])) {\n";
			} else {
				$data .= "\t\tif (! isset(\$".$value.")) {\n";
			}
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t} else {\n";
			if ($namearray != "") {
				$data .= "\t\t\t\$data_in[\"".$value."\"] = \$".$namearray."[\"".$value."\"];\n\t\t}\n";
			} else {
				$data .= "\t\t\t\$data_in[\"".$value."\"] = \$".$value.";\n\t\t}\n";
			}
		}
		return $data;
	}
	
	function def_method_set_var ($fields, $namearray = "") {
		$list_fields = explode(",", $fields);
		$data = "";
		foreach ($list_fields as $key => $value) {
			$data .= "\t\tif (! isset(\$data_out[\"".$value."\"])) {\n";
			$data .= "\t\t\t\$this->failed_transaction();\n";
			$data .= "\t\t\treturn FALSE;\n\t\t} else {\n";
			if ($namearray != "") {
				$data .= "\t\t\t\$".$namearray."[\"".$value."\"] = \$data_out[\"".$value."\"];\n\t\t}\n";
			} else {
				$data .= "\t\t\t\$".$value." = \$data_out[\"".$value."\"];\n\t\t}\n";
			}
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
	
	function def_acls_users ($fields_allow, $fields_deny) {
		$data = "";
		if ($fields_allow != "") {
			$data .= "\t\t\$users_allowed = Array (";
			$list_allow = explode(",", $fields_allow);
			$first_time = TRUE;
			foreach ($list_allow as $key => $value) {
				if ($first_time) {
					$first_time = FALSE;
				} else {
					$data .= ",";
				}
				$data .= "'".$value."'";
			}
			$data .= ");\n";
			$data .= "\t\tif (! is_user_allow(\$users_allowed)) {\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";
		}
		if ($fields_deny != "") {
			$data .= "\t\t\$users_deny = Array ('".$fields_deny."');\n";
			$data .= "\t\tif (is_user_allow(\$users_deny)) {\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";
		}
		return $data;
	}

	function def_acls_groups ($fields_allow, $fields_deny) {
		$data = "";
		if ($fields_allow != "") {
			$data .= "\t\t\$groups_allowed = Array (";
			$list_allow = explode(",", $fields_allow);
			$first_time = TRUE;
			foreach ($list_allow as $key => $value) {
				if ($first_time) {
					$first_time = FALSE;
				} else {
					$data .= ",";
				}
				$data .= "'".$value."'";
			}
			$data .= ");\n";
			$data .= "\t\tif (! is_user_belong_group(\$groups_allowed)) {\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";
		}
		if ($fields_deny != "") {
			$data .= "\t\t\$groups_deny = Array ('".$fields_deny."');\n";
			$data .= "\t\tif (  is_user_belong_group(\$groups_deny)) {\n";
			$data .= "\t\t\treturn FALSE;\n\t\t}\n";
		}
		return $data;
	}

	function method_input ($fields, &$p_input) {
//		print_r($fields);
//		print_r($p_input);
	}

	protected function create_interface ($object, $value)
	{
		global $GLOBALS;
		$data = "<?\n\trequire_once('zz-admin/config.php');\n\n\t\$cmd_in = Array();\n\t\$cmd_out = Array();\n\n";
		$data .= "\tif (isset(\$_POST) && count(\$_POST) > 0) {\n\t\t\$cmd_in = \$_POST;\n\t} else if (isset(\$_GET) && count(\$_GET) > 0) {\n\t\t\$cmd_in = \$_GET;\n\t}\n\n";
		$data .= "\tinclude \"zz-object/".$object."/init.php\";\n\t\$T = new ".$object."();\n";
		$data .= "\t\$result = \$T->".$value["METHOD"]."(\$cmd_in, \$cmd_out);\n";
		$data .= "\tif (\$result) {\n\t\tif (! \$cmd_out)\n\t\t\theader('HTTP/1.1 204 No Content');\n\t\techo json_encode(\$cmd_out);\n";
		$data .= "\t} else {\n\t\theader('HTTP/1.1 405 Method Not Allowed');\n\t\techo \"error\";\n\t}\n";
		$path = get_directory()."/zz-interface/".$object."_".$value["NAME"].".php";
		$data .= "?>\n";
		if (! file_exists($path)) {
			echo $data;
			file_put_contents($path, $data);
		}
	}
	
	protected function create_controller ($object, $value)
	{
		global $GLOBALS;
		global $app_name;
		$path = get_directory()."/js/controllers/ctrl_".$value["NAME"].".js";
		if (! file_exists($path)) {
			$data = "";
	//		$data .= "angular.module('".$app_name."').controller('".$value["NAME"]."', function(\$scope, \$routeParams, \$http) {\n";
			$data .= "angular.module('".$app_name."').controller('".$value["NAME"]."', function(\$scope, \$http) {\n";
			$data .= "\t\$scope.loading = true;\n";
			$data .= "\t\$http.get('/zz-content/exec_json.method.php',{ params: {\n";
			if (isset($value["FIELD"])) {
				$list_fields = explode(",", $value["FIELD"]);
				foreach ($list_fields as $key => $field) {
					$data .= "\t\t".trim($field).": ".trim($field).",\n";
				}
			}
			$data .= "\t\tobject: '".$value["OBJECT"]."',\n";
			$data .= "\t\tmethod: '".$value["METHOD"]."'\n";
			$data .= "\t}}).success(function (data, status, headers, config) {\n";
			$data .= "\t\t\$scope.".$value["COLLECTOR"]." = data;\n";
			$data .= "\t}).error(function (data, status, headers, config) {\n";
			$data .= "\t\tconsole.error (data, status, headers, config);\n";
			$data .= "\t}).finally(function () {\n";
			$data .= "\t\t\$scope.loading = false;\n";
			$data .= "\t});\n";
			$data .= "});\n";
			echo $data;
			file_put_contents($path, $data);
		}
	}
}
?>
