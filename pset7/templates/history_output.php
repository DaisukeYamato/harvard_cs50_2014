<div id="portfolio">
    <table>
        <tr>
            <td>Transaction</td>
            <td>Time</td>
            <td>Symbol</td>
            <td>Shares</td>
            <td>Price</td>
        </tr>
        <?php foreach($history as $row): ?>
            <tr>
                <td><?= $row["transaction"] ?></td>
                <td><?= $row["trans_time"]   ?></td>
                <td><?= $row["symbol"] ?></td>
                <td><?= $row["shares"] ?></td>
                <td>$<?= number_format($row["price"], 2) ?></td>
            </tr>
        <?php endforeach ?>        
    </table>
</div>

