<?php
    session_start();
    if(session_destroy()) { // yeetus session deletus
        header("Location: Login.php/?p=Login"); // yeetus go login fetus
    }
?>
