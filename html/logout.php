<?php
    session_start();
    if(session_destroy()) { // yeetus session deletus
        header("Location: loginForm.php?p=Login"); // yeetus go login fetus
    }
?>
