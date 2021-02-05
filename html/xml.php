<?php
/**
  * checkt of de meegegeven xml een ack bevat.
  * @param data de xml die de ack dient te bevatten.
  * @return bool resultaat of de ack in de xml zat.
**/
function xmlCheckAck($data) {
    $xml = simplexml_load_string($data) or die;
    if($xml->function == "ack") {
        return true;
    } else {
        return false;
    }
}

/**
  * Verwijdert de metadata en parsed de data tot een array van apparaten
  * @param data bevat de ruwe xml data om te parsen
  * @return devices[] bevat enkel de apparaten met hun data.
**/
function xmlParseDevices($data) {
    $devices = NULL; $i = 0;
    $xml = simplexml_load_string($data) or die;

    // pakt enkel de losse wemos apparaten uit het xml bestand als zijnde apparaat
    foreach($xml->context->wemosjes->children() as $device) {
        $devices[$i] = $device;
        $i++;
    }
    return $devices;
}

?>
