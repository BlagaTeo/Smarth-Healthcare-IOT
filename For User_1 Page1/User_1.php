
<?php
    include_once('a-esp-database.php');
    if ($_GET["readingsCount"]){
      $data = $_GET["readingsCount"];
      $data = trim($data);
      $data = stripslashes($data);
      $data = htmlspecialchars($data);
      $readings_count = $_GET["readingsCount"];
    }
    // default readings count set to 20
    else {
      $readings_count = 10;
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
        <h1>DEVICE MEASUREMENTS</h1>
        <form method="get">
            <input type="number" name="readingsCount" min="1" placeholder="Number of readings: <?php echo $readings_count; ?>">
            <input type="submit" value="UPDATE">
        </form>
    </header>
<body>
    
    
    
    <script>
    google.charts.load('current', {packages: ['corechart', 'line']});
    google.charts.setOnLoadCallback(drawBasic);

    function drawBasic() {
        
      var data = new google.visualization.DataTable();
      data.addColumn('number', 'Y');
      data.addColumn('number', 'Temperature');

      data.addRows([
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

      var options = {
          chart: {
                title: '',     
          },
          
            height: 400,
            colors: ['Red']
      };

      var chart = new google.visualization.LineChart(document.getElementById('Temperature'));

      chart.draw(data, options);
    }
    </script>
    
    <div class="container" style="width:900px;">
        <h3 align="center">Temperature Values</h3>   
        <div id="Temperature"></div>
        <br /><br />
    </div>
    
    <script>
    google.charts.load('current', {packages: ['corechart', 'line']});
    google.charts.setOnLoadCallback(drawBasic);

    function drawBasic() {

      var data = new google.visualization.DataTable();
      data.addColumn('number', 'Y');
      data.addColumn('number', 'Pulse');

      data.addRows([
        <?php $result = getAllReadings($readings_count);
                if ($result) {
                    while ($row = $result->fetch_assoc()) {
                        $row_id = $row["id"];
                        $row_value2 = $row["value2"];
                        $row_reading_time = $row["reading_time"];
                        $row_reading_time = date("Y-m-d H:i:00", strtotime("$row_reading_time + 9 hours"));
                    
                        echo "[". $row_id .", " . $row_value2 ."], ";
                    }
                    $result->free();
                }
        ; ?>
    ]);

      var options = {
          chart: {
                title: '',     
          },
          
            height: 400,
            colors: ['Green']
      }; 

      var chart = new google.visualization.LineChart(document.getElementById('Pulse'));

      chart.draw(data, options);
    }
    </script>
    
    <div class="container" style="width:900px;">
        <h3 align="center">Pulse Values</h3>   
        <div id="Pulse"></div>
        <br /><br />
    </div>
    
    <script>
    google.charts.load('current', {packages: ['corechart', 'line']});
    google.charts.setOnLoadCallback(drawBasic);

    function drawBasic() {

      var data = new google.visualization.DataTable();
      data.addColumn('number', 'Y');
      data.addColumn('number', 'Oxygen');

      data.addRows([
        <?php $result = getAllReadings($readings_count);
                if ($result) {
                    while ($row = $result->fetch_assoc()) {
                        $row_id = $row["id"];
                        $row_value3 = $row["value3"];
                        $row_reading_time = $row["reading_time"];
                        $row_reading_time = date("Y-m-d H:i:00", strtotime("$row_reading_time + 9 hours"));
                    
                        echo "[". $row_id .", " . $row_value3 ."], ";
                    }
                    $result->free();
                }
        ; ?>
    ]);

      var options = {
          chart: {
                title: '',     
          },
          
            height: 400,
            colors: ['Blue']
      };

      var chart = new google.visualization.LineChart(document.getElementById('Oxygen'));

      chart.draw(data, options);
    }
    </script>
    
    <div class="container" style="width:900px;">
        <h3 align="center">Oxygen Values</h3>   
        <div id="Oxygen"></div>
        <br /><br />
    </div>
    
    <script>
    google.charts.load('current', {packages: ['corechart', 'bar']});
    google.charts.setOnLoadCallback(drawMultSeries);

    function drawMultSeries() {
        var data = new google.visualization.DataTable();
        data.addColumn('number', 'Time of Day');
        data.addColumn('number', 'Temp');
        data.addColumn('number', 'Pulse');
        data.addColumn('number', 'Ox');
        
        data.addRows([
            <?php $result = getAllReadings($readings_count);
                if ($result) {
                while ($row = $result->fetch_assoc()) {
                    $row_id = $row["id"];
                    $row_value1 = $row["value1"];
                    $row_value2 = $row["value2"];
                    $row_value3 = $row["value3"];
                    $row_reading_time = $row["reading_time"];
                    $row_reading_time = date("Y-m-d H:i:00", strtotime("$row_reading_time + 9 hours"));

                    echo "[". $row_id .", " . $row_value1 .", " . $row_value2 .", " . $row_value3 ."], ";
                    }
                $result->free();
                }
            ; ?>
        ]);
        
        var options = {
            title: '',
            colors: ['#D64949', '#229031','#196CA2'],
            height:400,
        };

      var chart = new google.visualization.ColumnChart(document.getElementById('STUFF'));

      chart.draw(data, options);
    }
    </script>
    
    <div class="container" style="width:1000px;">
        <h3 align="center">Values Summary</h3>   
        <div id="STUFF"></div>
        <br /><br />
    </div>
    
    <h3 align="center">If you wish to view your ECG measurements please click the button below</h3>   
        <br /><br />
    <a class="button" href="User_1a.php" target="_blank">View results</a>
    
<?php
    echo   '<h2> Last ' . $readings_count . ' Readings</h2>
            <table cellspacing="5" cellpadding="5" id="tableReadings">
                <tr>
                    <th>ID</th>
                    <th>Temperature</th>
                    <th>Pulse</th>
                    <th>Oxygen</th>
                    <th>Timestamp</th>
                </tr>';

    $result = getAllReadings($readings_count);
        if ($result) {
        while ($row = $result->fetch_assoc()) {
            $row_id = $row["id"];
            $row_value1 = $row["value1"];
            $row_value2 = $row["value2"];
            $row_value3 = $row["value3"];
            $row_reading_time = $row["reading_time"];
            // Uncomment to set timezone to - 1 hour (you can change 1 to any number)
            //$row_reading_time = date("Y-m-d H:i:s", strtotime("$row_reading_time - 1 hours"));
            // Uncomment to set timezone to + 7 hours (you can change 7 to any number)
            $row_reading_time = date("Y-m-d H:i:00", strtotime("$row_reading_time + 9 hours"));

            echo '<tr>
                    <td>' . $row_id . '</td>
                    <td>' . $row_value1 . '</td>
                    <td>' . $row_value2 . '</td>
                    <td>' . $row_value3 . '</td>
                    <td>' . $row_reading_time . '</td>
                  </tr>';
        }
        echo '</table>';
        $result->free();
    }
?>
    
</body>
</html>