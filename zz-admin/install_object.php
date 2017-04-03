<?php
	// We are located in the parent directory
	chdir('..');
	ini_set('include_path', getcwd());

	require_once "data_object/o.module.php";
	if (isset($_GET["project"])) {
		require_once "zz-admin/config.".$_GET["project"].".php";
	} else {
		require_once "zz-admin/config.php";
	}
	require_once "data_object/db.mysql.php";
	$path = $_SERVER["DOCUMENT_ROOT"];

	require_once "data_object/z.install.php";
?>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
	<title>Install object</title>
</head>

<body>
<?php
	$dg = new install_ziszap();
	foreach ($dg->list_tables() as $key => $value) {
		$dg->table_struct($value["NAME"]);
	}
	$m = new object_module();
	
/*	$p_input = Ainstall_objectay();
	$p_output = Ainstall_objectay();

	$path_object = $path."/zz-object/".$_GET["class"]."/thread/";
	if ((! is_dir($path_object)) && ($path_object != "")) {
		mkdir ($path_object);
	}

	$path_object = $path."/zz-object/".$_GET["class"]."/test/";
	if ((! is_dir($path_object)) && ($path_object != "")) {
		mkdir ($path_object);
	}

	$m->create_class($_GET["class"]);
	$m->create_init($_GET["class"]);
	$m->create_class($_GET["class"], true);
*/
	
?>
</body>
</html>