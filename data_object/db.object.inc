<?PHP

//
// Nynpha Multimedia Portal System Objects.
//
// Sistema de objetos genericos de database.
// Llamadas gen�ricas de base de datos, de forma que se llama al objeto gen�rico
// de cada base de datos de data_object
//
// Propiedad de Mankind Capital, S.L -- Todos los derechos reservados
// Nynpha Multimedia y sus respectivos logos son marcas registradas de
// Mankind Capital SL
//

class generical_object_database {
//	var $classname = "generical_object_database";
	var $fields;
	var $form_object;
	var $fields_error;

	function generical_object_database () {
		global $PORTAL;
		require_once("plugins/form/init.inc");

		$object = $this->classname;
		$this->form_object = new plugin_form(find_object($object)."data.xml");
		$this->form_object->process_form();
		$this->form_object->serialize("output","",$this->form_array);
	}

	function object_field_names($d) {
		if ($this->fields == "") $this->object_struct();
		for ($i_vars = 0; $i_vars < $this->fields["count"]; $i_vars++) {
			if (isset($d[$this->fields[$i_vars]["form"]]) && ($d[$this->fields[$i_vars]["form"]] != ""))
				$d_field .= $this->fields[$i_vars]["database"].",";
		}
		$d_field = substr($d_field, 0, strlen($d_field) - 1);
		return "(".$d_field.")";
	}

	function object_field_values($d) {
		if ($this->fields == "") $this->object_struct();
		for ($i_vars = 0; $i_vars < $this->fields["count"]; $i_vars++) {
			if (isset($d[$this->fields[$i_vars]["form"]]) && ($d[$this->fields[$i_vars]["form"]] != ""))
				if ((strtoupper($this->fields[$i_vars]["typedb"]) == "VARCHAR") || (strtoupper($this->fields[$i_vars]["typedb"]) == "TEXT") || (strtoupper($this->fields[$i_vars]["typedb"]) == "DATETIME") || (strtoupper($this->fields[$i_vars]["typedb"]) == "DATE"))
					$d_field .= "'".$d[$this->fields[$i_vars]["form"]]."',";
				else
					$d_field .= $d[$this->fields[$i_vars]["form"]].",";
		}
		$d_field = substr($d_field, 0, strlen($d_field) - 1);
		return "(".$d_field.")";
	}

	function object_field_assign($d) {
		if ($this->fields == "") $this->object_struct();
		for ($i_vars = 0; $i_vars < $this->fields["count"]; $i_vars++) {
			if (isset($d[$this->fields[$i_vars]["form"]]))
				if ((strtoupper($this->fields[$i_vars]["typedb"]) == "VARCHAR") || (strtoupper($this->fields[$i_vars]["typedb"]) == "TEXT") || (strtoupper($this->fields[$i_vars]["typedb"]) == "DATETIME") || (strtoupper($this->fields[$i_vars]["typedb"]) == "DATE"))
					$d_field .= " ".$this->fields[$i_vars]["database"]." = '".$d[$this->fields[$i_vars]["form"]]."',";
				else
					$d_field .= " ".$this->fields[$i_vars]["database"]." = ".$d[$this->fields[$i_vars]["form"]].",";
		}
		$d_field = substr($d_field, 0, strlen($d_field) - 1);
		return $d_field;
	}

	function object_field_where($d) {
		if ($this->fields == "") $this->object_struct();
		for ($i_vars = 0; $i_vars < $this->fields["count"]; $i_vars++) {
			if (isset($d[$this->fields[$i_vars]["form"]]))
				if ((strtoupper($this->fields[$i_vars]["typedb"]) == "VARCHAR") || (strtoupper($this->fields[$i_vars]["typedb"]) == "TEXT") || (strtoupper($this->fields[$i_vars]["typedb"]) == "DATETIME") || (strtoupper($this->fields[$i_vars]["typedb"]) == "DATE"))
					$d_field .= $this->fields[$i_vars]["database"]." = '".$d[$this->fields[$i_vars]["form"]]."' AND ";
				else
					$d_field .= $this->fields[$i_vars]["database"]." = ".$d[$this->fields[$i_vars]["form"]]." AND ";
		}
		$d_field = substr($d_field, 0, strlen($d_field) - 5);
		return $d_field;
	}

	function object_field_where_pkey($d) {
		if ($this->fields == "") $this->object_struct();
		for ($i_vars = 0; $i_vars < $this->fields["count"]; $i_vars++) {
			if (strtoupper($this->fields[$i_vars]["primary_key"]) == "YES")
				if ((strtoupper($this->fields[$i_vars]["typedb"]) == "VARCHAR") || (strtoupper($this->fields[$i_vars]["typedb"]) == "TEXT") || (strtoupper($this->fields[$i_vars]["typedb"]) == "DATETIME") || (strtoupper($this->fields[$i_vars]["typedb"]) == "DATE"))
					$d_field .= $this->fields[$i_vars]["database"]." = '".$d[$this->fields[$i_vars]["form"]]."' AND ";
				else
					$d_field .= $this->fields[$i_vars]["database"]." = ".$d[$this->fields[$i_vars]["form"]]." AND ";
		}
		$d_field = substr($d_field, 0, strlen($d_field) - 5);
		return $d_field;
	}

	function object_field_where_ekey_nx1($d,$database_export=true) {
		$object = $this->classname;
		require_once($PORTAL["file"]["root"]."data_object/od.xml.file.inc");
		$db_serialize = new xml_file_object(find_object($object)."data.xml");
		$db_serialize->serialize("xml_array",$params,$fields);

		$contents = $fields["DB_TABLE"][$object]["DB_CONTENT"][0]["CONTENT"];
		for ($i_vars = 0; $i_vars < $contents["count"]; $i_vars++) {
			if ($contents[$i_vars]["attr"]["RELATION"] == "nx1") {
				$external_k[$contents[$i_vars]["attr"]["KEY"]] = $contents[$i_vars]["attr"]["KEY"];
			}
		}

		if ($this->fields == "") $this->object_struct();
		for ($i_vars = 0; $i_vars < $this->fields["count"]; $i_vars++) {
			if ((isset($external_k[$this->fields[$i_vars]["database"]])) && (isset($d[$this->fields[$i_vars]["database"]])))
				if ((strtoupper($this->fields[$i_vars]["typedb"]) == "VARCHAR") || (strtoupper($this->fields[$i_vars]["typedb"]) == "TEXT") || (strtoupper($this->fields[$i_vars]["typedb"]) == "DATETIME") || (strtoupper($this->fields[$i_vars]["typedb"]) == "DATE"))
					if ($database_export)
						$d_field .= $this->fields[$i_vars]["database"]." = '".$d[$this->fields[$i_vars]["form"]]."' AND ";
					else
						$d_field .= $this->fields[$i_vars]["database"]."=".$d[$this->fields[$i_vars]["form"]]."&";
				else
					if ($database_export)
						$d_field .= $this->fields[$i_vars]["database"]." = ".$d[$this->fields[$i_vars]["form"]]." AND ";
					else
						$d_field .= $this->fields[$i_vars]["database"]."=".$d[$this->fields[$i_vars]["form"]]."&";
		}
		if ($database_export) $d_field = substr($d_field, 0, strlen($d_field) - 5);
		else $d_field = substr($d_field, 0, strlen($d_field) - 1);

		return $d_field;
	}

	function object_field_where_ekey_1xn($d,$database_export=true) {
		$object = $this->classname;
		require_once($PORTAL["file"]["root"]."data_object/od.xml.file.inc");
		$db_serialize = new xml_file_object(find_object($object)."data.xml");
		$db_serialize->serialize("xml_array",$params,$fields);

		$contents = $fields["DB_TABLE"][$object]["DB_CONTENT"][0]["CONTENT"];
		for ($i_vars = 0; $i_vars < $contents["count"]; $i_vars++) {
			if ($contents[$i_vars]["attr"]["RELATION"] == "1xn") {
				$external_k[$contents[$i_vars]["attr"]["KEY"]] = $contents[$i_vars]["attr"]["KEY"];
			}
		}

		if ($this->fields == "") $this->object_struct();
		for ($i_vars = 0; $i_vars < $this->fields["count"]; $i_vars++) {
			if ((isset($external_k[$this->fields[$i_vars]["database"]])) && (isset($d[$this->fields[$i_vars]["database"]])))
				if ((strtoupper($this->fields[$i_vars]["typedb"]) == "VARCHAR") || (strtoupper($this->fields[$i_vars]["typedb"]) == "TEXT") || (strtoupper($this->fields[$i_vars]["typedb"]) == "DATETIME") || (strtoupper($this->fields[$i_vars]["typedb"]) == "DATE"))
					if ($database_export)
						$d_field .= $this->fields[$i_vars]["database"]." = '".$d[$this->fields[$i_vars]["form"]]."' AND ";
					else
						$d_field .= $this->fields[$i_vars]["database"]."=".$d[$this->fields[$i_vars]["form"]]."&";
				else
					if ($database_export)
						$d_field .= $this->fields[$i_vars]["database"]." = ".$d[$this->fields[$i_vars]["form"]]." AND ";
					else
						$d_field .= $this->fields[$i_vars]["database"]."=".$d[$this->fields[$i_vars]["form"]]."&";
		}
		if ($database_export) $d_field = substr($d_field, 0, strlen($d_field) - 5);
		else $d_field = substr($d_field, 0, strlen($d_field) - 1);

		return $d_field;
	}

	function is_full($d) {
		if ($this->fields == "") $this->object_struct();
		for ($i_vars = 0; $i_vars < $this->fields["count"]; $i_vars++) {
			if (isset($d[$this->fields[$i_vars]["form"]]))
				$d_field[$this->fields[$i_vars]["form"]] = $this->fields[$i_vars]["database"];
		}
		$d_field["count"] = count($d_field);
		return $d_field;
	}

	function is_must($d) {
		if ($this->fields == "") $this->object_struct();
		for ($i_vars = 0; $i_vars < $this->fields["count"]; $i_vars++) {
			if ($this->fields[$i_vars]["require"] == "yes")
				if ($d[$this->fields[$i_vars]["form"]] == ""){
					$count_error = count($this->fields_error);
					$this->fields_error[$count_error]["field"] = $this->fields[$i_vars]["form"];
					$this->fields_error[$count_error]["text"] = "Falta rellenar el campo ".$this->fields[$i_vars]["text"];
				}
		}
		if (count($this->fields_error) > 0) return false;
		return true;
	}

	function is_form_once() {
		global $vQUERY;
		for ($i_vars = 0; $i_vars < count($this->fields_error); $i_vars++) {
			if (isset($vQUERY[$this->fields_error[$i_vars]["field"]])) return false;
		}
		return true;
	}

	function list_autoincrement($d) {
		$object = $this->classname;
		require_once($PORTAL["file"]["root"]."data_object/od.xml.file.inc");
		$db_serialize = new xml_file_object(find_object($object)."data.xml");
		$db_serialize->serialize("xml_array",$params,$fields);

		if ($this->fields == "") $this->object_struct();
		for ($i_vars = 0; $i_vars < $this->fields["count"]; $i_vars++) {
			if (isset($fields["DB_TABLE"][$this->table_object]["DB_FIELDS"][0]["FIELD"][$this->fields[$i_vars]["database"]]["AUTO_INCREMENT"])) {
				$count_autoincrement = count($field_autoincrement);
				$field_autoincrement[$count_autoincrement] = $this->fields[$i_vars];
			}
		}
		$field_autoincrement["count"] = count($field_autoincrement);
		return $field_autoincrement;
	}

	function is_must_no_autoincrement($d) {
		$autoincrements = $this->list_autoincrement($d);
		for ($i_vars = 0; $i_vars < $autoincrements["count"]; $i_vars++) {
			$d[$autoincrements[$i_vars]["form"]] = "zz";
		}
		return $this->is_must($d);
	}

	function is_validate_ok($d) {
		if ($this->fields == "") $this->object_struct();
		for ($i_vars = 0; $i_vars < $this->fields["count"]; $i_vars++) {
			if ($this->fields[$i_vars]["validation_key"] != "") {
				if (preg_match($this->fields[$i_vars]["validation_key"],$d[$this->fields[$i_vars]["form"]]) == 0) {
					$count_error = count($this->fields_error);
					$this->fields_error[$count_error]["field"] = $this->fields[$i_vars]["form"];
					$this->fields_error[$count_error]["text"] = "Sintaxis incorrecta";
				}
			}
		}
		if (count($this->fields_error) > 0) {
			return false;
		}
		return true;
	}

	function end_method() {
		global $class_method;
		$class_method->free_method();
	}

	function object_struct() {
		global $PORTAL;

		$object = $this->classname;
		require_once($PORTAL["file"]["root"]."data_object/od.xml.file.inc");
		$db_serialize = new xml_file_object(find_object($object)."data.xml");
		$db_serialize->serialize("xml_array",$params,$fields);

		// Aqui comprobamos la base de datos por defecto y cogemos las referencias a la BBDD.
		if ((! isset($this->table_object)) || ($this->table_object == "")) {
			$keys_db = array_keys($fields["DB_TABLE"]);
			$this->table_object = $keys_db[0];
		}

		$array_fieldsdb = $fields["DB_TABLE"][$this->table_object]["DB_FIELDS"][0]["FIELD"];
		unset($array_fieldsdb["count"]);
		$values_fieldsdb = array_keys($array_fieldsdb);
//		print_r($array_fieldsdb);

		// Aqui comprobamos el formulario por defecto y cogemos sus referencias.
		if ((! isset($name_form)) || ($name_form == "")) {
			$keys_form = array_keys($fields["FORM_FIELDS"]);
			$name_form = $keys_form[0];
		}
		$array_section = $fields["FORM_FIELDS"][$name_form]["SECTION"];

		// Procesamos cada secci�n:
		$values_fieldsform = Array();
		for ($i_vars = 0; $i_vars < $array_section["count"]; $i_vars++) {
			unset($array_section[$i_vars]["FIELD"]["count"]);
			$values_fieldsform = array_merge(array_keys($array_section[$i_vars]["FIELD"]), $values_fieldsform);
			for ($j_vars = 0; $j_vars < count($values_fieldsform); $j_vars++) {
				$arr_name = explode("[.]", $values_fieldsform[$j_vars]);
				if (isset($arr_name[1])) {
					// TODO: Hacer si hace referencia a otra tabla
					$param["name"] = $arr_name[1];
					$param["text"] = $this->fields["DB_TABLE"][$arr_name[0]]["DB_FIELDS"][0]["FIELD"][$arr_name[1]]["TEXT"][0]["data"];
				} else $array_fieldsform[$arr_name[0]] = $array_section[$i_vars]["FIELD"][$arr_name[0]];
			}
		}

		for ($i_vars = 0; $i_vars < count($values_fieldsdb); $i_vars++) {
			$value = $values_fieldsdb[$i_vars];
			$zcnt = count($this->fields);
			$this->fields[$zcnt]["database"] = $value;
			if (isset($array_fieldsform[$value])) $this->fields[$zcnt]["require"] = $array_fieldsform[$value]["attr"]["REQUIRED"];
			else $this->fields[$zcnt]["require"] = "no";
			$this->fields[$zcnt]["form"] = $value;
			$this->fields[$zcnt]["type"] = $array_fieldsform[$value]["attr"]["TYPE"];
			if (isset($array_fieldsdb[$value]["EXTERNAL"])) $this->update_external($array_fieldsdb[$value]);
			$this->fields[$zcnt]["typedb"] = $array_fieldsdb[$value]["attr"]["TYPE"];
			if (isset($array_fieldsdb[$value]["PRIMARY_KEY"])) $this->fields[$zcnt]["primary_key"] = "yes";
			else $this->fields[$zcnt]["primary_key"] = "no";
			if (isset($array_fieldsdb[$value]["VALIDATION"][0]["data"])) $this->fields[$zcnt]["validation_key"] = $array_fieldsdb[$value]["VALIDATION"][0]["data"];
			else $this->fields[$zcnt]["validation_key"] = "";
			$this->fields[$zcnt]["attributes"] = trim($fieldsdb[$i_vars][3]);
		}
		$this->fields["count"] = count($this->fields);
//		print_r($this->fields);
	}

	function fix_value($name_var, $value_var) {
		$this->form_object->fix_value($this->form_array, $name_var, $value_var);
	}

	function is_create($d) {
		return $this->is_must_no_autoincrement($d) && $this->is_validate_ok($d);
	}

	function sql_create($d) {
		$autoincrement = $this->list_autoincrement($d);
		for ($i_vars = 0; $i_vars < count($autoincrement); $i_vars++)
			unset($d[$autoincrement[$i_vars]["database"]]);
		$sql_query  = "INSERT INTO ".$this->table_object." ".$this->object_field_names($d);
		$sql_query .= " VALUES ".$this->object_field_values($d);
		return $sql_query;
	}

	function create_object($d) {
		global $class_method;
		global $database_object;

		if ($this->is_create($d)) {
			$d_fields = $this->is_full($d);
			$sql_query = $this->sql_create($d);
			$params["serialize.query"] = $sql_query;
			$database_object->serialize("output", $params, $info);
			$class_method->free_method();
		} else {
			$this->form_object->txt_button("submit","A�adir >>");
			$this->form_object->serialize("values",$d,$this->form_array);
		}
	}

	function is_update($d) {
		return $this->is_must($d) && $this->is_validate_ok($d);
	}

	function sql_update($d) {
		$sql_query  = "UPDATE ".$this->table_object." SET ".$this->object_field_assign($d);
		$sql_query .= " WHERE ".$this->object_field_where_pkey($d);
		return $sql_query;
	}

	function update_object($d) {
		global $class_method;
		global $database_object;

		if ($this->is_update($d)) {
			$d_fields = $this->is_full($d);
			$sql_query = $this->sql_update($d);
			$params["serialize.query"] = $sql_query;
			$database_object->serialize("output", $params, $info);
			$class_method->free_method();
		} else {
			$this->form_object->txt_button("submit","Actualizar >>");
			$this->form_object->serialize("values",$d,$this->form_array);
		}
	}

	function sql_delete($d) {
		$sql_query  = "DELETE FROM ".$this->table_object;
		$sql_query .= " WHERE ".$this->object_field_where_pkey($d);
		return $sql_query;
	}

	function delete_object($d) {
		global $class_method;
		global $database_object;

		if ($this->fields == "") $this->object_struct();
		$sql_query = $this->sql_delete($d);
		$params["serialize.query"] = $sql_query;
		$database_object->serialize("output", $params, $info);
		$class_method->free_method();
	}

	function update_select_object($d) {
		global $database_object;

		if ($this->is_update($d)) {
			$this->update_object($d);
		} else {
			$d_fields = $this->is_full($d);
			$sql_query = $this->sql_select_unique($d);
			$params["serialize.query"] = $sql_query;
			$database_object->serialize("output", $params, $info);
			$dd = $info[0];
			$this->serialize("fields_db2form",$info[0],$dd);
			$this->form_object->txt_button("submit","Actualizar >>");
			$this->form_object->serialize("values",$dd,$this->form_array);
		}
	}

	function sql_select($d) {
		$sql_query  = "SELECT * FROM ".$this->table_object." WHERE ".$this->object_field_where($d);
		return $sql_query;
	}

	function select_object($d,&$info) {
		global $class_method;
		global $database_object;

		$d_fields = $this->is_full($d);
		$sql_query = $this->sql_select($d);
		$params["serialize.query"] = $sql_query;
		$database_object->serialize("output", $params, $info);
		$class_method->free_method();
	}

	function view_object($d) {
		global $class_method;
		global $database_object;
		global $vSESSION;

		$d_fields = $this->is_full($d);
		$sql_query = $this->sql_select($d);
		$params["serialize.query"] = $sql_query;
		$database_object->serialize("output", $params, $info);

		$dd = $info[0];
		$this->serialize("fields_db2form",$info[0],$dd);
		$this->form_object->serialize("values",$dd,$this->form_array);

//		$vSESSION["fieldsdb"] = $info;
	}

	function sql_select_unique($d) {
		$sql_query  = "SELECT * FROM ".$this->table_object." WHERE ".$this->object_field_where_pkey($d);
		return $sql_query;
	}

	function select_unique_object($d,&$info) {
		global $class_method;
		global $database_object;

		$d_fields = $this->is_full($d);
		$sql_query = $this->sql_unique_select($d);
		$params["serialize.query"] = $sql_query;
		$database_object->serialize("output", $params, $info);
		$class_method->free_method();
	}

	function sql_select_list($d) {
		if ($this->object_field_where_ekey_nx1($d) != "")
			$sql_query  = "SELECT * FROM ".$this->table_object." WHERE ".$this->object_field_where_ekey_nx1($d);
		else $sql_query  = "SELECT * FROM ".$this->table_object;
		return $sql_query;
	}

	function select_list_object($d) {
		global $class_method;
		global $database_object;
		global $vSESSION;

		$d_fields = $this->is_full($d);
		$sql_query = $this->sql_select_list($d);
		$params["serialize.query"] = $sql_query;
		$database_object->serialize("output", $params, $info);
		$vSESSION["fieldsdb"] = $info;
//		$class_method->free_method();
	}

	function sql_select_all($d) {
		$sql_query  = "SELECT * FROM ".$this->table_object;
		return $sql_query;
	}

	function select_all_object($d) {
		global $class_method;
		global $database_object;
		global $vSESSION;

		$d_fields = $this->is_full($d);
		$sql_query = $this->sql_select_all($d);
		$params["serialize.query"] = $sql_query;
		$database_object->serialize("output", $params, $info);
		$vSESSION["fieldsdb"] = $info;
//		$class_method->free_method();
	}

	function update_external(&$arr_external) {
		global $PORTAL;

		$object = $arr_external["EXTERNAL"][0]["attr"]["OBJECT"];
		if ($arr_external["EXTERNAL"][0]["attr"]["TABLE"]) $table = $arr_external["EXTERNAL"][0]["attr"]["TABLE"];
		else $table = $object;
		$field = $arr_external["EXTERNAL"][0]["attr"]["FIELD"];

		require_once($PORTAL["file"]["root"]."data_object/od.xml.file.inc");
		$db_serialize = new xml_file_object(find_object($object)."data.xml");
		$db_serialize->serialize("xml_array",$params,$fields_ext);
		if (! isset($fields_ext["DB_TABLE"][$table]["DB_FIELDS"][0]["FIELD"][$field])) $db_serialize->xml_die("Table[$table] o Field[$field] incorrectos en la definici�n de external del objecto[$object]");
		$array_fields_db = $fields_ext["DB_TABLE"][$table]["DB_FIELDS"][0]["FIELD"][$field];
		$arr_external["attr"]["TYPE"] = $array_fields_db["attr"]["TYPE"];
		$arr_external["attr"]["CARDINAL"] = $array_fields_db["attr"]["CARDINAL"];
	}

	function serialize_xml2database() {
		global $PORTAL;
		global $database_object;
		global $table_name;
		global $method_object;

		// http://seth/shuttle.php?module=project&method=zCalendar.export_calendar&table_name=zTask
		// http://virtual1/shuttle.php?module=workflow&method=wk_circular.import_xml (sobre el mateix objecte wk_circular).
		if (isset($table_name)) $object = $table_name;
		else $object = $method_object;

		require_once($PORTAL["file"]["root"]."data_object/od.xml.file.inc");
		$db_serialize = new xml_file_object(find_object($object)."data.xml");
		$db_serialize->serialize("xml_array",$params,$fields);
		// PRIMER PASO: Comprobamos que exista las tablas en la base de datos
		if (isset($fields["WORKFLOW"])) $workflow_object = true;
		else $workflow_object = false;
		$field_workflow_detected = false;

		$params["serialize.query"] = "show tables";
		$database_object->serialize("output", $params, $info);
		$is_table_exist = false;
		unset($fields["DB_TABLE"]["count"]);
		$list_tables = array_keys($fields["DB_TABLE"]);
		for ($i_vars = 0; $i_vars < count($list_tables); $i_vars++) {
			$is_tables[$i_vars] = 0;
			for ($j_vars=0; $j_vars < $info["count"]; $j_vars++) {
				if ($list_tables[$i_vars] == $info[$j_vars]["Tables_in_".$PORTAL["database"]["name"]])
					$is_tables[$i_vars] = 1;
			}
		}
//		print_r($list_tables);
		unset($info);

		// SEGUNDO PASO: En caso de existir la tabla comprobar los atributos, tipados y cardinalidades
		$list_primary_keys = Array();
		$list_unique_keys = Array();
		$list_index_keys = Array();
		$field_workflow_detected = false;
		for ($i_vars = 0; $i_vars < count($list_tables); $i_vars++) {
			if ($is_tables[$i_vars] == 0) {
				$params["serialize.query"] =  "CREATE TABLE ".$list_tables[$i_vars]."(workflow ENUM ('S','W','N') DEFAULT 'W')";
				$database_object->serialize("output", $params, $info);
				$is_tables[$i_vars] = 1;
			}
			if ($is_tables[$i_vars] == 1) {
				if (isset($fields["DB_TABLE"][$list_tables[$i_vars]]["WORKFLOW"])) $workflow_db = true;
				else $workflow_db = false;
				if (isset($fields["DB_TABLE"][$list_tables[$i_vars]]["FILES"])) $file_db = true;
				else $file_db = false;
				$params["serialize.query"] = "show fields from ".$list_tables[$i_vars];
				$database_object->serialize("output", $params, $info);
				$array_fields_db = $fields["DB_TABLE"][$list_tables[$i_vars]]["DB_FIELDS"][0]["FIELD"];
				// Aplicar per cada field de la base de dades.
				unset($array_fields_db["count"]);
				$list_fields_db = array_keys($array_fields_db);
				$params_autoincrement = "";
				// Aqui revisamos si existe el campo workflow en la base de datos
				for ($j_vars = 0; $j_vars < $info["count"]; $j_vars++)
					if ($info[$j_vars]["Field"] == "workflow") $field_workflow_detected = true;
				// Sols afegirem els que es trobin al XML i no a la base de dades, a la inversa no els esborrarem.
				for ($j_vars = 0; $j_vars < count($list_fields_db); $j_vars++) {
					$action_db_serialize = " CHANGE ".$list_fields_db[$j_vars];
					for ($k_vars = 0; $k_vars < $info["count"]; $k_vars++)
						if ($list_fields_db[$j_vars] == $info[$k_vars]["Field"]) break;
					if ($k_vars == $info["count"]) {
						// L'element XML no existeix a la base de dades, es crea.
						$action_db_serialize = " ADD";
//						continue;
					}
					// En aquest punt es comparen els camps que coincideixin.
					// Fijaremos la serializaci�n a FALSE que cuando encontremos que se ha de realizar un cambio la pondremos a true.
					$xml_db_serialize = false;
					// Comprovaremos que el campo no tenga el elemento external si es el caso a�adiremos las entrades. (type, cardinal)
					if (isset($array_fields_db[$list_fields_db[$j_vars]]["EXTERNAL"])) $this->update_external($array_fields_db[$list_fields_db[$j_vars]]);
					// Comparaci�n Type.
//					print_r($array_fields_db[$list_fields_db[$j_vars]]);
					$xml_type = $array_fields_db[$list_fields_db[$j_vars]]["attr"]["TYPE"];
					if (isset($array_fields_db[$list_fields_db[$j_vars]]["attr"]["CARDINAL"])) $xml_type .= "(".$array_fields_db[$list_fields_db[$j_vars]]["attr"]["CARDINAL"].")";
					if (isset($array_fields_db[$list_fields_db[$j_vars]]["UNSIGNED"])) $xml_type .= " unsigned";
					if ($info[$k_vars]["Type"] != $xml_type) $xml_db_serialize = true;
					// Comparaci�n Null.
					$xml_null = $db_null = "";
					if ($info[$k_vars]["Null"] != "") $db_null = " NOT NULL";
					if (isset($array_fields_db[$list_fields_db[$j_vars]]["NOT_NULL"])) $xml_null = " NOT NULL";
					if (isset($array_fields_db[$list_fields_db[$j_vars]]["PRIMARY_KEY"])){
						$xml_null = " NOT NULL";
						$list_primary_keys[$list_fields_db[$j_vars]] = 1;
					}
					if (isset($array_fields_db[$list_fields_db[$j_vars]]["UNIQUE_KEY"])){
						$xml_null = " NOT NULL";
						$list_unique_keys[$list_fields_db[$j_vars]] = 1;
					}
					if (isset($array_fields_db[$list_fields_db[$j_vars]]["INDEX_KEY"])){
						$list_index_keys[$list_fields_db[$j_vars]] = 1;
					}
					if ($db_null != $xml_null) $xml_db_serialize = true;
					// Comparaci�n Default.
					$xml_default_value = $db_default_value = "";
					if ($info[$k_vars]["Default"] != "NULL") $db_default_value = " DEFAULT \"".$info[$k_vars]["Default"]."\"";
					if (isset($array_fields_db[$list_fields_db[$j_vars]]["attr"]["DEFAULT"])) $xml_default_value = " DEFAULT \"".$array_fields_db[$list_fields_db[$j_vars]]["attr"]["DEFAULT"]."\"";
					if ($db_default_value != $xml_default_value) $xml_db_serialize = true;
					// Comparaci�n Extra.
					$xml_extra = $db_extra = "";
					if ($info[$k_vars]["Extra"] != "") $db_extra = " AUTO_INCREMENT";
					if (isset($array_fields_db[$list_fields_db[$j_vars]]["AUTO_INCREMENT"])) $xml_extra = " AUTO_INCREMENT";
					if (($db_extra != $xml_extra) || ($xml_extra == " AUTO_INCREMENT")){
						$params_autoincrement["serialize.query"] = "ALTER TABLE ".$list_tables[$i_vars]." CHANGE ".$list_fields_db[$j_vars]." ".$list_fields_db[$j_vars]." ".$xml_type.$xml_default_value.$xml_null.$xml_extra;
						$xml_db_serialize = true;
					}
					// Serialitzaci� de la BBDD a trav�s del XML.
					if ($xml_db_serialize == true) {
						$params["serialize.query"] = "ALTER TABLE ".$list_tables[$i_vars].$action_db_serialize." ".$list_fields_db[$j_vars]." ".$xml_type.$xml_default_value.$xml_null;
						$database_object->serialize("output", $params, $info_i);
					}
				}
				// Las claves primarias las asignamos una vez modificada la tabla
				if (count($list_primary_keys) > 0){
					$list_fields_primary_keys = array_keys($list_primary_keys);
					$params["serialize.query"] = "ALTER TABLE ".$list_tables[$i_vars]." DROP PRIMARY KEY, ADD PRIMARY KEY(".implode(",",$list_fields_primary_keys).")";
					$database_object->serialize("output", $params, $info_i);
//					echo $params["serialize.query"];
				}
				// En este punto actualizamos los indices y claves �nicas.
				$params["serialize.query"] = "show keys from ".$list_tables[$i_vars];
				$database_object->serialize("output", $params, $info_temp);
				for ($temp_vars = 0; $temp_vars < $info_temp["count"]; $temp_vars++) {
					if ($info_temp[$temp_vars]["Key_name"] != "PRIMARY") {
						$params["serialize.query"] = "ALTER TABLE ".$list_tables[$i_vars]." DROP INDEX ".$info_temp[$temp_vars]["Key_name"];
						$database_object->serialize("output", $params, $info_i);
					}
				}
				if (count($list_unique_keys) > 0){
					$list_fields_unique_keys = array_keys($list_unique_keys);
					for ($temp_vars = 0; $temp_vars < count($list_fields_unique_keys); $temp_vars++) {
						$params["serialize.query"] = "ALTER TABLE ".$list_tables[$i_vars]." ADD UNIQUE (".$list_fields_unique_keys[$temp_vars].")";
						$database_object->serialize("output", $params, $info_i);
					}
				}
				if (count($list_index_keys) > 0){
					$list_fields_index_keys = array_keys($list_index_keys);
					for ($temp_vars = 0; $temp_vars < count($list_fields_index_keys); $temp_vars++) {
						$params["serialize.query"] = "ALTER TABLE ".$list_tables[$i_vars]." ADD INDEX (".$list_fields_index_keys[$temp_vars].")";
						$database_object->serialize("output", $params, $info_i);
					}
				}
				// AUTOINCREMENT se debe hacer despu�s de las claves porque AUTO_INCREMENT necesita ser clave primaria.
				if ($params_autoincrement != "") $database_object->serialize("output", $params_autoincrement, $info_i);

				// Procesamos si la tabla ha de tener ficheros asociados en su directorio
				if ($file_db == true) {
					$params["serialize.query"] = "ALTER TABLE ".$list_tables[$i_vars]." ADD SYS_FILE VARCHAR(25)";
					$database_object->serialize("output", $params, $info_i);
				}
				// Finalmente procesamos el workflow de la tabla.
				if ($field_workflow_detected == true) {
					if (($workflow_object == false) && ($workflow_db == false)) {
						$params["serialize.query"] = "ALTER TABLE ".$list_tables[$i_vars]." DROP workflow";
						$database_object->serialize("output", $params, $info_i);
					}
				} else {
					if (($workflow_object == true) || ($workflow_db == true)) {
						$params["serialize.query"] = "ALTER TABLE ".$list_tables[$i_vars]." ADD workflow ENUM('Y','W','N')  DEFAULT 'W'";
						$database_object->serialize("output", $params, $info_i);
					}
				}
				unset($list_primary_keys);
				unset($list_unique_keys);
				unset($list_index_keys);
//				print_r($list_fields_db);
			}
		}
	}

	function serialize_database2xml() {
		global $PORTAL;
		global $database_object;
		global $table_name;

		if (isset($table_name)) $object = $table_name;
		else $object = $method_object;
		$field_workflow_detected = false;

		// PRIMER PASO: Comprobamos que exista las tablas en la base de datos
		// SEGUNDO PASO: En caso de existir la tabla comprobar los atributos, tipados y cardinalidades
		$list_primary_keys = Array();
		$list_unique_keys = Array();
		$list_index_keys = Array();

		header("Content-Type: application/octet-stream");
		// En este punto actualizamos los indices y claves �nicas.
		$params["serialize.query"] = "show keys from ".$object;
		$database_object->serialize("output", $params, $info_temp);
		for ($i_vars = 0; $i_vars < $info_temp["count"]; $i_vars++) {
			if ($info_temp[$i_vars]["Key_name"] == "PRIMARY") $list_primary_keys[$info_temp[$i_vars]["Column_name"]] = 1;
			else {
				if ($info_temp[$i_vars]["Non_unique"] == 0) $list_unique_keys[$info_temp[$i_vars]["Column_name"]] = 1;
				if ($info_temp[$i_vars]["Non_unique"] == 1) $list_index_keys[$info_temp[$i_vars]["Column_name"]] = 1;
			}
		}
		echo "<object name=\"".$object."\">\n\t<db_table name=\"".$object."\">\n\t\t<db_fields>\n";
		$params["serialize.query"] = "show fields from ".$object;
		$database_object->serialize("output", $params, $info);
		// Sols afegirem els que es trobin al XML i no a la base de dades, a la inversa no els esborrarem.
		for ($i_vars = 0; $i_vars < $info["count"]; $i_vars++) {
			if ($info[$i_vars]["Field"] == "workflow") {
				$field_workflow_detected = true;
				continue;
			}
			echo "\t\t\t<field name=\"".$info[$i_vars]["Field"]."\"";
			$parse_type = explode(" ", $info[$i_vars]["Type"]);
			if (ereg("([A-z]+)(\()([0-9]+)(\))", $parse_type[0], $regs))
				echo " type=\"".$regs[1]."\" cardinal=\"".$regs[3]."\"";
			else
				echo " type=\"".$parse_type[0]."\"";
			if ($info[$i_vars]["Default"] != "") echo " default=\"".$info[$i_vars]["Default"]."\"";
			echo ">\n"; // type=\"integer" cardinal="11">
			if ($info[$i_vars]["Null"] == "") echo "\t\t\t\t<not_null />\n";
			if ($info[$i_vars]["Extra"] == "auto_increment") echo "\t\t\t\t<auto_increment />\n";
			if ($list_primary_keys[$info[$i_vars]["Field"]] == 1) echo "\t\t\t\t<primary_key />\n";
			if ($list_unique_keys[$info[$i_vars]["Field"]] == 1) echo "\t\t\t\t<unique_key />\n";
			if ($list_index_keys[$info[$i_vars]["Field"]] == 1) echo "\t\t\t\t<index_key />\n";
			echo "\t\t\t</field>\n";
		}
		if ($field_workflow_detected == true) echo "\t\t\t<workflow />\n";
		echo "\t\t</db_fields>\n\t</db_table>\n</object>\n";
	}

	function serialize($option_serialize, $params_serialize, &$data_serialize) {
		if ($option_serialize == "fields_db2form") {
			for ($i_vars=0; $i_vars<$this->fields["count"]; $i_vars++) {
				$cmd = "\$data_serialize[".$this->fields[$i_vars]["form"]."]=\$params_serialize[".$this->fields[$i_vars]["database"]."];";
				eval($cmd);
			}
		} else if ($option_serialize == "fields_form2db") {
			for ($i_vars=0; $i_vars<$this->fields["count"]; $i_vars++) {
				$cmd = "\$data_serialize[".$this->fields[$i_vars]["database"]."]=\$params_serialize[".$this->fields[$i_vars]["form"]."];";
				eval($cmd);
			}
		}
	}

}
?>
