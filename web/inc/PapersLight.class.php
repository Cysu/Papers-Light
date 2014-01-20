<?php

require_once('inc/DBConn.class.php');
require_once('inc/DBError.class.php');

class PapersLight {
    private $_type2attr;
    private $_tables;

    public function __construct($type2attr) {
        $this->_type2attr = $type2attr;

        $prefix = function($item) {
            return 'pl_'.$item;
        };

        $this->_tables = join(',', array_map($prefix, array_keys($type2attr)));
    }

    public function getPapers() {
        try {
            $types = join(',', array_keys($this->_type2attr));

            $db = $this->getDatabase();
            $result = $db->query('SELECT * FROM '.$this->_tables);

            $papers = [];
            foreach ($result as $paper) {
                array_push($papers, ['year' => $paper['year'],
                                     'author' => $paper['author'],
                                     'title' => $paper['title'],
                                     'booktitle' => $paper['booktitle']]);
            }

            return $papers;
        } catch (DBError $e) {
            return ['error' => 'database-error'];
        }
    }

    private function getDatabase() {
        return new DBConn('mysql', 'papers_light', 'pl_admin', 'cuhk_mmlab');
    }
}
