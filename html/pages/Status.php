<meta http-equiv="refresh" content="10" >

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
    if(isset($_POST[$i])) { //checkt of er op een schakel knop gedrukt is.
        echo "<br>Switching mode of ".$devices[$i]->type;

        // stuurt de vereiste data naar de server met een verzoek tot aanpassen
	$data1 = 1;
	if($devices[$i]->data1 == 1) {
		$data1 = 0;
	}
	$data2 = 1;
	if(isset($devices[$i]->data2)) {
		if($devices[$i]->data2 == 1) {
			$data2 = 0;
		}
	} else {
		$data2 = NULL;
	}
        toggleDevice($devices[$i]->name, $devices[$i]->type, $data1, $data2);
	
        // herlaad de pagina.
        //header("/?p=Status");
    }
}


?>