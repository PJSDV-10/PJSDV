<div class="main">
<br>
<?php
    require "socket.php";
    $devices;
    if(initialiseSocket()) {
        $devices = xmlParseDevices(requestAllSocket());
    }
    //require "xml.php";
    /*$devices = xmlParseDevices("<message>
            <header>
                <sender>server</sender>
                <receiver>website</receiver>
            </header>
            <function>answerToStatusRequest</function>
            <context>
                <wemosjes>
                    <wemos>
                        <name>UniekeWemosNaamDieDeWemosOokGebruiktBijDeSenderInDeHeader</name>
                        <type>stoel</type>
                        <data1>0</data1>
                    </wemos>
                    <wemos>
                        <name>AndereUniekeWemosNaamDieDeWemosOokGebruiktBijDeSenderInDeHeader</name>
                        <type>lamp</type>
                        <data1>1</data1>
                    </wemos>
                </wemosjes>
            </context>
    </message>");*/
?>
<table class="">
    <thead>
        <tr>
            <th>Device</th>
            <th>Status</th>
            <th>Toggle</th>
        </tr>
    </thead>
    <tbody>
<?php
    $i = 0;
    foreach($devices as $device) {
        echo "<tr>";
        echo "<td>".$device->type."</td>";
        $status = "off";
        if($device->data1 > 0 && (!isset($device->data2) || $device->data2 > 0)) {
            $status = "on";
        }
        echo "<td>".$status."</td>";
        if($status == "on") { $status = "turn off"; }
        else {$status = "turn on"; }
        echo "<td><form method=\"post\"><input type=\"submit\" name=\""
            .$i."\" class=\"button\" value=\"".$status."\"/></form></td>";
        echo "</tr>";
        $i++;
    }
    ?>
    </tbody>
</table>

</div>

<?php
for($i; $i >= 0; $i--) {
    if(isset($_POST[$i])) {
        echo "<br>Switching mode of ".$devices[$i]->type;
        //stel er zou iets bij de server zitten kon ik hiervoor een echte actie kunnen sturen...
        toggleDevice($devices[$i]->name, $devices[$i]->type, $devices[$i]->data1, if(isset($devices[$i]->data2)) {$devices[$i]->data2} else {NULL});
        header("/?p=Status");
    }
}


?>
