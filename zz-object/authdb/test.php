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
		if ($data_in["func"] == "delete_user") {
			$T->delete_user($data_in, $data_out);
		}
		if ($data_in["func"] == "new_group") {
			$T->new_group($data_in, $data_out);
		}
		if ($data_in["func"] == "add_user_to_group") {
			$T->add_user_to_group($data_in, $data_out);
		}
		if ($data_in["func"] == "del_user_to_group") {
			$T->del_user_to_group($data_in, $data_out);
		}
		if ($data_in["func"] == "is_user_to_group") {
			$T->is_user_to_group($data_in, $data_out);
		}
		if ($data_in["func"] == "find_user_to_group") {
			$T->find_user_to_group($data_in, $data_out);
		}
		if ($data_in["func"] == "delete_group") {
			$T->delete_group($data_in, $data_out);
		}
		if ($data_in["func"] == "off_user") {
			$T->off_user($data_in, $data_out);
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
	<a href="?func=delete_user">delete_user</a><br>
	<a href="?func=new_group">new_group</a><br>
	<a href="?func=add_user_to_group">add_user_to_group</a><br>
	<a href="?func=del_user_to_group">del_user_to_group</a><br>
	<a href="?func=is_user_to_group">is_user_to_group</a><br>
	<a href="?func=find_user_to_group">find_user_to_group</a><br>
	<a href="?func=delete_group">delete_group</a><br>
	<a href="?func=off_user">off_user</a><br>

</html>
