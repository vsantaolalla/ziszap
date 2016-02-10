<html><title>session test</title>

<?php 
	require_once "zz-content/header.form.php";
	include "zz-object/session/init.php";
	require_once "data_object/o.validate.form.inc";

	$data_in = $_GET;
	$data_out = Array();

	if (isset($data_in["func"])) {
		$T = new session();
		if ($data_in["func"] == "register_session") {
			$T->register_session($data_in, $data_out);
		}
		if ($data_in["func"] == "is_registered_session") {
			$T->is_registered_session($data_in, $data_out);
		}
		if ($data_in["func"] == "unregister_session") {
			$T->unregister_session($data_in, $data_out);
		}
?>
	<input type="hidden" name="func" value="<?php echo $data_in["func"]; ?>" />

<?php
	}
	require_once "zz-content/footer.form.php";
?>
	<a href="?func=register_session">register_session</a><br>
	<a href="?func=is_registered_session">is_registered_session</a><br>
	<a href="?func=unregister_session">unregister_session</a><br>

</html>
