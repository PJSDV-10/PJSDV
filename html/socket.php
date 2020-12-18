<p>submitted text: <?php echo $_POST["teststory"]; ?></p>

<?php
print("<p>starting socket test. . .</p>");

$fp = fsockopen("127.0.0.1", 3016, $errno, $errstr, 20);
if(!$fp) {
	echo "$errstr ($errno)<br>";
} else {
	$out = "test";
	fwrite($fp,$out);
	while(!feof($fp)) {
		echo fgets($fp, 128);
	}
	fclose($fp);
}

?>
