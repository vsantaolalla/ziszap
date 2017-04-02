<?php
require_once "zz-object/authdb/class.php";

class authdb extends z_authdb {
        var $classname = "authdb";

        /**
         *
         * Funcion: login ... Conectar el usuario al sistema
         * @input       varchar usulgn  Nombre del usuario
         * @input       varchar usupwd  Password del usuario
         *
        */
        function login ( $data_in, &$data_out ) {

                // TODO: Funcion login
                return TRUE;
        }

        /**
         *
         * Funcion: lostpwd ... Recuperar password del usuario
         * @input       varchar usulgn  Nombre del usuario
         * @input       varchar usuema  E-Mail del usuario registrado
         *
        */
        function lostpwd ( $data_in, &$data_out ) {

                // TODO: Funcion lostpwd
                return TRUE;
        }

        /**
         *
         * Funcion: create_user ... Crear usuario
         * @input       varchar usulgn  Nombre del usuario
         * @input       varchar usupwd  Password del usuario
         * @input       varchar usuema  E-Mail del usuario registrado
         * @output      varchar usulgn  Nombre del usuario
         * @output      varchar usupwd  Password del usuario
         * @output      varchar usuema  E-Mail del usuario registrado
         *
        */
        function create_user ( $data_in, &$data_out ) {

                // TODO: Funcion create_user
                $data_out = $data_in;
                $data_out["usupwd"] = md5($data_in["usupwd"]);
                return TRUE;
        }

        /**
         *
         * Funcion: not_find_user_to_group ... Eliminar usuario
         * @input       varchar usulgn  Nombre del usuario
         *
        */
        function not_find_user_to_group ( $data_in, &$data_out ) {

                // TODO: Funcion not_find_user_to_group
                return TRUE;
        }

        /**
        *
         * Funcion: add_member_to_list ... Añadir usuario al grupo
         * @input       int     grpid   ID del grupo
         * @input       varchar usulgn  Nombre del usuario
         * @input       varchar grpnam  Nombre del grupo
         * @input       text    grpmem  Miembros del grupo
         * @output      int     grpid   ID del grupo
         * @output      varchar grpnam  Nombre del grupo
         * @output      text    grpmem  Miembros del grupo
         *
        */
        function add_member_to_list ( $data_in, &$data_out ) {
        
                $key_users = Array();
                $data_temp = Array();
                $data_temp = $this->first_item();
                // Si no tiene miembros el grpmem es null, asignamos a un array vacio.
                if ((! isset($data_temp["grpmem"])) || ($data_temp["grpmem"] == ""))  {
                        $data_temp["grpmem"] = "";
                } else {
                        $list_users = explode(",", $data_temp["grpmem"]);
                        foreach ($list_users as $key => $value) {
                                $key_users[$value] = 1;
                        }
                }
                $key_users[$data_in["usulgn"]] = 1;
        
                $data_out = $data_temp;
                $data_out["grpmem"] = implode(",", array_keys($key_users));
        
                return TRUE;
        }
        
        /**
         *
         * Funcion: del_member_to_list ... Eliminar usuario al grupo
         * @input       int     grpid   ID del grupo
         * @input       varchar usulgn  Nombre del usuario
         * @input       varchar grpnam  Nombre del grupo
         * @input       text    grpmem  Miembros del grupo
         * @output      int     grpid   ID del grupo
         * @output      varchar grpnam  Nombre del grupo
         * @output      text    grpmem  Miembros del grupo
         *
        */
        function del_member_to_list ( $data_in, &$data_out ) {
        
                $key_users = Array();
                $data_temp = Array();
                $data_temp = $this->first_item();
                // Si no tiene miembros el grpmem es null, asignamos a un array vacio.
                if ((! isset($data_temp["grpmem"])) || ($data_temp["grpmem"] == ""))  {
                        return FALSE;
                } else {
                        $list_users = explode(",", $data_temp["grpmem"]);
                        foreach ($list_users as $key => $value) {
                                $key_users[$value] = 1;
                        }
                }
                if (! isset($key_users[$data_in["usulgn"]])) {
                        return FALSE;
                }
                unset($key_users[$data_in["usulgn"]]);

                $data_out = $data_temp;
                $data_out["grpmem"] = implode(",", array_keys($key_users));

                return TRUE;
        }

        /**
         *
         * Funcion: is_member_to_list ... Verificar si el usuario pertenece al grupo
         * @input       varchar usulgn  Nombre del usuario
         * @input       varchar grpnam  Nombre del grupo
         * @input       text    grpmem  Miembros del grupo
         *
        */
        function is_member_to_list ( $data_in, &$data_out ) {

                // TODO: Funcion is_member_to_list
                $key_users = Array();
                $data_temp = Array();
                $data_temp = $this->first_item();
                // Si no tiene miembros el grpmem es null, asignamos a un array vacio.
                if ((! isset($data_temp["grpmem"])) || ($data_temp["grpmem"] == ""))  {
                        return FALSE;
                } else {
                        $list_users = explode(",", $data_temp["grpmem"]);
                        foreach ($list_users as $key => $value) {
                                $key_users[$value] = 1;
                        }
                }
                if (! isset($key_users[$data_in["usulgn"]])) {
                        return FALSE;
                } else {
                        return TRUE;
                }
        }

        /**
         *
         * Funcion: find_member_to_list ... Encontrar un usuario al grupo
         * @input       varchar usulgn  Nombre del usuario
         * @input       varchar grpnam  Nombre del grupo
         * @input       text    grpmem  Miembros del grupo
         *
        */
        function find_member_to_list ( $data_in, &$data_out ) {

                // TODO: Funcion find_member_to_list
                return TRUE;
        }

        /**
         *
         * Funcion: logout ... Desconetar el usuario
         *
        */
        function logout ( $data_in, &$data_out ) {

                // TODO: Funcion logout
                return TRUE;
        }

}
?>
