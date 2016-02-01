<?php
//	require_once('zz-admin/config.php');

//	$link_referer = $_SERVER["HTTP_REFERER"];

	$cmd_in = Array();
	$cmd_out = Array();
	$method_call = "";

	if (isset($_POST) && count($_POST) > 0) {
		$cmd_in = $_POST;
	} else if (isset($_GET) && count($_GET) > 0) {
		$cmd_in = $_GET;
	}
	
	if (!isset($cmd_in["object"])) {
		header('HTTP/1.1 405 Method Not Allowed1');
		echo "error";
		die();
	}

	include "zz-object/".$cmd_in["object"]."/init.php";
	eval ("\$T = new ".$cmd_in["object"]."();");
	unset($cmd_in["object"]);

	if (!isset($cmd_in["method"])) {
		header('HTTP/1.1 405 Method Not Allowed2');
		echo "error";
		die();
	}
	$method_call = $cmd_in["method"];
	unset($cmd_in["method"]);

	eval ("\$result = \$T->".$method_call."(\$cmd_in, \$cmd_out);");
	if ($result) {
		if (! $cmd_out)
			header('HTTP/1.1 204 No Content');
		// Check if key is the enum field. if checked add "{key}.text" with text field.
		foreach (reset($cmd_out) as $key => $value) {
			// Si el campo es enum, para todos los valores del campo generamos el key.text 
			if ($T->get_fieldType($key) == "enum") {
				foreach ($cmd_out as $key_i => $value_i) {
					$cmd_out[$key_i][$key."_text"] = $T->view_value_fields($key, $value_i[$key]);
				}
			}
		}
		echo json_encode($cmd_out);
	} else {
		header('HTTP/1.1 405 Method Not Allowed');
		echo "error";
	}
?>
