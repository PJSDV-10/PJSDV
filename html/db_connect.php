<?php
// basisgegevens
$dbhost = "localhost";
$dbuser = 'PJSDV';
$dbpass = 'gaatjeniksaan';
$dbname = "PJSDV";

// initialiseer de database verbinding
$conn = mysqli_connect($dbhost, $dbuser, $dbpass, $dbname);

//indien de connectie niet geinitialiseerd is
if(!$conn) {
    die('Could not connect: ' . mysqli_connect_error());
}
?>
