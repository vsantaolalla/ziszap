<?php
//	require_once('zz-admin/config.php');

//	$link_referer = $_SERVER["HTTP_REFERER"];
try {
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
		if (! $cmd_out) {
			header('HTTP/1.1 204 No Content');
			echo "error";			
		}
		echo json_encode($cmd_out);
	} else {
		header('HTTP/1.1 405 Method Not Allowed');
		echo "error";
	}
}
catch (PDOException $e) {
	header('HTTP/1.1 520 Unknown Error');
	print 'Exception : ' . $e->getMessage();
	echo "error";
}
?>
