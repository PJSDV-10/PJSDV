<?php
	session_start(); // start sessie
	$page;
	//checkt of de sessie geinitialiseerd is.
	if(!isset($_SESSION['username'])) {
		// geen sessie is doorsturen naar login
		header("Location: Login.php?p=Login");
		die;
	} else {
		// een sessie waar page bekend is, haal deze op.
		if(isset($_GET['p'])) {
			$page = $_GET['p'];
		// indien page onbekend ga naar Status
		} else {
			$page = "Status";
		}
	}

	require "header.php";
	require "pages/{$page}.php";
	include "footer.php";
?>
