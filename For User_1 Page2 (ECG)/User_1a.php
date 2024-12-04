
<?php
    include_once('User_1a-esp-database.php');
    if ($_GET["readingsCount"]){
      $data = $_GET["readingsCount"];
      $data = trim($data);
      $data = stripslashes($data);
      $data = htmlspecialchars($data);
      $readings_count = $_GET["readingsCount"];
    }
    // default readings count set to 20
    else {
      $readings_count = 100;
    }

    
?>

<!DOCTYPE html>
<html>
    <head><meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    
        <link rel="stylesheet" href="//cdnjs.cloudflare.com/ajax/libs/morris.js/0.5.1/morris.css ">
        <link rel="stylesheet" type="text/css" href="User_b4-esp-style.css"> 
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
        <script src="https://cdnjs.cloudflare.com/ajax/libs/raphael/2.1.0/raphael-min.js"></script>
        <script src="https://cdnjs.cloudflare.com/ajax/libs/morris.js/0.5.1/morris.min.js"></script>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    </head>
    
    <header class="header">
        <h1>ECG MEASUREMENTS</h1>
        <form method="get">
            <input type="number" name="readingsCount" min="1" placeholder="Number of readings: <?php echo $readings_count; ?>">
            <input type="submit" value="UPDATE">
        </form>
    </header>
<body>
    
    
    <h3 align="center">Last reading</h3>   
    
    <div id="myChart" style="width:100%; max-width:1000px; height:300px;"></div>

    <script>
        google.charts.load('current',{packages:['corechart']});
        google.charts.setOnLoadCallback(drawChart);

        function drawChart() {
            // Set Data
            var data = google.visualization.arrayToDataTable([
                ['', 'Value'],
                <?php $result = getAllReadings($readings_count);
                if ($result) {
                    while ($row = $result->fetch_assoc()) {
                        $row_id = $row["id"];
                        $row_value1 = $row["value1"];
                        $row_reading_time = $row["reading_time"];
                        $row_reading_time = date("Y-m-d H:i:00", strtotime("$row_reading_time + 9 hours"));
                    
                        echo "[". $row_id .", " . $row_value1 ."], ";
                    }
                    $result->free();
                }
                ; ?>
                
            ]);
            // Set Options
            var options = {
                title: 'Values',
                hAxis: {title: ''},
                vAxis: {title: ''},
                legend: 'none'
            };
            // Draw
            var chart = new google.visualization.LineChart(document.getElementById('myChart'));
            chart.draw(data, options);
            }
    </script>
    
    
<?php
    echo   '<h2> Last ' . $readings_count . ' Readings</h2>
            <table cellspacing="5" cellpadding="5" id="tableReadings">
                <tr>
                    <th>ID</th>
                    <th>Value</th>
                    <th>Timestamp</th>
                </tr>';
 
    $result = getAllReadings($readings_count);
        if ($result) {
        while ($row = $result->fetch_assoc()) {
            $row_id = $row["id"];
            $row_value1 = $row["value1"];
            $row_reading_time = $row["reading_time"];
            // Uncomment to set timezone to - 1 hour (you can change 1 to any number)
            //$row_reading_time = date("Y-m-d H:i:s", strtotime("$row_reading_time - 1 hours"));
            // Uncomment to set timezone to + 7 hours (you can change 7 to any number)
            $row_reading_time = date("Y-m-d H:i:00", strtotime("$row_reading_time + 9 hours"));

            echo '<tr>
                    <td>' . $row_id . '</td>
                    <td>' . $row_value1 . '</td>
                    <td>' . $row_reading_time . '</td>
                  </tr>';
        }
        echo '</table>';
        $result->free();
    }
?>
    
</body>
</html>