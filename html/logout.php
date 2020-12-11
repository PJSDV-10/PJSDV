<?php
    session_start();
    if(session_destroy()) { // yeetus session deletus
        header("Location: loginForm.php?p=login"); // yeetus go login fetus
    }
?>
