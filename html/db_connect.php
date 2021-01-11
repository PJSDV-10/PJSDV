<?php

$dbhost = "localhost";
$dbuser = 'PJSDV';
$dbpass = 'gaatjeniksaan';
$dbname = "PJSDV";
$conn = mysqli_connect($dbhost, $dbuser, $dbpass, $dbname);

if(!$conn) {
    die('Could not connect: ' . mysqli_connect_error());
}
echo "<!--DB_success-->";
?>
