<?PHP

//
// Ziszap Portal System Objects.
//
// Sistema de objetos genericos de database.
// Llamadas gen�ricas de base de datos, de forma que se llama al objeto gen�rico
// de cada base de datos de data_object
//

class generical_object {
//	var $classname = "generical_object";

	private $fields;
	private $fields_external;
	private $fields_enum;
	private $tables;
	private $methods;
	private $transactions;
	private $interfaces;

	var $form_object;
	var $fields_error;

	function generical_object () {
		if (isset($this->classname)) {
			$object = $this->classname;
			require_once('data_object/od.xml.file.inc'); 
			$parser = new xml_file_object("/zz-object/".$object."/data.xml");
			$result = $parser->parse_xml_file();
//			print_r($parser);
			// Actualitzación de fields
			if (isset($parser->arrOutput[0]["FIELDS"][0]["FIELD"])) {
				foreach ($parser->arrOutput[0]["FIELDS"][0]["FIELD"] as $key => $value) {
					$this->fields[$value["attr"][0]["NAME"]] = $value["attr"][0];
					// Si es un enum guardem els valors del enum dins la variable $this->fields_enum.
					if (isset($value["FIELD_VALUE"])) {
						foreach ($value["FIELD_VALUE"] as $keyi => $valuei) {
							$this->fields_enum[$value["attr"][0]["NAME"]][] = $valuei["attr"][0];
						}
					}
				}
			}
			if (isset($parser->arrOutput[0]["FIELDS_EXTERNAL"][0]["FIELD_EXTERNAL"])) {
				foreach ($parser->arrOutput[0]["FIELDS_EXTERNAL"][0]["FIELD_EXTERNAL"] as $key => $value) {
//					print_r($value);
					$this->fields_external[$value["attr"][0]["NAME"]] = $value["attr"][0];				
				}
			}
			if (isset($parser->arrOutput[0]["TABLES"][0]["TABLE"])) {
				foreach ($parser->arrOutput[0]["TABLES"][0]["TABLE"] as $key => $value) {
					$this->tables[$key] = $value["attr"][0];
				}
			}
			if (isset($parser->arrOutput[0]["TABLES_EXTERNAL"][0]["TABLE_EXTERNAL"])) {
				foreach ($parser->arrOutput[0]["TABLES_EXTERNAL"][0]["TABLE_EXTERNAL"] as $key => $value) {
					$this->tables_external[$value["attr"][0]["NAME"]] = $value["attr"][0];
				}
			}
			if (isset($parser->arrOutput[0]["METHODS"][0]["METHOD"])) {
				foreach ($parser->arrOutput[0]["METHODS"][0]["METHOD"] as $key => $value) {
					$this->methods[$key] = $value["attr"][0];
					foreach ($value as $m_key => $meth) {
						if (!is_array($meth)) continue;
						if (isset($meth[1]["attr"][0]))
							foreach ($meth as $num => $cmd) {
								$cmd["attr"][0]["NAME"] = $cmd["name"];
								$this->methods[$key][] = $cmd["attr"][0];
							}
						else if (isset($meth[0]["attr"][0])){
							$meth[0]["attr"][0]["NAME"] = $meth[0]["name"];
							$this->methods[$key][] = $meth[0]["attr"][0];
						}
					} 
//					print_r($value);
				}
			}
			if (isset($parser->arrOutput[0]["TRANSACTIONS"][0]["TRANSACTION"])) {
				foreach ($parser->arrOutput[0]["TRANSACTIONS"][0]["TRANSACTION"] as $key => $value) {
					$this->transactions[$key] = $value["attr"][0];
					foreach ($value as $t_key => $tran) {
						if (!is_array($tran)) continue;
						if (isset($tran[1]["attr"][0]))
							foreach ($tran as $num => $cmd) {
								$this->transactions[$key][] = $cmd["attr"][0];
							}
						else if (isset($tran[0]["attr"][0])){
							$this->transactions[$key][] = $tran[0]["attr"][0];
						}
					} 
				}
			}

			if (isset($parser->arrOutput[0]["INTERFACES"][0]["INTERFACE"])) {
				foreach ($parser->arrOutput[0]["INTERFACES"][0]["INTERFACE"] as $key => $value) {
					$this->interfaces[$value["attr"][0]["NAME"]] = $value["attr"][0];
				}
			}
		}
	}

	function list_fields() {
		$external_field = array();
		if (isset($this->fields_external)) {
			foreach ($this->fields_external as $key => $value) {
				$external_field[$value["NAME"]] = $this->get_external_field($value["OBJECT"], $value["FIELD"]);
				if (isset($value["DEFAULT"]) && ! isset($external_field[$value["NAME"]]["DEFAULT"])) $external_field[$value["NAME"]]["DEFAULT"] = $value["DEFAULT"];
			}
		}
		return array_merge($this->fields, $external_field);
	}
	
	function list_enum_fields() {
		if (isset($this->fields_enum)) {
			return $this->fields_enum;
		} else {	
			return Array();
		}
	}
	
	function get_field ($field) {
		foreach ($this->list_fields() as $key => $value) {
			if ($value["NAME"] == $field)
				return $value;
		}
		return NULL;
	}
	
	function get_external_field ($object, $field) {
		$external_field = array();	
		require_once('data_object/od.xml.file.inc'); 
		$parser = new xml_file_object("/zz-object/".$object."/data.xml");
		$result = $parser->parse_xml_file();
		if (isset($parser->arrOutput[0]["FIELDS"][0]["FIELD"])) {
			foreach ($parser->arrOutput[0]["FIELDS"][0]["FIELD"] as $key => $value) {
				if ($value["attr"][0]["NAME"] == $field)	
					$external_field = $value["attr"][0];				
			}
		}
		if (! isset($external_field["NAME"])) {
			$this->object_error($field." not present in ".$object);
		}
		return $external_field;
	}

	function get_external_table ($object, $table) {
		$external_table = array();	
		require_once('data_object/od.xml.file.inc'); 
		$parser = new xml_file_object("/zz-object/".$object."/data.xml");
		$result = $parser->parse_xml_file();
		if (isset($parser->arrOutput[0]["TABLES"][0]["TABLE"])) {
			foreach ($parser->arrOutput[0]["TABLES"][0]["TABLE"] as $key => $value) {
				if ($value["attr"][0]["NAME"] == $table)	
					$external_table = $value["attr"][0];				
			}
		}
		if (! isset($external_table["NAME"])) {
			$this->object_error($table." not present in ".$object);
		}
		return $external_table;
	}
	
	function list_all_tables() {
		$returned_table = $this->tables;
		if (isset($this->tables_external)) {
			foreach ($this->tables_external as $key => $value) {
//				print_r($value);
				$returned_table[] = $this->get_external_table($value["OBJECT"], $value["NAME"]);
			}
		}
		return $returned_table;
	}
	
	function list_tables () {
		if (isset($this->tables)) {
			return $this->tables;
		} else {
			return Array();
		}	
	}
	
	function get_table ($table) {
		$txt_list_fields = "";
		$txt_list_externals = "";
		$table_return = array();
		$arr_xml_fields = array();
		
		foreach ($this->tables as $key => $value) {
			if ($value["NAME"] == $table) {
				$table_return = $value;
				$txt_list_fields = $value["FIELDS"];
				if (isset($value["EXTERNAL"])) $txt_list_externals = $value["EXTERNAL"];
			}
		}
		
		// 1. Paso: Tratamos los campos de la misma tabla.
		$arr_xml_fields = $this->list_fields();
		$arr_list_fields = explode(",", $txt_list_fields);
		foreach ($arr_list_fields as $key => $value) {
			$table_return["COLUMNS"][$value] = $arr_xml_fields[$value]; 
		}
		return $table_return;
	}
	
	function list_methods() {
		if (isset($this->methods)) {
			return $this->methods;
		} else {
			return Array();
		}	
	}
	
	function get_method ($method) {
		foreach ($this->methods as $key => $value) {
			if ($value["NAME"] == $method)
				return $value;
		}
		return NULL;
	}
	
	function list_input_methods ($method) {
		$var_arr = array();
		$methods = $this->get_method($method);
		if (is_null($methods)) return NULL;
		foreach ($this->get_method($method) as $key => $value) {
			if (is_array($value)) {
				if (isset ($value["NAME"])) {
					if ($value["NAME"] == "INPUT") {
						$var_arr[] = $value;
					}
				}
			}
		}
		return $var_arr;
	}
	
	function list_transactions() {
		if (isset($this->transactions)) {
			return $this->transactions;
		} else {
			return Array();
		}
	}
	
	function get_transaction ($transaction) {
		foreach ($this->transactions as $key => $value) {
			if ($value["NAME"] == $transaction)
				return $value;
		}
		return NULL;
	}

	function list_interfaces() {
		if (isset($this->interfaces)) {
			return $this->interfaces;
		} else {
			return Array();
		}	
	}
	
	function get_interface ($interface) {
		foreach ($this->interfaces as $key => $value) {
			if ($value["NAME"] == $interface)
				return $value;
		}
		return NULL;
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
		echo "<font face='Verdana,Arial' size=2><i>&lt;/data_object/ag.object.inc&gt;</i> <b>$strFILEError</b></font><br><br>";
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
