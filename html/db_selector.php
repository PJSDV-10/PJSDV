<?php
// basisgegevens
$dbhost = "localhost";
$dbuser = 'PJSDV';
$dbpass = 'gaatjeniksaan';
$dbname = "PJSDV";

// initialiseer de database verbinding
$conn = mysqli_connect($dbhost, $dbuser, $dbpass, $dbname);
// indien de connectie niet geinitialiseerd is
if(!$conn) {
    die('Could not connect: ' . mysqli_connect_error());
}

/**
 * checkt of de gebruiker in de database bestaat
 * @param user de te controleren gebruikersnaam
 * @return bool resultaat van de controle of de naam er 1 keer in staat.
**/
function queryUser($user) {
    global $conn;
    $sql = "SELECT COUNT(username) as num FROM users WHERE username = '$user';";
    $res = mysqli_query($conn, $sql);
    $row = mysqli_fetch_assoc($res);
    return $row["num"] == 1;
}

/**
 * controleert of het wachtwoord bij de juiste gebruiker in de db staat.
 * @param user ingevoerde gebruikersnaam om te testen
 * @param pass ingevoerd wachtwoord om te testen
 * @return bool resultaat of het ww in de db staat bij de usrname
**/
function queryPass($user, $pass) {
    global $conn;
    $sql = "SELECT password FROM users WHERE username = '$user';";
    $res = mysqli_query($conn, $sql);
    $row = mysqli_fetch_assoc($res);
    return $row["password"] == $pass;
}

/**
 * Haalt het kamernummer voor de gebruiker uit de database
 * @param user gebruikersnaam voor wie het kamernummer gezocht moet worden
 * @return nummer van de kamer
**/
function queryRoom($user) {
    global $conn;
    $sql = "SELECT room_number FROM users WHERE username = '$user';";
    $res = mysqli_query($conn, $sql);
    $row = mysqli_fetch_assoc($res);
    return $row["room_number"];
}
?>
