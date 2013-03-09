<html><title>authdb test</title>

<?php 
	require_once "zz-content/header.form.php";
	include "zz-object/authdb/init.php";
	require_once "data_object/o.validate.form.inc";

	$data_in = $_GET;
	$data_out = Array();

	if (isset($data_in["func"])) {
		$T = new authdb();
		if ($data_in["func"] == "auth_user") {
			$T->auth_user($data_in, $data_out);
		}
		if ($data_in["func"] == "lost_pwd_user") {
			$T->lost_pwd_user($data_in, $data_out);
		}
		if ($data_in["func"] == "new_user") {
			$T->new_user($data_in, $data_out);
		}
		if ($data_in["func"] == "logout") {
			$T->logout($data_in, $data_out);
		}
?>
	<input type="hidden" name="func" value="<?php echo $data_in["func"]; ?>" />

<?php
	}
	require_once "zz-content/footer.form.php";
?>
	<a href="?func=auth_user">auth_user</a><br>
	<a href="?func=lost_pwd_user">lost_pwd_user</a><br>
	<a href="?func=new_user">new_user</a><br>
	<a href="?func=logout">logout</a><br>

</html>
