<?php
	include "header.php";
?>


<div class="main">
	<form action="login.php" method="post" class="center loginform">
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
