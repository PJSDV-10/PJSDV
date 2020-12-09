<?php
	session_start(); //starting session
	$error=''; // variable for error mssg
	
	if(isset($_POST['submit'])) {
		if(empty($_POST['username']) || empty($_POST['password'])) {
			$error = "Give a username and password!";
		} else {
			$username=$_POST['username'];
			$password=$_POST['password'];

			if($username == "admin" && $password == "admin") {
				$_SESSION['login_user'] = $username;
				header("?p=home");
			} else {
				$error = "Username or password is incorrect!";
			}
		}
	}

	include("loginForm.php");
?>
