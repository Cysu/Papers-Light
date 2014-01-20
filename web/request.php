<?php

require_once('inc/common.php');
require_once 'inc/PapersLight.class.php';

session_start();
if (isset($_SESSION['pl'])) {
    $pl = unserialize($_SESSION['pl']);
} else {
    $pl = new PapersLight($type2attr);
}

if (isset($_GET['action'])) header('Content-Type: application-json');

if (isset($_GET['action'])) {
    if ($_GET['action'] === 'getpapers') {
        echo json_encode($pl->getPapers());
    }
}
