<?php
function xmlCheckAck($data) {
    $xml = simplexml_load_string($data) or die;
    if($xml->function == "ack") {
        return true;
    } else {
        return false;
    }
}

function xmlParseDevices($data) {
    $devices = NULL; $i = 0;
    $xml = simplexml_load_string($data) or die;
    foreach($xml->context->wemosjes->children() as $device) {
        $devices[$i] = $device;
        $i++;
    }
    echo $devices[0]->name;
    return $devices;
}

?>
