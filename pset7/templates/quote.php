<?php

    // configuration
    require("../includes/config.php");
    
    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // check user and password is entered
        if (empty($_POST["symbol"])){
            apologize("You must enter symbol to quote.");           
        }

        $quote = lookup($_POST["symbol"]);
        if ( $quote === false ){
            apologize("Cannot find " . $_POST["symbol"] . ".");
        }
        
        $s = $quote["symbol"] . " (" . $quote["name"] . ") value: " 
            . number_format($quote["price"], 2);
        render("quote_output.php", ["quote" => $quote]);
        exit;
    }
    else {
        // else render form
        render("quote_form.php", ["title" => "Portfolio"]);
    }
    
?>
