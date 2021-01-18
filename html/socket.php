<?php
$fp = fsockopen("127.0.0.1", 8080, $errno, $errstr, 1);
if(!$fp) {
	echo "$errstr ($errno)<br>";
} else {
	$out = "<message>
	    <header>
	        <sender>website</sender>
	        <receiver>server</receiver>
	    </header>
	    <function>authentication</function>
	    <context>
	        <password>JeMoederIsEenWachtwoord</password>
	        <clientName>Website</clientName>
	        <type>website</type>
	    </context>
	</message>";

	fwrite($fp,$out);
	while(!feof($fp)) {
		echo fgets($fp, 128);
	}
	fclose($fp);
}

?>
