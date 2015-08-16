<?php

    // configuration
    require("../includes/config.php");
    
    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // check user and password is entered
        if (empty($_POST["symbol"])){
            apologize("You must select symbol to sell.");           
        }
        
        // get current price and shares
        $portfolio = query("SELECT * FROM shares WHERE symbol = ?", $_POST["symbol"]);
        $quote     = lookup($_POST["symbol"]);
        if ( $quote === false ){
            apologize("Cannot find " . $_POST["symbol"] . ".");
        }
        
        if ($portfolio === false){
            apologize("Cannot sell " . $_POST["symbol"] . ".");
        }
        $gain =  $quote["price"] * $portfolio[0]["shares"];
        
        // remove shares from portfolio
        $remove = query("DELETE FROM shares WHERE id = ? AND symbol = ?", 
                        $_SESSION["id"], $_POST["symbol"]);
        // update CASH
        $update = query("UPDATE users SET cash = cash + ? WHERE id = ?", $gain, $_SESSION["id"]);
        
         // Insert history table
        $history = query("INSERT INTO history (transaction, trans_time, symbol, shares, price, id) VALUES('Sell', NOW(), ?, ?, ?, ?)",
                        $_POST["symbol"], $portfolio[0]["shares"], $quote["price"], $_SESSION["id"]);
        
        // redirect index.php
        redirect("/");
    }
    // let sell_form
    else {
        // first list up current shares
        $portfolio = query("SELECT * FROM shares WHERE id = ?", $_SESSION["id"]);
        
        // else render form
        render("sell_form.php", ["title" => "Sell", 
                                 "portfolio" => $portfolio]);
    }
    
?>
