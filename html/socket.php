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

/**
  * Initialiseert de socketverbinding met de server.
  * @param void
  * @return bool eerst wordt het opgestelde bericht verzonden via socket,
  *				 daarna wordt de xml die resulteert door checkack heen gehaald
**/
function initialiseSocket() {
	$out = "<message>";
	$out .= "<header>";
	$out .= "<sender>website</sender>";
	$out .= "<receiver>server</receiver>";
	$out .= "</header>";
	$out .= "<function>authentication</function>";
	$out .= "<context>";
	$out .= "<password>solarwinds123</password>";
	$out .= "<clientName>Website</clientName>";
	$out .= "<type>website</type>";
	$out .= "</context>";
	$out .= "</message>";

	return xmlCheckAck(messageSocket($out));
}

/**
  * Stuur een verzoek aan de server voor alle data aangaande de wemosjes
  * @param void
  * @return out een xml object met alle data aangaande de wemosjes
**/
function requestAllSocket() {
	$out = "<message>";
	$out .= "<header>";
	$out .= "<sender>website</sender>";
	$out .= "<receiver>server</receiver>";
	$out .= "</header>";
	$out .= "<function>getStatusAll</function>";
	$out .= "<context>";
	$out .= "<password>solarwinds123</password>";
	$out .= "</context>";
	$out .= "</message>";

	return messageSocket($out);
}

/**
  * Stuurt een verzoek aan de server om een wemosje van status te laten veranderen.
  * @param name is de unieke naam van de wemos
  * @param type soort apparaat dat aan de wemos hangt
  * @param data0 waarde van de data die bij het apparaat hoort
  * @param data1 waarde van een extra datapunt van het apparaat
  * @return out het xml object dat terug komt van de server
**/
function toggleDevice($name, $type, $data0, $data1) {
	$out = "<message>";
	$out .= "<header>";
	$out .= "<sender>website</sender>";
	$out .= "<receiver>server</receiver>";
	$out .= "</header>";
	if($data0 > 0) {
		$dt = "Aan";
	} else {
		$dt = "Uit";
	}
	$out .= "<function>changeStatus${dt}</function>";
	$out .= "<context>";
	$out .= "<password>solarwinds123</password>";
	$out .= "<clientName>${name}</clientName>";
	$out .= "<type>${type}</type>";
	$out .= "<data1>${data0}</data1>";
	if($type == "Column") {
	} else {
		$out .= "<data2>${data1}</data2>";
	}
	$out .= "</context>";
	$out .= "</message>";

	return messageSocket($out);
}

?>
