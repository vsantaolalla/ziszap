<?php
	require_once "data_object/o.module.inc";
	if (isset($_GET["project"])) {
		require_once "zz-admin/config.".$_GET["project"].".php";
	} else {
		require_once "zz-admin/config.php";
	}
	require_once "data_object/db.mysql.inc";
	$path = $_SERVER["DOCUMENT_ROOT"];

class rr extends database_object {
	public $classname = "alertes";
	function rr () {
		global $db_host;
		global $db_user;
		global $db_passwd;
		global $database_name;
		global $_GET;
		
		print_r($_GET);
		$this->classname = $_GET["class"];
		parent::database_object($database_name."@".$db_host,$db_user,$db_passwd);
	}
}
?>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
	<title>Install object <?php echo $_GET["class"]; ?></title>
</head>

<body>
<?php
	$dg = new rr();
	foreach ($dg->list_tables() as $key => $value) {
		$dg->table_struct($value["NAME"]);
	}
	$m = new object_module();
	
	$p_input = Array();
	$p_output = Array();

	$path_zzinterface = $path."/zz-interface/";
	if ((! is_dir($path_zzinterface)) && ($path_zzinterface != "")) {
		mkdir ($path_zzinterface);
	}
	
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
?>
</body>
</html>