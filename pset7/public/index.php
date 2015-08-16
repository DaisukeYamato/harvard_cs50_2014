<?php

    // configuration
    require("../includes/config.php"); 
    
    // get current cash
    $user_info = query("SELECT * FROM users WHERE id = ?", $_SESSION["id"]);
    $cash = $user_info[0]["cash"];
    
    // get current shares
    $rows = query("SELECT id, symbol, shares FROM shares WHERE id = ?", $_SESSION["id"]);
    
    // get prices using shares
    $shares = [];
    foreach ($rows as $row){
        $stock = lookup($row["symbol"]);
        if ($stock !== false){
            $shares[] = [
                "name" => $stock["name"],
                "price" => $stock["price"],
                "shares" => $row["shares"],
                "symbol" => $row["symbol"]
            ];
        }
    }
    
    // render portfolio
    render("portfolio.php", ["cash" => $cash, "shares" => $shares, "title" => "Portfolio"]);

?>
