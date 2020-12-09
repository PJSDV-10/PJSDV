<?php
	if(!isset($_SESSION['login_user']) {
		header("?p=login");
	} else {
		$page = $_GET['p'];
		if(empty($_GET['p'])) {
			$page = "login"
		}

		include("header");
		include("{$page}.php");
		include("footer");
	}
?>
