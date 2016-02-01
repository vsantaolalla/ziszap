<?php
require_once "data_object/o.form.inc";

class ValidateForm extends object_form {
	public	$fieldsRequired = Array ();
	public	$fieldsSize = Array ();
	public	$fieldsValidate = Array ();
	
	protected function is_required ($field) {
		foreach ($this->fieldsRequired as $value) {
			if ($field == $value) {
				return TRUE;
			}
		}
		return FALSE;	
	}
	
	function create_form ($method) {
		$form_data = array ();	
		$list_fields = $this->list_fields();
		$input_fields = $this->list_input_method($method);
		return $input_fields;
	}
	
	function create_html_input ($listFields, $data_in = Array()) {
//		if (! isset($vars)) return NULL;
//		$form_fields = $this->create_form ($method);
//		print_r($listFields);
		foreach ($listFields as $key => $value) {
			if ((isset($this->fieldsValidate[$key])) && (! isset($data_in[$key]))) {
				echo "<p>\n";
				echo "<label for=\"c".$key."\">".$value."</label>\n";
				if ($this->fieldsValidate[$key] == "varchar") {
					echo '<input id="c'.$key.'" type="text" name="'.$key.'"';
				} else if ($this->fieldsValidate[$key] == "int") {
					echo '<input id="c'.$key.'" type="digits" name="'.$key.'"';
					
				} else if ($this->fieldsValidate[$key] == "date") {
					echo '<input id="c'.$key.'" type="date" name="'.$key.'"';
				} else if ($this->fieldsValidate[$key] == "enum") {
//					print_r($this->list_enum_fields());
				}
				if ($this->is_required($key)) echo ' class="required"';
				echo ' />'."\n";
				echo "</p>\n";
			}
		}
	}
		
}
?>