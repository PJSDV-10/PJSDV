<!--meta http-equiv="refresh" content="10" -->

<div class="main">
<br>
<?php
    require "socket.php"; // voor de verbinding met de server.
    $devices; // scoping
    if(initialiseSocket()) { // test of initialisatie slaagt
        $devices = xmlParseDevices(requestAllSocket()); // haalt alle data op en parsed het
    } else {
	       $devices = NULL;
    }

    $apparaten = array(
        array("Column", "Push button: ", "Gas sensor: "),
        array("Bed", "Push button: ", "Force sensor: "),
        array("Chair", "Force sensor: ", "Push button: "),
        array("Table-lamp", "PIR sensor: ", "Force sensor: ")
    )
    function onOrOff($data) {
        if($data > 0) {
            return "aan";
        } else {
            return "uit";
        }
    }
    function getDevice($type, $dat1, $dat2) {
        $returnString = "";
        $index = 0;
        for($apparaten as $app) {
            if($type == $app) {
                $returnString .= $apparaten[$index][0] . onOrOff($dat1) . "<br>";
                $returnString .= $apparaten[$index][1] . onOrOff($dat2);
            }
            $index++;
        }
        return $returnString;
    }
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
                echo "<td>".getDevice($device->type, $device->data1, $device->data2)."</td>";
        if($device->data1 > 0) { $status = "turn off"; }
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
    if(isset($_POST[$i])) { //checkt of er op een schakel knop gedrukt is.
        echo "<br>Switching mode of ".$devices[$i]->type." from ".$devices[$i]->data1."&".$devices[$i]->data2;

        // stuurt de vereiste data naar de server met een verzoek tot aanpassen
	$d1 = 1;
	if($devices[$i]->data1 == 1) {
		$d1 = 0;
	}
	$d2 = NULL;
	if($devices[$i]->data2 == 1) {
		$d2 = 0;
	} else if($devices[$i]->data2 == 0){
		$d2 = 1;
	}
        toggleDevice($devices[$i]->name, $devices[$i]->type, $d1, $d2);

        // herlaad de pagina.
        //header("/?p=Status");
    }
}


?>
