<div id="portfolio">
    <div class="toplinks">
        <a href="/quote.php">Quote</a>
        <a href="/buy.php">Buy</a>
        <a href="sell.php">Sell</a>
        <a href="history.php">History</a>
        <a href="logout.php">Log Out</a>
    </div>
    <table>
        <tr>
            <td>Symbol</td>
            <td>Name</td>
            <td>Shares</td>
            <td>Price</td>
            <td>TOTAL</td>
        </tr>
        <?php foreach($shares as $share): ?>
            <tr>
                <td><?= $share["symbol"] ?></td>
                <td><?= $share["name"]   ?></td>
                <td><?= $share["shares"] ?></td>
                <td>$<?= $share["price"] ?></td>
                <td>$<?= $share["price"] * $share["shares"]?></td>
            </tr>
        <?php endforeach ?>
        <tr>
            <td>CASH</td>
            <td></td>
            <td></td>
            <td></td>
            <td>$<?= number_format($cash, 2)?></td>
        </tr>
        
    </table>
</div>
<div>
    <a href="change.php">Change Password</a>
    <a href="logout.php">Log Out</a>
</div>
