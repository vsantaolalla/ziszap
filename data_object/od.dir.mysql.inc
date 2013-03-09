<?
class directory_object {

	var $classname = "directory_object";
	var $ldap_root = "cn=root,o=mk";
	var $ldap_passwd = "zip08970";
	var $ldap_base = "o=mk";
	var $ldap_host;
	var $ds;

	function directory_object($server_data){
		global $ds, $ldap_root, $ldap_passwd;
		$this->ldap_host = $server_data;
		$this->ds = ldap_connect($server_data);  // must be a valid LDAP server!
		$r = ldap_bind($this->ds, $this->ldap_root, $this->ldap_passwd);
		ldap_set_option($this->ds, LDAP_OPT_PROTOCOL_VERSION, 3);
	}

	function serialize($option_serialize, $params_serialize, &$data_serialize){
		if ($option_serialize == "output"){
			$sr = @ldap_search($this->ds, $params_serialize["serialize.dn"], $params_serialize["serialize.condition"]);
			$data_serialize = @ldap_get_entries($this->ds, $sr);
		}
	}

	function _directory_object(){
		ldap_close($this->ds);
	}
}
?>
