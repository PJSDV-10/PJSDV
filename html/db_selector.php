<?php
$dbhost = "localhost";
$dbuser = 'PJSDV';
$dbpass = 'gaatjeniksaan';
$dbname = "PJSDV";

//connect
$conn = mysqli_connect($dbhost, $dbuser, $dbpass, $dbname);
//check connection
if(!$conn) {
    die('Could not connect: ' . mysqli_connect_error());
}


function queryUser($user) {
    global $conn;
    $sql = "SELECT COUNT(username) as num FROM users WHERE username = '$user';";
    $res = mysqli_query($conn, $sql);
    $row = mysqli_fetch_assoc($res);
    return $row["num"] == 1;
}

function queryPass($user, $pass) {
    global $conn;
    $sql = "SELECT password FROM users WHERE username = '$user';";
    $res = mysqli_query($conn, $sql);
    $row = mysqli_fetch_assoc($res);
    return $row["password"] == $pass;
}

function queryRoom($user) {
    global $conn;
    $sql = "SELECT room_number FROM users WHERE username = '$user';";
    $res = mysqli_query($conn, $sql);
    $row = mysqli_fetch_assoc($res);
    return $row["room_number"];
}
?>
