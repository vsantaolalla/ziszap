<?PHP

//
// Ziszap Portal System Objects.
//
// Sistema de objetos genericos de database.
// Llamadas gen�ricas de base de datos, de forma que se llama al objeto gen�rico
// de cada base de datos de data_object
//

require_once("data_object/ag.object.inc");

class object_form extends generical_object {
//	var $classname = "object_form";

	function object_form () {
		parent::generical_object ();
	}
	
	function create_form ($method) {
		$form_data = array ();	
		$list_fields = $this->list_fields();
		$input_fields = $this->list_input_methods($method);
		foreach ($input_fields as $key => $value) {
			$form_data_temp = explode(",", $value["FIELD"]);
			foreach ($form_data_temp as $key_temp => $value_temp) {
				$form_data[$value_temp] = $list_fields[$value_temp];
			}
		}
		return $form_data;
	}
	
	function create_html_input ($method, $vars) {
		if (! isset($vars)) return NULL;
		$form_fields = $this->create_form ($method);
		foreach ($form_fields as $key => $value) {
			if (! isset($vars[$key])) {
				echo $value["TEXT"].": ";
				if (($value["TYPE"] == "varchar") || ($value["TYPE"] == "int")) {
					echo '<input type="text" name="'.$value["NAME"].'" />';
				}
				echo "<br>";
			}
		}
	}
	
}
?>
