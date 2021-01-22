<div class="main">
    <!--<table style="width:100%">
        <tr>
            <th>apparaat</th>
            <th>status</th>
        </tr>
        <tr>
            <td>lamp</td>
            <td id='but'>uit</td>
        </tr>
        <tr>
            <td>lamp</td>
            <td id='but'>uit</td>
        </tr>
        <tr>
            <td>lamp</td>
            <td id='but'>uit</td>
        </tr>
        <tr>
            <td>lamp</td>
            <td id='but'>uit</td>
        </tr>
    </table>-->

    <h1>output:</h1>
    <p><?php
        require "socket.php";
        $msgs;
        if(initialiseSocket()) {
            $msgs = xmlParseDevice(requestAllSocket());
        }
        //echo $msgs;
        foreach($msgs as $msg) {
            echo $msg['name'];
        }
    ?></p>

</div>
