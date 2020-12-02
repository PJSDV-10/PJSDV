<p>submitted text: <?php echo $_POST["teststory"]; ?></p>

<?php
print("<p>starting socket test. . .</p>");

$fp = fsockopen("127.0.0.1", 3016, $errno, $errstr, 20);
if(!$fp) {
	echo "$errstr ($errno)<br>";
} else {
	$out = "GET / HTTP/1.1\r\n";
	$out .= "Host: 127.0.0.1\r\n";
	$out .= "Connection: Close\r\n";
	fwrite($fp,$out);
	while(!feof($fp)) {
		echo fgets($fp, 128);
	}
	fclose($fp);
}

?>
