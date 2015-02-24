<?
	require_once "data_object/db.mysql.inc";
	require_once "data_object/o.module.inc";
	require_once "zz-admin/config.php";

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

	$dg = new rr();
	foreach ($dg->list_tables() as $key => $value) {
		$dg->table_struct($value["NAME"]);
	}
	$m = new object_module();
	
	$p_input = Array();
	$p_output = Array();
	$m->create_class($_GET["class"]);
	$m->create_init($_GET["class"]);
	$m->create_test($_GET["class"]);
?>