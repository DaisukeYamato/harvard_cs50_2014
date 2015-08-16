<?php

    // configuration
    require("../includes/config.php");
    
    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // check symbol entry
        if (empty($_POST["symbol"])){
            apologize("You must select symbol to sell.");           
        }
        // check share entry
        if (empty($_POST["shares"])){
            apologize("You must enter shares to buy.");
        }
        
        // check shares entered correctly (do not allow fraction)
        if (!preg_match("/^\d+$/", $_POST["shares"])){
            apologize("You must enter shares by integer.");
        }
        
        // to upper 
        $symbol = strtoupper($_POST["symbol"]);
        
        // get current price
        $quote = lookup($_POST["symbol"]);
        if ($quote === false){
            apologize("Cannot find " . $symbol . ".");
        }
        
        // get current cash
        $current_cash = query("SELECT cash FROM users WHERE id = ?", $_SESSION["id"]);
        //$current_cash = $states[0]["cash"];
        
        // if (current cash) < (price * shares)
        if ( $current_cash < $quote["price"] * $_POST["shares"]){
            apologize("You do not have enough money to buy...");
        }
        
        // OK. Now buy it.
        $update = query("INSERT INTO shares (id, symbol, shares) VALUES(?, ?, ?) ON DUPLICATE KEY UPDATE shares = shares + VALUES(shares)",
            $_SESSION["id"], $symbol, $_POST["shares"]);
        $update_cash = query("UPDATE users SET cash = cash - ? WHERE id = ?", 
                            $quote["price"] * $_POST["shares"], $_SESSION["id"]);
        
        // Insert history table
        $history = query("INSERT INTO history (transaction, trans_time, symbol, shares, price, id) VALUES ('Buy', NOW(), ?, ?, ?, ?)",
                        $symbol, $_POST["shares"], $quote["price"], $_SESSION["id"]);
        
        // redirect index.php
        redirect("/");
    }
    // let buy_form
    else {
       // else render form
        render("buy_form.php", ["title" => "Buy"]);
    }
    
?>
