<?php

require_once('inc/DBError.class.php');

class DBConn {
    private $_db;

    public function __construct($driver, $dbname, $username, $password,
        $host='127.0.0.1', $charset='utf8'
    ) {
        try {
            $this->_db = new PDO(
                $driver.':host='.$host.';dbname='.$dbname.';charset='.$charset,
                $username, $password);
        } catch (PDOException $e) {
            echo $e->getMessage();
            throw new DBError('Could not connect to database');
        }
    }
}
