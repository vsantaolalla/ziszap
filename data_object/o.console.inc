<?php
$_console = FALSE;
if (isset($_SERVER["argc"])) {
    for ($i=1; $i < $_SERVER["argc"]; $i++) {
	$_console = TRUE;
	$i_array = Array();
	$i_array = explode("=", $_SERVER["argv"][$i]);
	$_GET[$i_array[0]] = $i_array[1];
    }
}

?>