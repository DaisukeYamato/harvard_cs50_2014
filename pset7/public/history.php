<?php

    // configuration
    require("../includes/config.php");

    // qet history
    $history = query("SELECT transaction, trans_time, symbol, shares, price FROM history WHERE id = ? ORDER BY trans_time DESC", 
                    $_SESSION["id"]);
    
    render("history_output.php", ["history" => $history]);  
?>
