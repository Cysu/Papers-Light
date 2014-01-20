<?php

require_once('inc/DBConn.class.php');
require_once('inc/DBError.class.php');

class PapersLight {

    public $user;

    private $_type2attr;

    public function __construct($type2attr) {
        $this->user = '';
        $this->_type2attr = $type2attr;

        if (isset($_COOKIE['plname']) && isset($_COOKIE['plpass'])) {
            $this->adminLogin($_COOKIE['plname'], $_COOKIE['plpass']);
        }
    }

    public function adminLogin($username, $password, $remember = true) {
        if ($username === 'pl_admin' && $password === 'cuhk_mmlab') {
            if ($remember) {
                setcookie('plname', 'pl_admin', time() + 90 * 86400, '/');
                setcookie('plpass', 'cuhk_mmlab', time() + 90 * 86400, '/');
            }
            $this->user = $username;
            $_SESSION['pl'] = serialize($this);
        } else {
            return ['error' => 'admin-login-failed'];
        }
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
