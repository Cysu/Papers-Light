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
            return ['success' => 'admin-login-success'];
        } else {
            return ['error' => 'admin-login-failed'];
        }
    }

    public function getTypes() {
        return $this->_type2attr;
    }

    public function getPapers() {
        try {
            $db = $this->getDatabase();
            $papers = [];
            foreach (array_keys($this->_type2attr) as $type) {
                $result = $db->query('SELECT * FROM pl_'.$type);
                foreach ($result as $paper) {
                    $paper['type'] = $type;
                    array_push($papers, $paper);
                }
            }
            return $papers;
        } catch (DBError $e) {
            return ['error' => 'database-error'];
        }
    }

    public function addPaper($type, $paper) {
        if ($this->user !== 'pl_admin') {
            return ['error' => 'No database access permission'];
        }

        try {
            $paper = (array) $paper;
            $columns = array_keys($paper);
            $values = array_map(function($column) {
                return ':'.$column;
            }, $columns);

            $qstr = 'INSERT INTO pl_'.$type;
            $qstr .= '('.join(',', $columns).')';
            $qstr .= ' VALUES('.join(',', $values).')';

            $params = [];
            for ($i = 0; $i < count($columns); ++$i) {
                array_push($params, [
                    $values[$i],
                    $paper[$columns[$i]],
                    PDO::PARAM_STR
                ]);
            }
            
            $db = $this->getDatabase();
            $db->query($qstr, $params);

            return ['success' => 'add-paper-success'];
        } catch (DBError $e) {
            return ['error' => 'database-error'];
        }
    }

    public function updatePaper($origType, $newType, $paperId, $paper) {
        if ($this->user !== 'pl_admin') {
            return ['error' => 'No database access permission'];
        }


        try {
            if ($origType === $newType) {
                $paper = (array) $paper;
                $columns = array_keys($paper);
                $values = array_map(function($column) {
                    return $column.' = :'.$column;
                }, $columns);

                $qstr = 'UPDATE pl_'.$newType;
                $qstr .= ' SET '.join(',', $values);
                $qstr .= ' WHERE paper_id = :paper_id';

                $params = [];
                for ($i = 0; $i < count($columns); ++$i) {
                    array_push($params, [
                        ':'.$columns[$i],
                        $paper[$columns[$i]],
                        PDO::PARAM_STR
                    ]);
                }
                array_push($params, [':paper_id', (int) $paperId, PDO::PARAM_INT]);

                $db = $this->getDatabase();
                $db->query($qstr, $params);

                return ['success' => 'update-paper-success'];
            } else {
                $result = $this->removePaper($origType, $paperId);
                if (isset($result['error'])) return $result;

                $result = $this->addPaper($newType, $paper);
                if (isset($result['error'])) return $result;

                return ['success' => 'update-paper-success'];
            }
        } catch (DBError $e) {
            return ['error' => 'database-error'];
        }
    }

    public function removePaper($type, $paperId) {
        if ($this->user !== 'pl_admin') {
            return ['error' => 'No database access permission'];
        }

        try {
            $qstr = 'DELETE FROM pl_'.$type.' WHERE paper_id = :paper_id';
            $params = [[':paper_id', (int) $paperId, PDO::PARAM_INT]];

            $db = $this->getDatabase();
            $db->query($qstr, $params);

            return ['success' => 'remove-paper-success'];
        } catch (DBError $e) {
            return ['error' => 'database-error'];
        }
    }

    private function getDatabase() {
        return new DBConn('mysql', 'papers_light', 'pl_admin', 'cuhk_mmlab');
    }
}
