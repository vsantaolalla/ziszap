<?php

//
// Ziszap Portal System Objects.
//
// Sistema de objetos genericos de database.
// Llamadas gen�ricas de base de datos, de forma que se llama al objeto gen�rico
// de cada base de datos de data_object
//

require_once("data_object/o.form.php");

abstract class generical_object_database extends object_form {
//	var $classname = "generical_object_database";

	function generical_object_database () {
		parent::object_form ();
	}

	abstract function create_object ($i_form);
	abstract function delete_object ($i_form);
	abstract function update_object ($i_form);
	abstract function list_object   ($i_form);
	abstract function view_object   ($i_form);
	abstract function explain_table ($n_table);
	abstract function alter_create  ($table, $def_field);
	abstract function alter_change  ($table, $def_field);
	abstract function list_db_tables ();
	abstract function bbdd_table_create  ($table, $def_field);
	abstract function db_synchronize_keys_tables ($table_xml_def);

	public function table_struct ($name_table) {
		// PASO INICIAL: Comprobamos que existe la definicion de $name_table
		$list_tables = $this->list_tables();
//		print_r($list_tables);
		$is_table_exist_xml = false;
		foreach ($list_tables as $key => $db_xml) {
			if ($db_xml["NAME"] == $name_table) {
				$is_table_exist_xml = true;
			}
		}
		// sino existe la tabla salimos dando error de semantica: No existe definicion de la BD.
		if (! $is_table_exist_xml) {
			$this->db_error("table_struct: Database definition [$name_table] don't exist in XML.");
		}
		
		// PRIMER PASO: Comprobamos que exista las tablas en la base de datos
		$l_db_tables = $this->list_db_tables();
		$is_table_exist = false;

		foreach ($l_db_tables as $i_key => $db_table) {
			if ($db_table == $name_table)
				$is_table_exist = true;
		}
//		print_r($l_db_tables);
		// SEGUNDO PASO: En caso de existir la tabla comprobar los atributos, tipados y cardinalidades
		$table_xml_def = $this->get_table($name_table);
		$columns_xml_def = $table_xml_def["COLUMNS"];

		if (! $is_table_exist){
			// TODO: Create_table
//			print_r($table_xml_def);
			$this->bbdd_table_create($name_table, $table_xml_def);
		} else {
			// TODO: Comparamos los campos y verificamos que esten sincronizados.
			$table_db_def = $this->explain_table($name_table);
//			print_r($columns_xml_def);
//			print_r($table_db_def);
			// Comprobamos que los campos de XML estan en la BBDD, al reves (eliminar) no haremos nada.
			foreach ($columns_xml_def as $key_xml => $value_xml) {
				$key_exist = FALSE;
				foreach ($table_db_def as $key_db => $value_db) {
					if ($key_xml == $key_db) {
						$key_exist = TRUE;
						$execute_alter_change = FALSE;
						if (
							(isset($value_db["TYPE"])) && 
							(isset($value_xml["TYPE"])) &&
							($value_db["TYPE"] == $value_xml["TYPE"]) 
						) {
							if (isset($value_xml["CARDINAL"])) {
								if ($value_db["CARDINAL"] != $value_xml["CARDINAL"]) {
									$execute_alter_change = TRUE;
								}
							}
							if (isset($value_xml["NULL"])) {
								if ($value_db["NULL"] != $value_xml["NULL"]) {
									$execute_alter_change = TRUE;
								}
							}
							if (isset($value_xml["DEFAULT"])) {
								if ($value_db["DEFAULT"] != $value_xml["DEFAULT"]) {
									$execute_alter_change = TRUE;
								}
							}
							if ($execute_alter_change == TRUE) {
								$this->alter_change($name_table, $value_xml);
							}
						}
						else {
							$this->alter_change($name_table, $value_xml);
						}
					}
				}
				if (! $key_exist) {
					// Creamos la columna
					$this->alter_create($name_table, $value_xml);
				}
			}
//			print_r($list_xml_primary_keys);
			$this->db_synchronize_keys_tables($table_xml_def);
		}
	}

	function db_error($strFILEError)
	{
		echo "<br><br>";
		$func_backtrace = debug_backtrace();
		echo "<table border='0' cellpadding='1' cellspacing='1' width='100%'><tr><td bgcolor='#514537'>";
		echo "<table border='0' cellpadding='5' cellspacing='0' width='100%'>";
		echo "<tr bgcolor='#EEEEEE'>";
		echo "<td width='20' valign='top'><img src='rcrs/gif/error.gif' hspace='3'></td>";
		echo "<td valign='middle'>";
		echo "<font face='Verdana,Arial' size=2><i>&lt;/data_object/o.database.php&gt;</i> <b>$strFILEError</b></font><br><br>";
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
