<?php
	if(!isset($_SESSION['login_user']) {
		header("Location: ?p=login");
	} else {
		$page = $_GET['p'];
		if(empty($_GET['p'])) {
			$page = "login"
		}

		include("header");
		include("{$page}.php");
		if($page == "login") {
			include("loginForm.php");
		}
		include("footer");
	}
?>
