<?php

require_once('inc/DBConn.class.php');
require_once('inc/DBError.class.php');

class PapersLight {
    public function __construct() {
        
    }

    public function getPapers() {
        try {
            $db = $this->getDatabase();
            return ['test_key' => 'test_value'];
        } catch (DBError $e) {
            return ['error' => 'database-error'];
        }
    }

    private function getDatabase() {
        return new DBConn('mysql', 'papers_light', 'pl_admin', 'cuhk_mmlab');
    }
}
