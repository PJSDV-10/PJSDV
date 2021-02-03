<?php
require "xml.php";

$sock_IP = "127.0.0.1";
$sock_Port = 8080;

/**
 * Sends the given message to the socketserver
 * @param out the message that has to be sent to the server.
 * @return message if the connection was succesful, if not it'll return 0.
**/
function messageSocket($out) {
	global $sock_IP; global $sock_Port;
	$fp = fsockopen($sock_IP, $sock_Port, $errno, $errstr, 1);
	if(!$fp) {
		//echo "$errstr ($errno)<br>";
		return false;
	} else {
		fwrite($fp, $out);
		$in = "";
		while(!feof($fp)) {
			$in .= fgets($fp, 128);
		}
		fclose($fp);
		return $in;
	}
}

function initialiseSocket() {
	$out = "<message>";
	$out .= "<header>";
	$out .= "<sender>website</sender>";
	$out .= "<receiver>server</receiver>";
	$out .= "</header>";
	$out .= "<function>authentication</function>";
	$out .= "<context>";
	$out .= "<password>JeMoederIsEenWachtwoord</password>";
	$out .= "<clientName>Website</clientName>";
	$out .= "<type>website</type>";
	$out .= "</context>";
	$out .= "</message>";

	return xmlCheckAck(messageSocket($out));
}

function requestAllSocket() {
	$out = "<message>";
	$out .= "<header>";
	$out .= "<sender>website</sender>";
	$out .= "<receiver>server</receiver>";
	$out .= "</header>";
	$out .= "<function>getStatusAll</function>";
	$out .= "<context>";
	$out .= "<password>JeMoederIsEenWachtwoord</password>";
	$out .= "</context>";
	$out .= "</message>";

	return messageSocket($out);
}

function toggleDevice($name, $type, $data0, $data1) {
	$out = "<message>";
	$out .= "<header>";
	$out .= "<sender>website</sender>";
	$out .= "<receiver>server</receiver>";
	$out .= "</header>";
	$out .= "<function>changeStatus</function>";
	$out .= "<context>";
	$out .= "<password>JeMoederIsEenWachtwoord</password>";
	$out .= "<wemosjes>";
	$out .= "<wemos>";
	$out .= "<name>${name}</name>";
	$out .= "<type>${type}</type>";
	$out .= "<data1>${data0}</data1>";
	if(isset($data1)) {
		$out .= "<data2>${data1}</data2>";
	}
	$out .= "</wemos>";
	$out .= "</wemosjes>";
	$out .= "</context>";
	$out .= "</message>";

	return messageSocket($out);
}

?>
