<?php

    // configuration
    require("../includes/config.php");
    
    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // check user and password is entered
        if (empty($_POST["username"]) || empty($_POST["password"])){
            apologize("You must enter username and password");
            
        }
        // check password & confirmation match
        if ($_POST["password"] != $_POST["confirmation"]) {
            apologize("There is difference between password and confirmation.");
        }
        
        // insert username & password (hashed) into database
        $q_result = query("INSERT INTO users (username, hash, cash) VALUES (?, ?, 10000.00)", 
            $_POST["username"], crypt($_POST["password"]));
        if ( $q_result === false ){
            apologize("Failed to create new user...");
        }
        
        // search id in database
        $rows = query("SELECT LAST_INSERT_ID() AS id");
        $id = $rows[0]["id"];
        // set id into session
        $_SESSION["id"] = $id;
        // redirect to index.php
        redirect("/");
        
    }
    else {
        // else render form
        render("register_form.php", ["title" => "Regiser"]);
    }
    
?>
