<?php
	session_start(); //starting session
	$error=''; // variable for error mssg

	if(isset($_POST['username'])) {
		if(empty($_POST['username']) || empty($_POST['password'])) {
			$error = "Give a username and password. . .";
		} else {
			$username=$_POST['username'];
			$password=$_POST['password'];

			require "db_selector.php";

			if(queryUser($username) && queryPass($username, $password)) {
					$_SESSION['username'] = $username;
					$_SESSION['room_number'] = queryRoom($username);
					header("Location: /?p=Home");
			} else {
				$error = "Username or password is incorrect!";
			}
		}
		if(!empty($error)) {
			header("Location: Login.php?p=Login&err={$error}");
		}
	}


?>


<?php
	require "header.php";
?>


<div class="main">
	<form action="Login.php?p=Login" method="post" class="center loginform">
		<label class="label">Username:</label></br></br>
		<input class="textbox" type="text" name="username"></br></br>
		<label class="label">Password:</label></br></br>
		<input class="textbox" type="password" name="password"></br></br>
		<?php
			if(isset($_GET['err'])) {
				echo "<label class=\"label\">" . $_GET['err'] . "</label></br>";
			}
		?>
		<input class="button" type="submit" value="submit">
	</form>
</div>

<?php
	include "footer.php";
?>
