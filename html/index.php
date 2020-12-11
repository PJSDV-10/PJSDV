<?php
	if(!isset($_SESSION['login_user']) {
		header("Location: loginForm.php");
	} else {
		$page = "home";
		if(isset($_GET['p'])) {
			$page = $_GET['p'];
		}

		include "header.php";
		include "{$page}.php";
		include "footer.php";
	}
?>
