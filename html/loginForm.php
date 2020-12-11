<?php
	include "header";
?>

<div class="header"></div>
<div class="main">
	<form action="login.php" method="post">
		<label>Username:</label></br>
		<input type="text" name="username"></br>
		<label>Password:</label></br>
		<input type="password" name="password"></br>
		<?php
			if(!(isset($_GET['err']))) {
				echo "<label>" . $_GET['err'] . "</label>";
			}
		?>
		</br>
		<input type="submit" value="submit">
	</form>
</div>

<?php
	include "footer";
?>
