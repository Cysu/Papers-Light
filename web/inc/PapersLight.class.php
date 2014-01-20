<?php

require_once('inc/DBConn.class.php');
require_once('inc/DBError.class.php');

class PapersLight {
    private $_type2attr;
    private $_tables;

    public function __construct($type2attr) {
        $this->_type2attr = $type2attr;
    }

    public function getPapers() {
        try {
            $db = $this->getDatabase();
            $papers = [];
            foreach (array_keys($this->_type2attr )as $type) {
                $result = $db->query('SELECT * FROM pl_'.$type);
                foreach ($result as $paper) {
                    array_push($papers, $this->getPaperInfo($paper));
                }
            }
            return array_slice($papers, 2, 1);
        } catch (DBError $e) {
            return ['error' => 'database-error'];
        }
    }

    private function getPaperInfo($paper) {
        $year = isset($paper['year']) ? $paper['year'] : 'Unknown';

        $title = isset($paper['title']) ? $paper['title'] : 'Unknown';

        $author = isset($paper['author']) ? $paper['author'] : (
                  isset($paper['editor']) ? $paper['editor'] : 'Unknown');

        $source = isset($paper['booktitle']) ? $paper['booktitle'] : (
                  isset($paper['journal']) ? $paper['journal'] : (
                  isset($paper['publisher']) ? $paper['publisher']: 'Unknown'));

        return ['year' => $year,'title' => $title, 'author' => $author, 'source' => $source];
    }

    private function getDatabase() {
        return new DBConn('mysql', 'papers_light', 'pl_admin', 'cuhk_mmlab');
    }
}
