<?php
//
// Nynpha Multimedia Portal System Databases.
//
// Sistema de base de datos API para el acceso a través del portal de todos los accesos
// a traves de la base de datos de MySQL.
//
// Propiedad de Mankind Capital, S.L -- Todos los derechos reservados
// Nynpha Multimedia y sus respectivos logos son marcas registradas de Mankind Capital SL
//

$sys_dbhost = "localhost";
$sys_dbuser = "root";
$sys_dbpasswd = "";
$sys_dbname = "source";

function db_connect() {
	global $sys_dbhost,$sys_dbuser,$sys_dbpasswd,$conn;
	$conn = @mysql_pconnect($sys_dbhost,$sys_dbuser,$sys_dbpasswd) or db_mysql_die();
	#return $conn;
}

function db_query($qstring,$print=0) {
	if ($print) print "<br>Query is: $qstring<br>";
	global $sys_dbname;
	$GLOBALS['db_qhandle'] = @mysql($sys_dbname,$qstring);
	return $GLOBALS['db_qhandle'];
}

function db_numrows($qhandle) {
	// return only if qhandle exists, otherwise 0
	if ($qhandle) {
		return @mysql_numrows($qhandle);
	} else {
		return 0;
	}
}

function db_result($qhandle,$row,$field) {
	return @mysql_result($qhandle,$row,$field);
}

function db_numfields($lhandle) {
	return @mysql_numfields($lhandle);
}

function db_fieldname($lhandle,$fnumber) {
           return @mysql_fieldname($lhandle,$fnumber);
}

function db_affected_rows($qhandle) {
	return @mysql_affected_rows();
}

function db_fetch_array($qhandle = 0) {
	if ($qhandle) {
		return @mysql_fetch_array($qhandle);
	} else {
		if ($GLOBALS['db_qhandle']) {
			return @mysql_fetch_array($GLOBALS['db_qhandle']);
		} else {
			return (array());
		}
	}
}

function db_insertid($qhandle) {
	return @mysql_insert_id($qhandle);
}

function db_error() {
	return @mysql_error();
}

function db_mysql_die()
{
	global $strMySQLError;

	echo "<br><br>";
	echo "<table border='0' cellpadding='1' cellspacing='1' width='100%'><tr><td bgcolor='#514537'>";
		echo "<table border='0' cellpadding='5' cellspacing='0' width='100%'>";
		echo "<tr bgcolor='#EEEEEE'>";
		echo "<td width='20' valign='top'><img src='rcrs/gif/error.gif' hspace='3'></td>";
		echo "<td valign='top'>";
		echo "<b>$strMySQLError</b><br>";
		echo db_error()."<br><br>";
		echo "</td>";
		echo "</tr></table>";
	echo "</td></tr></table>";
	echo "<br><br>";

	// die
	exit;
}

?>
