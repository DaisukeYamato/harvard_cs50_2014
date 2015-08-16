<?php

    // configuration
    require("../includes/config.php");
    
    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // check all fields are entered.
        if (empty($_POST["password"]) || empty($_POST["confirmation"])){
            apologize("You must enter new password & confirmation.");
        }
        // check password & confirmation match
        if ($_POST["password"] != $_POST["confirmation"]) {
            apologize("There is difference between new password and confirmation.");
        }
                
        $update = query("UPDATE users SET hash = ? WHERE id = ?", 
                        crypt($_POST["password"]), $_SESSION["id"]);

        // redirect to index.php
        render("message.php", ["message" => "Your password is changed successfully."]);
        
    }
    else {
        // else render form
        render("change_form.php", ["title" => "Regiser"]);
    }
    
?>
