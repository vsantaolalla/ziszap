<?php

require_once("data_object/o.database.php");

class database_object extends generical_object_database {

	var $sys_datahandle;
	var $sys_connhandle;
	var $sys_dbhandle;

	function database_object () {
		global $db_connhandle;
		global $db_syshandle;

		$this->sys_connhandle = $db_connhandle; 
		$this->sys_dbhandle = $db_syshandle; 
		parent::generical_object_database();
	}

	function explain_table ($n_table){
		$qhandle = @mysql_query("explain ".$n_table.";") or $this->db_mysql_die("explain ".$n_table." failed");
		$returned_array = Array();
		$matches = Array();
		while ($row = mysql_fetch_array($qhandle)) {
			$name = $row[0];
			$returned_array[$name]["NAME"] = $name;
			preg_match('/(?P<TYPE>\w+)\((?P<CARDINAL>\d+)\)/', $row[1], $matches);
			if (isset($matches["TYPE"]))
				$returned_array[$name]["TYPE"] = $matches["TYPE"];
			if (isset($matches["CARDINAL"]))
				$returned_array[$name]["CARDINAL"] = $matches["CARDINAL"];
			unset($matches);
			$returned_array[$name]["NULL"] = $row[2];
			$returned_array[$name]["key"] = $row[3];
			$returned_array[$name]["DEFAULT"] = $row[4];
			$returned_array[$name]["extra"] = $row[5];
		}
		return $returned_array;
	}

	function list_db_tables () {
		$qhandle = @mysql_query("show tables;") or $this->db_mysql_die("list_db_tables failed.");
		$returned_array = array();
		while ($row = mysql_fetch_array($qhandle)) {
			$returned_array[] = $row[0];
		}
		return $returned_array;
	}

	function db_synchronize_keys_tables ($table_xml_def) {

//		print_r($table_xml_def);

		// Revisamos el autoincrement
		$list_db_autoincrement_keys = "";
		$list_xml_autoincrement_keys = "";
		
		if (isset($table_xml_def["AUTOINCREMENT"])) {
			$list_xml_autoincrement_keys = $table_xml_def["AUTOINCREMENT"];
		}
				
		$qhandle = @mysql_query("explain ".$table_xml_def["NAME"].";") or $this->db_mysql_die("db_synchronize_keys_tables: Error explain ".$table_xml_def["NAME"]." failed");
		while ($row = mysql_fetch_array($qhandle)) {
			if ($row["Extra"] == "auto_increment") {
				$list_db_autoincrement_keys = $row["Field"];
			}	
		}

//		echo "BD: ".$list_db_autoincrement_keys." -- XML: ".$list_xml_autoincrement_keys."\n";
		if ($list_db_autoincrement_keys != $list_xml_autoincrement_keys) {
			// Esborrem l'anterior autoincrement.
			if ($list_db_autoincrement_keys != "")
				$this->alter_change($table_xml_def["NAME"], $table_xml_def["COLUMNS"][$list_db_autoincrement_keys]);
		}

		// Obtenemos las claves primarias, unicas y indices del XML (Definicion de la tabla).
		$list_xml_primary_keys = Array();
		$list_xml_unique_keys = Array();
		$list_xml_index_keys = Array();

		if (isset($table_xml_def["PKEY"])) {
			$list_xml_primary_keys = explode(",", $table_xml_def["PKEY"]);
		}
		if (isset($table_xml_def["UKEY"])) {
			$list_xml_unique_keys  = explode(",", $table_xml_def["UKEY"]);
		}
		if (isset($table_xml_def["IKEY"])) {
			$list_xml_index_keys   = explode(",", $table_xml_def["IKEY"]);
		}

		// Obtenemos las claves primarias, unicas y indices de la BD.
		$list_db_primary_keys = Array();
		$list_db_unique_keys = Array();
		$list_db_index_keys = Array();

		$qhandle = @mysql_query("show keys from ".$table_xml_def["NAME"].";") or $this->db_mysql_die("show_keys failed.");
		$returned_array = Array();
		while ($row = mysql_fetch_array($qhandle)) {
//			print_r($row);
			if ($row["Key_name"] == "PRIMARY") {
				$list_db_primary_keys[] = $row["Column_name"];
			} else {
				if ($row["Non_unique"] == 0) {
					$list_db_unique_keys[] = $row["Key_name"];
				} else {
					$list_db_index_keys[] = $row["Key_name"];
				}
			}
		}
//		print_r(array_diff($list_db_primary_keys, $list_xml_primary_keys));
//		print_r(array_diff($list_xml_primary_keys, $list_db_primary_keys));

		if (count(array_merge(array_diff($list_db_primary_keys, $list_xml_primary_keys),array_diff($list_xml_primary_keys, $list_db_primary_keys))) > 0) {
			// TODO: Aplicar alter_change a todos los campos que se modifiquen. 	
			if (count($list_db_primary_keys) > 0) {
				$qhandle = @mysql_query("ALTER TABLE ".$table_xml_def["NAME"]." DROP PRIMARY KEY;" ) or $this->db_mysql_die("db_synchronize_keys_tables: Error primary_keys");
			}
			if (isset($table_xml_def["PKEY"]) && ($table_xml_def["PKEY"] != "")) {
//				echo "ALTER TABLE ".$table_xml_def["NAME"]." ADD PRIMARY KEY (".$table_xml_def["PKEY"].");";	
				$qhandle = @mysql_query("ALTER TABLE ".$table_xml_def["NAME"]." ADD PRIMARY KEY (".$table_xml_def["PKEY"].");") or $this->db_mysql_die("db_synchronize_keys_tables: Error primary_keys");
			}
		}

		if (count(array_merge(array_diff($list_db_unique_keys, $list_xml_unique_keys),array_diff($list_xml_unique_keys, $list_db_unique_keys))) > 0) {
			if (count(array_diff($list_db_unique_keys, $list_xml_unique_keys)) > 0) {
				foreach (array_diff($list_db_unique_keys, $list_xml_unique_keys) as $key => $value) {
					if ((isset($value)) && ($value != "")) {	
						$qhandle = @mysql_query("ALTER TABLE ".$table_xml_def["NAME"]." DROP INDEX ".$value.";") or $this->db_mysql_die("db_synchronize_keys_tables: Error unique_keys");
					}
				}	
			} else {
				foreach (array_diff($list_xml_unique_keys, $list_db_unique_keys) as $key => $value) {
					if ((isset($value)) && ($value != "")) {
						$qhandle = @mysql_query("ALTER TABLE ".$table_xml_def["NAME"]." ADD UNIQUE (".$value.");") or $this->db_mysql_die("db_synchronize_keys_tables: Error unique_keys");
					}
				}
			}
		}

		if (count(array_merge(array_diff($list_db_index_keys, $list_xml_index_keys),array_diff($list_xml_index_keys, $list_db_index_keys))) > 0) {
			if (count(array_diff($list_db_index_keys, $list_xml_index_keys)) > 0) {
				foreach (array_diff($list_db_index_keys, $list_xml_index_keys) as $key => $value) {
					if ((isset($value)) && ($value != "")) {
						$qhandle = @mysql_query("ALTER TABLE ".$table_xml_def["NAME"]." DROP INDEX ".$value.";") or $this->db_mysql_die("db_synchronize_keys_tables: Error index_keys");
					}
				}	
			} else {
				foreach (array_diff($list_xml_index_keys, $list_db_index_keys) as $key => $value) {
					if ((isset($value)) && ($value != "")) {
						$qhandle = @mysql_query("ALTER TABLE ".$table_xml_def["NAME"]." ADD INDEX (".$value.");") or $this->db_mysql_die("db_synchronize_keys_tables: Error index_keys");
					}
				}
			}
		}

		if ($list_db_autoincrement_keys != $list_xml_autoincrement_keys) {
			// Afegim l'actual autoincrement.
			if ($list_xml_autoincrement_keys != "") {
				$table_xml_def["COLUMNS"][$list_xml_autoincrement_keys]["EXTRA"] = "auto_increment";
				$this->alter_change($table_xml_def["NAME"], $table_xml_def["COLUMNS"][$list_xml_autoincrement_keys]);
				unset($table_xml_def["COLUMNS"][$list_xml_autoincrement_keys]["EXTRA"]);
			}
		}
		
		return $returned_array;
	}

	function serialize($option_serialize, $params_serialize, &$data_serialize){
		if ($option_serialize == "outputid") {
			$this->sys_datahandle = @mysql_query($params_serialize["serialize.query"], $this->sys_connhandle) or $this->db_mysql_die($params_serialize["serialize.query"]);
			$this->sys_datahandle = @mysql_query("select LAST_INSERT_ID() as id", $this->sys_connhandle) or $this->db_mysql_die($params_serialize["serialize.query"]);
			while ($row = mysql_fetch_assoc($this->sys_datahandle)) {
				$data_serialize["id"] = $row["id"];
			}
			mysql_free_result($this->sys_datahandle);
		}
		if (($option_serialize == "output") || ($option_serialize == "outputgpc")) {
			$this->sys_datahandle = @mysql_query($params_serialize["serialize.query"],$this->sys_connhandle) or $this->db_mysql_die($params_serialize["serialize.query"]);
//			$this->sys_datahandle = @mysql_db_query($this->sys_dbname,$params_serialize["serialize.query"]) or $this->db_mysql_die($params_serialize["serialize.query"]);
			$data_number = 0;
			if (@mysql_num_rows($this->sys_datahandle) > 0) {
				while ($row=mysql_fetch_assoc($this->sys_datahandle)) {
					if ($option_serialize == "outputgpc")
						while (list($clave,$valor) = each($row))
							$row[$clave] = preg_replace("/'/","''",$row[$clave]);
					$data_serialize[$data_number] = $row;
					$data_number = $data_number + 1;
				}
				// Obtiene los campos de la consulta de salida <-> output
				for ($data_field=0; $data_field < mysql_num_fields($this->sys_datahandle); $data_field++)
					$data_serialize["fields"][$data_field] = mysql_field_name($this->sys_datahandle,$data_field);
				// Obtiene el par�metro ["count"]
				$data_serialize["count"] = mysql_num_rows($this->sys_datahandle);

				mysql_free_result($this->sys_datahandle);
			}
		}
	}

	function create_object ($i_form) { return "string"; }
	function delete_object ($i_form) { return "string"; }
	function update_object ($i_form) { return "string"; }
	function list_object   ($i_form) { return "string"; }
	function view_object   ($i_form) { return "string"; }

	function bbdd_table_create  ($table, $def_field) {

		$sql = "CREATE TABLE ".$table." ( ";
		$start_comma = FALSE;
		foreach ($def_field["COLUMNS"] as $key => $value) {
			if ($start_comma) $sql .= ", ";
			else $start_comma = TRUE;
			// El format enum es sustitueix per el que defineixi ENUM_TYPE.
			if ($value["TYPE"] == "enum") 
				$value["TYPE"] = $value["ENUM_TYPE"];
			
			$sql .= $value["NAME"]." ".$value["TYPE"];
			if (isset($value["CARDINAL"])) 
				$sql .= "(".$value["CARDINAL"].")";
			if (isset($value["NULL"]))
				if ($value["NULL"] == "no")
					$sql .= " NOT NULL";
			if (isset($value["DEFAULT"]))
				$sql .= " DEFAULT '".$value["DEFAULT"]."'";
			if (isset($def_field["AUTOINCREMENT"]))
				if ($key == $def_field["AUTOINCREMENT"])
					$sql .= " AUTO_INCREMENT";
		}
		if (isset($def_field["PKEY"])) {
			if ($def_field["PKEY"] != "") {
				$sql .= ", PRIMARY KEY (".$def_field["PKEY"].")";
			}
		}
		if (isset($def_field["IKEY"])) {
			if ($def_field["IKEY"] != "") {
				$sql .= ", INDEX (".$def_field["IKEY"].")";
			}
		}
		if (isset($def_field["UKEY"])) {
			if ($def_field["UKEY"] != "") {
				$sql .= ", UNIQUE (".$def_field["UKEY"].")";
			}
		}
		$sql .= " );";
		if (count($def_field) > 0) {
//			echo $sql."\n";
			$qhandle = @mysql_query($sql) or $this->db_mysql_die("BBDD Create ".$table." Sentence: ".$sql." failed");
		}
	}

	function alter_create  ($table, $def_field) {
//		print_r($def_field);
		if ($def_field["TYPE"] == "enum")
			$def_field["TYPE"] = $def_field["ENUM_TYPE"];
		$sql = "ALTER TABLE ".$table." ADD ".$def_field["NAME"]." ".$def_field["TYPE"];
		if (isset($def_field["CARDINAL"])) 
			$sql .= "(".$def_field["CARDINAL"].")";
		if (isset($def_field["NULL"]))
			if ($def_field["NULL"] == "no")
				$sql .= " NOT NULL";
		if (isset($def_field["DEFAULT"]))
			$sql .= " DEFAULT '".$def_field["DEFAULT"]."'";
		$qhandle = @mysql_query($sql.";") or $this->db_mysql_die("Sentence: ".$sql." failed");
	}

	function alter_change  ($table, $def_field) {
		if ($def_field["TYPE"] == "enum")
			$def_field["TYPE"] = $def_field["ENUM_TYPE"];
		$sql = "ALTER TABLE ".$table." CHANGE ".$def_field["NAME"]." ".$def_field["NAME"]." ".$def_field["TYPE"];
		if (isset($def_field["CARDINAL"])) 
			$sql .= "(".$def_field["CARDINAL"].")";
		if (isset($def_field["NULL"]))
			if ($def_field["NULL"] == "no")
				$sql .= " NOT NULL";
		if (isset($def_field["DEFAULT"]))
			$sql .= " DEFAULT '".$def_field["DEFAULT"]."'";
		if (isset($def_field["EXTRA"]))
			$sql .= " ".$def_field["EXTRA"];
		$qhandle = @mysql_query($sql.";") or $this->db_mysql_die("Sentence: ".$sql." failed");
	}

	function alter_delete ($table, $def_field) {
		
	}

	protected function db_query($qstring) {
		$this->sys_datahandle = @mysql_query($this->sys_dbname,$qstring) or $this->db_mysql_die("db_query q_string=".$qstring);
		return $this->sys_datahandle;
	}

	protected function db_numrows() {
		// return only if qhandle exists, otherwise 0
		$qhandle = $this->sys_datahandle;
		if ($qhandle) {
			return @mysql_numrows($qhandle) or $this->db_mysql_die("db_numrows: Error");
		} else {
			return 0;
		}
	}

	protected function db_result($row,$field) {
		$qhandle = $this->sys_datahandle;
		return @mysql_result($qhandle,$row,$field) or $this->db_mysql_die("db_result: Error");
	}

	protected function db_numfields($lhandle) {
		return @mysql_numfields($lhandle) or $this->db_mysql_die("db_numfields: Error");
	}

	protected function db_fieldname($lhandle,$fnumber) {
		return @mysql_fieldname($lhandle,$fnumber) or $this->db_mysql_die("db_fieldname: Error");
	}

	protected function db_affected_rows() {
		return @mysql_affected_rows() or $this->db_mysql_die("db_affected_rows: Error");
	}

	protected function db_fetch_array($qstring) {
		if ($this->sys_datahandle) {
			return @mysql_fetch_array($qhandle) or $this->db_mysql_die("db_fetch_array->db_die: Error");
		}
	}

	protected function db_insertid() {
		return @mysql_insert_id() or $this->db_mysql_die("db_insertid: Error");
	}

	function db_mysql_die($strMySQLError) {
		$this->object_error(mysql_error());
		exit;
	}

	function _database_object(){
		mysql_close($this->db_handle);
	}

}

if (!isset($db_connhandle))
	$db_connhandle = @mysql_connect($db_host, $db_user, $db_passwd) or database_object::db_mysql_die("connect_server_database");;
if (!isset($db_syshandle))
	$db_syshandle = @mysql_select_db($database_name) or database_object::db_mysql_die("select_database");

?>
