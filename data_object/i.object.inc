<?php
//
// Ziszap Portal System Objects.
//
// Sistema de objetos genericos de database.
// Llamadas gen�ricas de base de datos, de forma que se llama al objeto gen�rico
// de cada base de datos de data_object
//

require_once("zz-admin/config.php");
//require_once("db.mysql.inc");


abstract class Object {
	abstract protected function flush_data ();
	abstract function get_data ( &$data_out );
	abstract function set_data ( $data_in );
	
	protected $array_bbdd = Array();
	protected $index_bbdd;
	protected $count_bbdd;
	protected $qhandle;
	protected $bbdd_table;
	protected $errors_transactions;
	protected $where_scan;		// Variable que conté el Id necesari per la transacció. SCAN
	
	public $last_insert_id;
	
	function begin_database_transaction () {
		$this->errors_transactions = FALSE;	
		$sql = "START TRANSACTION;";
		@mysql_query($sql.";");
	}
	
	function end_database_transaction () {
		if (! $this->errors_transactions) {	
			$sql = "COMMIT;";
			@mysql_query($sql.";");
			return TRUE;
		} else {
			$sql = "ROLLBACK;";
			@mysql_query($sql.";");
			return FALSE;
		}
	}
	
	function failed_database_transaction () {
		$sql = "ROLLBACK;";
		@mysql_query($sql.";");
	}

	function begin_transaction() {
		global $_SESSION;
		
		$debug = debug_backtrace();
		$_SESSION["class"] = $debug[1]["class"];
		$_SESSION["function"] = $debug[1]["function"];
	}
	
	function end_transaction() {
		global $_SESSION;
		
		unset($_SESSION["class"]);
		unset($_SESSION["function"]);
	}

	function failed_transaction() {
		global $_SESSION;
		global $func_debug;
		
		unset($_SESSION["class"]);
		unset($_SESSION["function"]);
		if ( $func_debug ) {
			$func_backtrace = debug_backtrace();
			for ($i=0; $i<count($func_backtrace); $i++){
				echo $func_backtrace[$i]["file"]."=>".$func_backtrace[$i]["function"]."(".$func_backtrace[$i]["line"].")";
				if (isset($func_backtrace[$i]["args"])) var_dump($func_backtrace[$i]["args"]);
			}
		} 
		$this->errors_transactions = TRUE;
	}
	
	function get_fields ($table) {
		return $this->tables[$table];
	}
	function get_pKeyfields ($table) {
		return $this->pKey_tables[$table];
	}
	
	function create_object ($i_form, &$o_form) {
		$o_form = $i_form;
		return TRUE;
	}
	function delete_object ($i_form, &$o_form) {
		$o_form = $i_form;
		return TRUE;
	}
	function update_object ($i_form, &$o_form) {
		$entrada = Array();
		$entrada = $this->next_item();
		
		if (is_array($entrada)) {
			$o_form = array_merge($entrada, $i_form);
		} else {
			$o_form = $i_form;
		}
		return TRUE;
	}
	function update_select_object ($i_form, &$o_form) {
		$isFULL = TRUE;
		$entrada = Array();
			
		$entrada = $this->next_item();
		if (is_array($entrada)) {
			foreach ($entrada as $key => $value) {
				if (isset($i_form[$key])) {
					$entrada[$key] = $i_form[$key];
				} else {
					$isFULL = FALSE;
				}
			}
			$this->update_html_input($entrada, $entrada);
			if ($isFULL) $o_form = $i_form;
		} else {
			$o_form = array_merge($_GET, $_POST);
		}
		return TRUE;
		//print_r($o_form);
	}
	
	function list_object   ($i_form, &$o_form) {
		global $func_debug;
			
		$values = $this->first_item();
		while (! $this->is_empty()) {
			if (is_array($values))
				$o_form[] = $values;
			$values = $this->next_item();
		}	
		return TRUE;
	}
	
	function view_object   ($i_form, &$o_form) {
		$o_form = $this->next_item();
		return TRUE;
	}
	
	function table_loaddata ($i_form, &$o_form) {
		global $func_debug;
		$fieldsPKey = Array();
		$arr_fields = Array();

		header('Content-Type: text/xml');
		
//		print_r($i_form);
		$tablesBBDD = explode(",", $this->bbdd_table);
		foreach ($tablesBBDD as $key => $value) {
			foreach (explode(",",$this->pKey_tables[$value]) as $key_field => $value_field) {
				$fieldsPKey[$value_field] = $value.".".$value_field;
			}
		}

//		foreach ($tablesBBDD as $key => $value) {
//			$fieldsPKey = array_merge($fieldsPKey, explode(",",$this->pKey_tables[$value]));
//		}
//		print_r($fieldsPKey);

		foreach ($tablesBBDD as $key => $value) {
			foreach (explode(",",$this->tables[$value]) as $key_field => $value_field) {
				if (! isset($arr_fields[$value_field])) {	
					$arr_fields[$value_field] = $value;
				}
			}
		}
//		if ($func_debug) print_r($arr_fields);
		$xml = '<?xml version="1.0" encoding="utf-8" ?>'."\n";
		$xml.= "<table><metadata>\n";

		$values = $this->first_item();
		if (! $this->is_empty()) {
			if (is_array($values)) {
//				print_r($values);
				foreach ($values as $key => $value) {
					if ($this->fieldsValidate[$key] == "int") $type_key = "integer";
					else if ($this->fieldsValidate[$key] == "float") $type_key = "double";
//					else if ($this->fieldsValidate[$key] == "date") $type_key = "date";
					else $type_key = "string";

// TODO: A revisar amb les entrades.
//					$pkey_key = (isset($fieldsPKey[$key])) ? "false" : "true";
					$pkey_key = "true";

					$xml.= "<column name='".$arr_fields[$key].".".$key."' label='".$this->fieldsText[$key]."' datatype='".$type_key."' editable='".$pkey_key."'>\n";
//					$grid->addColumn($arr_fields[$key].'.'.$key, $this->fieldsText[$key], $type_key, NULL, $pkey_key, $key);
					$xml.= "</column>\n";
				}
			}
			$values = $this->next_item();
		}
		$xml.= "</metadata><data pkey=\"";
		foreach ($fieldsPKey as $key => $value) {
			$xml .= $value." ";
		}
		$xml .= "\">\n";
		
		$key_counter = 1;
		while (! $this->is_empty()) {
			if (is_array($values)) {
				$xml .= "<row id='".$key_counter++."'";
				foreach ($tablesBBDD as $key => $value) {
					$table = $value;
					foreach (explode(",",$this->pKey_tables[$value]) as $key_field => $value_field) {
						$xml .= " ".$value.".".$value_field.".pkey"." = '".$values[$value_field]."'";
					}
				}
				$xml.= ">\n";
				foreach ($values as $key => $info) {
					$xml.= "<column name='".$arr_fields[$key].".".$key."'><![CDATA[".$info."]]></column>\n";
				}
				$xml.= "</row>\n";
			}
//			$grid->renderXML($values, array("PROVETES.pkey" => "prv_id", "SERIES.pkey" => "ser_id"));
			$values = $this->next_item();
			if (is_array($values))
				$o_form[] = $values;
		}
		$xml .= "</data></table>\n";
		echo $xml;
		die();
	}

	function sql_search ($fieldsSQL, $table, $data_in, $needle, $haystack) {
		global $func_debug;
		$fieldsSELECT = $fieldsSQL[0];
		$index_select = 1;
		$fieldsHAYSTACK = $haystack[0];
		$index_haystack = 1;

		for (; $index_select < count($fieldsSQL) ; $index_select++) {
			$fieldsSELECT .= ",". $fieldsSQL[$index_select];
		}
		
		for (; $index_haystack < count($haystack) ; $index_haystack++) {
			$fieldsHAYSTACK .= ",". $haystack[$index_haystack];
		}
			
		$sql = "SELECT ".$fieldsSELECT.", MATCH(".$fieldsHAYSTACK.") AGAINST ('".$needle."' in boolean mode) as relevancy FROM ".$table." WHERE MATCH(".$fieldsHAYSTACK.") AGAINST ('".$needle."' in boolean mode) order by relevancy desc";
		if ($func_debug) echo $sql."\n";
		unset($this->qhandle);
		// Si hay problema hay que verificar lo siguiente:
		// ALTER TABLE nombretabla engine=MyISAM;
		// ALTER TABLE nombretabla ADD FULLTEXT(<campos $haystack>);
		// Para eliminar el antiguo: ALTER TABLE nombretabla DROP nombreindex;
		// Para ver la estructura de la tabla: SHOW CREATE TABLE nombretabla;
		$this->qhandle = @mysql_query($sql.";") or $this->object_error("sql_search ".$table." failed");
		$this->index_bbdd = 0;
		$this->count_bbdd = mysql_num_rows($this->qhandle);
		$this->bbdd_table = $table;
		return TRUE;
	}

	function sql_scan ($SQL, $data_in) {
		global $func_debug;
		
		if ((!isset($SQL["field"])) || (!isset($SQL["table"]))) {
			$this->object_error("Not enought parameters in sql_scan");
			return FALSE;
		}
		
		$fieldsSELECT = "";
		$fieldsWHERE = "";
		$first_time = TRUE;
		foreach ($SQL["field"] as $key => $value) {
			if (!$first_time){
				$fieldsSELECT .= ",";
				$fieldsWHERE .= " AND ";
			} else {
				$first_time = FALSE;
			}
			if (isset($data_in[$value])) {
				$fieldsSELECT .= $value;
				$fieldsWHERE .= $value." = '".mysql_real_escape_string($data_in[$value])."'";
			}
			else {
				return FALSE;
			}
		}
		$sql = "SELECT ".$fieldsSELECT." FROM ".implode(",", $SQL["table"])." WHERE ".$fieldsWHERE;
		if ($func_debug) echo $sql."\n";
		$qhandle = @mysql_query($sql.";") or $this->object_error("sql_scan ".implode(",", $SQL["table"])." failed");
		if (mysql_num_rows($qhandle) > 0) return TRUE;
		else return FALSE;
	}
	
	function sql_array_read ($SQL, &$data_in) {
		global $func_debug;
		$values = Array();
		
		$this->sql_read($SQL, $data_in);
		$values = $this->first_item();
		while (! $this->is_empty()) {
			if (is_array($values)) {
				$data_in[$SQL["array"]][] = $values;
			}
			$values = $this->next_item();
		}
		return TRUE;
	}

	function sql_read ($SQL, &$data_in) {
		global $func_debug;

		if ((!isset($SQL["field"])) || (!isset($SQL["table"]))) {
			$this->object_error("Not enought parameters in sql_read");
			return FALSE;
		}
		
		$fieldsSELECT = "";
		$fieldsWHERE = "";
		$first_time = TRUE;
		$arr_fields = Array();
		$arr_WHERE = Array();
		
		foreach ($SQL["table"] as $key => $value) {
			foreach (explode(",",$this->tables[$value]) as $key_field => $value_field) {
				if (! isset($arr_fields[$value_field])) {	
					$arr_fields[$value_field] = $value;
				} else {
					if (!$first_time) {
						$fieldsWHERE .= " AND ";
					} else {
						$first_time = FALSE;
					}
					$fieldsWHERE .= $value.".".$value_field." = ".$arr_fields[$value_field].".".$value_field;
				}
			}
			$arr_WHERE = array_unique(array_merge($arr_WHERE, explode(",",$this->tables[$value])));
//			$arr_JOIN_temp = Array();
//			for ($key2 = $key + 1; $key2 < count($tbl_WHERE); $key2++) {
//				$arr_JOIN_temp = array_intersect(explode(",",$this->tables[$value]), explode(",",$this->tables[$tbl_WHERE[$key2]]));
//				if (count($arr_JOIN_temp) > 0) {
//					foreach ($arr_JOIN_temp as $key_join => $value_join) {
//						if (!$first_time) {
//							$fieldsWHERE .= " AND ";
//						} else {
//							$first_time = FALSE;
//						}
//						$fieldsWHERE .= $value.".".$value_join." = ".$tbl_WHERE[$key2].".".$value_join; 
//					}
//				}
//			}
		}
		
		foreach ($arr_WHERE as $key => $value) {
			if (isset($data_in[$value])) {
				if (!$first_time) {
					$fieldsWHERE .= " AND ";
				} else {
					$first_time = FALSE;
				}
				if (($this->fieldsValidate[$value] != "float") && ($this->fieldsValidate[$value] != "double")) {
					$fieldsWHERE .= $arr_fields[$value].".".$value." = '".mysql_real_escape_string($data_in[$value])."'";
				} else {
					if (is_numeric($data_in[$value]))	
						$fieldsWHERE .= "ROUND(".$arr_fields[$value].".".$value.",3) = ROUND(".$data_in[$value].",3)";
					else {
						$fieldsWHERE .= $arr_fields[$value].".".$value." = '".mysql_real_escape_string($data_in[$value])."'";
					}
				}
			}
		}

		$first_time = TRUE;
		// TODO: Si algun cap esta en $data_in i no està en $fieldsSQL cal que ho posi en el where.
		foreach ($SQL["field"] as $key => $value) {
			if (!$first_time){
				$fieldsSELECT .= ",";
			} else {
				$first_time = FALSE;
			}

			$fieldsSELECT.= $arr_fields[$value].".".$value;
		}

		$table = implode(",", $SQL["table"]);
		if (($fieldsSELECT == "") && ($fieldsWHERE == "")) {
			$sql = "SELECT * FROM ".$table;
		} elseif ($fieldsWHERE == "") {
			$sql = "SELECT ".$fieldsSELECT." FROM ".$table;
		} elseif ($fieldsSELECT == "") {
			$sql = "SELECT * FROM ".$table." WHERE ".$fieldsWHERE;
		} else {
			$sql = "SELECT ".$fieldsSELECT." FROM ".$table." WHERE ".$fieldsWHERE;
		}
		if ($func_debug) echo $sql."\n";
		unset($this->qhandle);
		$this->qhandle = @mysql_query($sql.";") or $this->object_error("sql_read ".$table." failed");
		$this->index_bbdd = 0;
		$this->count_bbdd = mysql_num_rows($this->qhandle);
		$this->bbdd_table = $SQL["table"];
		return TRUE;
	}

	function sql_exec ($sql, $data_out ) {
		global $func_debug;
		$strInfo = "";
		
		if ($func_debug) echo $sql."\n";
		unset($this->qhandle);
		$this->qhandle = @mysql_query($sql.";") or $this->object_error("sql_exec ".$table." failed");
//		$this->count_bbdd = mysql_num_rows($this->qhandle);
//		$this->bbdd_table = $table;
		return TRUE;
	}
	
	function sql_array_write ($SQL, $data_out) {
		global $func_debug;
		
		$pExt_tables_all = Array();
		foreach ($SQL["table"] as $key => $value) {
			$pExt_tables_all = array_merge($pExt_tables_all, explode(",", $this->pExt_tables[$value]));
		}
		
		foreach ($data_out[$SQL["array"]] as $key => $value) {
			$SQL_iter = Array();
			$SQL_iter["field"] = array_merge($SQL["field"], $pExt_tables_all);
			$SQL_iter["table"] = $SQL["table"];
			$data_out_iter = $value; 
			foreach ($SQL_iter["field"] as $key_iter => $value_iter) {
				if (isset($data_out[$value_iter])) {
					$data_out_iter[$value_iter] = $data_out[$value_iter];
				}	
			}
//			var_dump($SQL_iter);
//			var_dump($data_out_iter);
			$this->sql_write($SQL_iter, $data_out_iter);
		}
		return TRUE;
	}
	
	function sql_write ($SQL, $data_out) { 
		global $func_debug;
		$fieldsSET = "";
		$fieldsWHERE = "";
		$first_time = TRUE;
		
		if ((!isset($SQL["field"])) || (!isset($SQL["table"]))) {
			$this->object_error("Not enought parameters in sql_write");
			return FALSE;
		}

		foreach ($SQL["field"] as $key => $value) {
			if (!$first_time){
				$fieldsSET .= ",";
			} else {
				$first_time = FALSE;
			}
			if (isset($data_out[$value])) $fieldsSET .= $value." = '".mysql_real_escape_string($data_out[$value])."'";
			else {
				if (in_array($value, $this->fieldsRequired)) {
					$this->object_error("Not enought fields was required in sql_write");
					return FALSE;
				}
				else $fieldsSET .= $value." = ''";
			}
		}
		
		$fieldsPKey = Array();
		foreach ($SQL["table"] as $key_table => $value_table) {
			$fieldsPKey = array_merge($fieldsPKey, explode(",", $this->pKey_tables[$value_table]));
		}

		$first_time = TRUE;
		foreach ($fieldsPKey as $key => $value) {
			if (isset($data_out[$value])){
				if (!$first_time){
					$fieldsWHERE .= " AND ";
				} else {
					$first_time = FALSE;
				}
				$fieldsWHERE .= $value." = '".mysql_real_escape_string($data_out[$value])."'";
			}
		}
		
		$table = implode(",", $SQL["table"]);
		$sql = "UPDATE ".$table." SET ".$fieldsSET." WHERE ".$fieldsWHERE;
		if ($func_debug) echo $sql."\n";
		$qhandle = @mysql_query($sql.";") or $this->object_error("sql_write ".$table." failed");
		$this->last_insert_id = @mysql_insert_id();
		return TRUE;
	}

	function sql_create ($SQL, $data_out) {
		global $func_debug;
		$fieldsINSERT = "";
		$fieldsVALUES = "";
		$first_time = TRUE;
		
		if ((!isset($SQL["field"])) || (!isset($SQL["table"]))) {
			$this->object_error("Not enought parameters in sql_create");
			return FALSE;
		}

		foreach ($SQL["field"] as $key => $value) {
			if (!$first_time){
				$fieldsINSERT .= ",";
				$fieldsVALUES .= ",";
			} else {
				$first_time = FALSE;
			}
			$fieldsINSERT .= $value;
			if (isset($data_out[$value])) $fieldsVALUES .= "'".mysql_real_escape_string($data_out[$value])."'";
			else {
				if (in_array($value, $this->fieldsRequired)){
					if ($func_debug) {
						$this->object_error("Not enought parameters in _GET o _POST ".$value);
					}
					return FALSE;
				} 
				else $fieldsVALUES .= "''";
			}
		}

		$table = implode(",", $SQL["table"]);
		$sql = "INSERT INTO ".$table." ( ".$fieldsINSERT." ) VALUES (".$fieldsVALUES.")";
		if ($func_debug) echo $sql."\n";
		$qhandle = @mysql_query($sql.";") or $this->object_error("sql_create ".$table." failed");
		$this->last_insert_id = @mysql_insert_id();
		return TRUE;
	}
	
	function sql_delete ($SQL, &$data_in) {
		global $func_debug;
		$fieldsWHERE = "";
		$first_time = TRUE;
		
		if ((!isset($SQL["field"])) || (!isset($SQL["table"]))) {
			$this->object_error("Not enought parameters in sql_delete");
			return FALSE;
		}
		
		foreach ($SQL["field"] as $key => $value) {
			if (!$first_time){
				$fieldsWHERE .= " AND ";
			} else {
				$first_time = FALSE;
			}
			if (isset($data_in[$value])) {
				$fieldsWHERE .= $value." = '".mysql_real_escape_string($data_in[$value])."'";
			}
			else {
				return FALSE;
			}
		}
		
		$table = implode(",", $SQL["table"]);
		$sql = "DELETE FROM ".$table." WHERE ".$fieldsWHERE;
		if ($func_debug) echo $sql."\n";

		$qhandle = @mysql_query($sql.";") or $this->object_error("sql_delete ".$table." failed");
		return TRUE;
	}

	function first_item() {
		$this->count_bbdd = mysql_num_rows($this->qhandle);
		if ($this->count_bbdd == 0) {
			unset($this->array_bbdd);
			$this->flush_data();
			return FALSE;
		}
		$row = mysql_fetch_assoc($this->qhandle);
		
		$this->set_data($row);
		unset($this->array_bbdd);
		$this->array_bbdd = $row;
		$this->index_bbdd = 1;
		return $row;
	}
	
	function next_item() {
		unset($this->array_bbdd);
		if ($this->count_bbdd <= $this->index_bbdd){
			$this->flush_data();	
			return FALSE;
		}	

		$row = mysql_fetch_assoc($this->qhandle);

		$this->set_data($row);
		$this->array_bbdd = $row;
		$this->index_bbdd = $this->index_bbdd + 1;
		return $row;
	}
	
	function is_empty() {
		if (isset($this->array_bbdd)) 
			return FALSE;
		else 
			return TRUE;
	}
	
	function serialize_to_array (&$data_out) {
		$values = $this->first_item();
		if (! $values) {
			return FALSE;
		}
		while (! $this->is_empty()) {
			if (is_array($values))
				$data_out[] = $values;
			$values = $this->next_item();
		}	
		return TRUE;
	}

	function create_html_input ($listFields, $data_in = Array()) {
		global $func_debug;
//		print_r($this->tables);
		$returned_value = FALSE;
		foreach ($listFields as $key => $value) {
			if ((isset($this->fieldsValidate[$key])) && (! isset($data_in[$key]))) {
				if (! $returned_value) {
					echo "<table class=\"formclass\" border=\"0\">\n";
				}
				$returned_value = TRUE;
				echo "\t<tr>\n";
				echo "\t\t<td class=\"labeltd\"><label for=\"c".$key."\">".$value."</label></td>\n";
				echo "\t\t<td class=\"contenttd\">";
				if ($this->fieldsValidate[$key] == "varchar") {
					echo '<input id="c'.$key.'" type="text" name="'.$key.'"';
				} else if (($this->fieldsValidate[$key] == "int") || ($this->fieldsValidate[$key] == "float")){
					echo '<input id="c'.$key.'" type="digits" name="'.$key.'"';
					
				} else if ($this->fieldsValidate[$key] == "date") {
					echo '<input id="c'.$key.'" type="date" name="'.$key.'"';
				} else if ($this->fieldsValidate[$key] == "enum") {
					$enum_values = Array();
					$cmd = "\$enum_values = \$this->enum_".$key.";\n";
					eval($cmd);
					echo '<select id="c'.$key.'" name="'.$key.'" >';
					foreach ($enum_values as $keyi => $valuei) {
						echo '<option value="'.$keyi.'">'.$valuei.'</option>';
					}
					echo '</select>';
				} else if ($this->fieldsValidate[$key] == "text") {
					echo '<textarea id="c'.$key.'" name="'.$key.'" rows="4" cols="50"';
				}
				if (in_array($key, $this->fieldsRequired) && $this->fieldsValidate[$key] != "enum") 
					echo ' class="required"';

				if ($this->fieldsValidate[$key] == "text") {
					echo ' ></textarea>';
				} else if ($this->fieldsValidate[$key] != "enum") echo ' />';
				echo "</td>\n";

				if ($func_debug) echo "\t\t<td class=\"debugtd\"><font color='red'>[ \$".$key." ]</font></td>\n";
				else echo "\t\t<td width=\"0\"> </td>"; 
				echo "\t</tr>\n";
			}
			else if (! isset($data_in[$key])) {
				$returned_value = TRUE;
				echo "\t<tr>\n";
				echo "\t\t<td><label for=\"c".$key."\">".$value."</label></td>\n";
				echo '\t\t<td><input id="c'.$key.'" type="text" name="'.$key.'" /></td>'."\n";
				if ($func_debug) echo "\t\t<td class=\"debugtd\"><font color='red'>[ \$".$key." ]</font></td>\n";
				echo "\t</tr>\n";	
			}
		}
		if ($returned_value) echo "</table>\n";
		return $returned_value;
	}
	
	function get_fieldText ($key) {
		if (isset($this->fieldsText[$key])) {
			return $this->fieldsText[$key];
		}
	}
	
	function get_fieldType ($key) {
		if (isset($this->fieldsValidate[$key])) {
			return $this->fieldsValidate[$key];
		}
	}
	
	function update_html_input ($listFields, $data_in = Array()) {
		global $func_debug;

		$returned_value = FALSE;
		foreach ($listFields as $key => $value) {
			if (isset($this->fieldsValidate[$key])) {
				if (! $returned_value) {
					echo "<table class=\"formclass\" border=\"0\">\n";
				}
				$returned_value = TRUE;
				echo "\t<tr>\n";
				echo "\t\t<td class=\"labeltd\"><label for=\"c".$key."\">".$this->fieldsText[$key]."</label></td>\n";
				echo "\t\t<td class=\"contenttd\">";
				if ($this->fieldsValidate[$key] == "varchar") {
					echo '<input id="c'.$key.'" type="text" name="'.$key.'" value="'.$data_in[$key].'" ';
				} else if (($this->fieldsValidate[$key] == "int") || ($this->fieldsValidate[$key] == "float")){
					echo '<input id="c'.$key.'" type="digits" name="'.$key.'" value="'.$data_in[$key].'" ';
				} else if ($this->fieldsValidate[$key] == "date") {
					echo '<input id="c'.$key.'" type="date" name="'.$key.'" value="'.$data_in[$key].'" ';
				} else if ($this->fieldsValidate[$key] == "enum") {
					$enum_values = Array();
					$cmd = "\$enum_values = \$this->enum_".$key.";\n";
					eval($cmd);
					echo '<select id="c'.$key.'" name="'.$key.'">';
					foreach ($enum_values as $keyi => $valuei) {
						if ($data_in[$key] == $keyi) {
							echo '<option selected value="'.$keyi.'">';
						} else {
							echo '<option value="'.$keyi.'">';
						}
						echo $valuei.'</option>\n';
					}
					echo '</select>';
				} else if ($this->fieldsValidate[$key] == "text"){
					echo '<textarea id="c'.$key.'" rows="4" cols="50" name="'.$key.'" ';
				}
				if (in_array($key, $this->fieldsRequired) && $this->fieldsValidate[$key] != "enum") 
					echo ' class="required"';

				if ($this->fieldsValidate[$key] == "text") {
					echo '>'.$data_in[$key].'</textarea>';
				} else if ($this->fieldsValidate[$key] != "enum") echo ' />';
				echo "</td>\n";

				if ($func_debug) echo "\t\t<td class=\"debugtd\"><font color='red'>[ \$".$key." ]</font></td>\n";
				else echo "\t\t<td width=\"0\"> </td>"; 
				echo "\t</tr>\n";
			}
			else {
				$returned_value = TRUE;
				echo "\t<tr>\n";
				echo "\t\t<td><label for=\"c".$key."\">".$value."</label></td>\n";
				echo '\t\t<td><input id="c'.$key.'" type="text" name="'.$key.'" value="'.$data_in[$key].'"/></td>'."\n";
				if ($func_debug) echo "\t\t<td class=\"debugtd\"><font color='red'>[ \$".$key." ]</font></td>\n";
				echo "\t</tr>\n";	
			}
		}
		if ($returned_value) echo "</table>\n"; 
		return $returned_value;
	}

	function update_value_input ($keys, $data_in) {

		$arr_keys = explode(",",$keys);
		if (count($arr_keys) > 0) echo "<p>\n";
		foreach ($arr_keys as $k => $key) {
			if ($this->fieldsValidate[$key] == "varchar") {
				echo '<input id="c'.$key.'" type="text" name="'.$key.'" value="'.$data_in[$key].'" ';
			} else if (($this->fieldsValidate[$key] == "int") || ($this->fieldsValidate[$key] == "float")){
				echo '<input id="c'.$key.'" type="digits" name="'.$key.'" value="'.$data_in[$key].'" ';
				
			} else if ($this->fieldsValidate[$key] == "date") {
				echo '<input id="c'.$key.'" type="date" name="'.$key.'" value="'.$data_in[$key].'" ';
			} else if ($this->fieldsValidate[$key] == "enum") {
				$enum_values = Array();
				$cmd = "\$enum_values = \$this->enum_".$key.";\n";
				eval($cmd);
				echo '<select id="c'.$key.'" name="'.$key.'">';
				foreach ($enum_values as $keyi => $valuei) {
					if ($data_in[$key] == $keyi) {
						echo '<option selected value="'.$keyi.'">';
					} else {
						echo '<option value="'.$keyi.'">';
					}
					echo $valuei.'</option>\n';
				}
				echo '</select>';
			} else if ($this->fieldsValidate[$key] == "text"){
				echo '<textarea id="c'.$key.'" rows="4" cols="50" name="'.$key.'" ';
			}
			if (in_array($key, $this->fieldsRequired) && $this->fieldsValidate[$key] != "enum") 
				echo ' class="required"';
			if ($this->fieldsValidate[$key] == "text") {
				echo '>'.$data_in[$key].'</textarea>'."\n";
			} else if ($this->fieldsValidate[$key] != "enum") echo ' />'."\n";
		}
		if (count($arr_keys) > 0) echo "</p>\n";
	}

	function view_value_fields($keys, $data_in, $separator="") {
		$arr_keys = explode(",",$keys);
		if (count($arr_keys) > 0) echo "<p>\n";
		foreach ($arr_keys as $k => $key) {
			if (($this->fieldsValidate[$key] == "varchar") || ($this->fieldsValidate[$key] == "int") || ($this->fieldsValidate[$key] == "float") || ($this->fieldsValidate[$key] == "date")) {
				echo $data_in[$key];
			} else if ($this->fieldsValidate[$key] == "enum") {
				$enum_values = Array();
				$cmd = "\$enum_values = \$this->enum_".$key.";\n";
				eval($cmd);
				if (is_array($data_in)) {
					foreach ($enum_values as $keyi => $valuei) {
						if ($data_in[$key] == $keyi) 
							echo $valuei;
					}
				} else {
					if (isset($enum_values[$data_in])) {
						echo $enum_values[$data_in];
					} 
				}
			}
			echo $separator;
		}
		if (count($arr_keys) > 0) echo "</p>\n";
	}
	
	function object_error($strFILEError)
	{
		echo "<br><br>";
		$func_backtrace = debug_backtrace();
		echo "<table border='0' cellpadding='1' cellspacing='1' width='100%'><tr><td bgcolor='#514537'>";
		echo "<table border='0' cellpadding='5' cellspacing='0' width='100%'>";
		echo "<tr bgcolor='#EEEEEE'>";
		echo "<td width='20' valign='top'><img src='rcrs/gif/error.gif' hspace='3'></td>";
		echo "<td valign='middle'>";
		echo "<font face='Verdana,Arial' size=2><i>&lt;/data_object/i.object.inc&gt;</i> <b>$strFILEError</b></font><br><br>";
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

if (!isset($db_connhandle))
	$db_connhandle = @mysql_connect($db_host, $db_user, $db_passwd) or Object::object_error("connect_server_database");;
if (!isset($db_syshandle))
	$db_syshandle = @mysql_select_db($database_name) or Object::object_error("select_database");

?>