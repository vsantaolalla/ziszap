<?php

class xml_file_object {
	var $classname = "xml_file_object";
	var $arrOutput = array();
	var $resParser;
	var $strXmlData;
	var $filename;

	function xml_file_object ($name_file) {
		$this->filename = $name_file;
	}

	function parse_xml_array() {
		require_once("data_object/od.file.inc");
		$conditions_file = new file_object($this->filename);
		$strInputXML = $conditions_file->file_get_contents();

		$this->resParser = xml_parser_create ();
		xml_set_object($this->resParser,$this);
		xml_set_element_handler($this->resParser, "tagOpen_xml", "tagClosed_xml");

		xml_set_character_data_handler($this->resParser, "tagData");

		$this->strXmlData = xml_parse($this->resParser, $strInputXML );
		if(!$this->strXmlData) {
			$this->xml_die(sprintf("XML error: %s at line %d", xml_error_string(xml_get_error_code($this->resParser)),
					xml_get_current_line_number($this->resParser)));
		}

		xml_parser_free($this->resParser);
		// TODO: Poner la funci�n parse_all con lo mismo pero retornando $this->arrOutput
		return $this->arrOutput[0];
	}

	function parse_xml_file() {
		require_once("data_object/od.file.inc");
		$conditions_file = new file_object($this->filename);
		$strInputXML = $conditions_file->file_get_contents();

		$this->resParser = xml_parser_create ();
		xml_set_object($this->resParser,$this);
		xml_set_element_handler($this->resParser, "tagOpen", "tagClosed");

		xml_set_character_data_handler($this->resParser, "tagData");

		$this->strXmlData = xml_parse($this->resParser, $strInputXML );
		if(!$this->strXmlData) {
			$this->xml_die(sprintf("XML error: %s at line %d", xml_error_string(xml_get_error_code($this->resParser)),
				xml_get_current_line_number($this->resParser)));
		}

		xml_parser_free($this->resParser);
		// TODO: Poner la funci�n parse_all con lo mismo pero retornando $this->arrOutput
		return $this->arrOutput[0];
	}

	function serialize($option_serialize, $params_serialize, &$data_serialize){
		// Guardar los elementos del array al fichero.
		if ($option_serialize == "input"){
		}
		// Recoger los valores del fichero al array
		if ($option_serialize == "output"){
			$data_serialize = $this->parse_xml_file();
		}
		if ($option_serialize == "xml_array"){
			$data_serialize = $this->parse_xml_array();
		}
	}

	function tagOpen_xml($parser, $name, $attrs) {
		// NAME: Nombre del objecto XML
		// ATTRS: Atributos del objecto XML
		$tag=array("name"=>$name,"attr"=>$attrs);
		array_push($this->arrOutput,$tag);
	}

	function tagOpen($parser, $name, $attrs) {
		// NAME: Nombre del objecto XML
		// ATTRS: Atributos del objecto XML
		$attrs[0] = $attrs;
		$tag=array("name"=>$name,"attr"=>$attrs);
		array_push($this->arrOutput,$tag);
	}

	function tagData($parser, $tagData) {
		if(trim($tagData)) {
			if(isset($this->arrOutput[count($this->arrOutput)-1]['data'])) {
				$this->arrOutput[count($this->arrOutput)-1]['data'] .= $tagData;
			} else {
				$this->arrOutput[count($this->arrOutput)-1]['data'] = $tagData;
			}
		}
	}

	function tagClosed_xml($parser, $name) {
		if (isset($this->arrOutput[count($this->arrOutput)-1]["attr"]["NAME"]))
			$this->arrOutput[count($this->arrOutput)-2][$name][$this->arrOutput[count($this->arrOutput)-1]["attr"]["NAME"]] = $this->arrOutput[count($this->arrOutput)-1];
		else
			$this->arrOutput[count($this->arrOutput)-2][$name][] = $this->arrOutput[count($this->arrOutput)-1];
		// Eliminamos el count para que no lo cuente, en el count general.
		// ??? unset($this->arrOutput[count($this->arrOutput)-2][$name]["count"]);
		// ??? $this->arrOutput[count($this->arrOutput)-2][$name]["count"] = count($this->arrOutput[count($this->arrOutput)-2][$name]);
		array_pop($this->arrOutput);
	}

	function tagClosed($parser, $name) {
		$this->arrOutput[count($this->arrOutput)-2][$name][] = $this->arrOutput[count($this->arrOutput)-1];
		// Eliminamos el count para que no lo cuente, en el count general.
		// ??? unset($this->arrOutput[count($this->arrOutput)-2][$name]["count"]);
		// ??? $this->arrOutput[count($this->arrOutput)-2][$name]["count"] = count($this->arrOutput[count($this->arrOutput)-2][$name]);
		array_pop($this->arrOutput);
	}

	function xml_die($strFILEError)
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