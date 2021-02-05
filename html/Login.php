<?php
	session_start(); //starting session
	$error=''; // variable for error mssg

	if(isset($_POST['username'])) { //trigger start login
		if(empty($_POST['username']) || empty($_POST['password'])) {
			// geval van een leeg veld:
			$error = "Give a username and password. . .";
		} else {
			// Geen lege velden, dus waarden velden opslaan:
			$username=$_POST['username'];
			$password=$_POST['password'];

			require "db_selector.php";	//DB functies invoegen of error poepen.

			if(queryUser($username) && queryPass($username, $password)) {
				// indien wachtwoord en gebruikersnaam goed zijn:
				$_SESSION['username'] = $username;
				$_SESSION['room_number'] = queryRoom($username);
				header("Location: /?p=Status"); // stuur door naar Status
			} else {
				// indien ww en/of usr fout is.
				$error = "Username or password is incorrect!";
			}
		}
		if(!empty($error)) {
			// redirect richting zelfde bladzijde met een vooraf ingestelde foutmelding.
			header("Location: Login.php?p=Login&err={$error}");
		}
	}


?>


<?php
	require "header.php"; // voeg header toe of geef fout.
	//<html code login form>
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
	//</html code login form>
	include "footer.php"; // voeg footer toe indien deze bestaat.
?>
