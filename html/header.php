<?php $page = $_GET['p']; ?>
<!DOCTYPE html>
<html>
	<head>
		<title><?php echo $page; ?></title>

		<link rel="stylesheet" type="text/css" href="css/global.css">
		<link rel="stylesheet" type="text/css" href="css/<?php echo $page; ?>.css">
	</head>

	<body>
		<div class="header">
			<div class="box filler">
				<?php
				if($page != "Login") {
					echo "<p class=\"info\">Name: ".$_SESSION['login_user']."<br>";
					echo "Room: "; //.$_SESSION['room'];
					echo "</p>";
				}
				?>
			</div>
			<div class="box">
				<h1 class="title"><?php echo $page; ?></h1>
			</div>
			<div class="box">

				<input class="button right" type="submit" value="Log out"
					onclick="window.location.href='logout.php'">
			</div>
		</div>
