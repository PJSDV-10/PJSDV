<?php
	session_start();
	if(!isset($_SESSION['username'])) {
		header("Location: Login.php?p=Login");
		die;
	} else {
		if(isset($_GET['p'])) {
			$page = $_GET['p'];
		} else {
			$page = "Status";
		}
	}

	require "header.php";
	include "pages/{$page}.php";
	require "footer.php";
?>
