<?php
include 'db_connect.php';
include 'query_auswertung.php';
include 'diagramm.php';


// Das ist ein Sicherheisproblem (mysql injection) ...
#$idRun = $_GET["idrun"] ; // wenn null wird einfach der letzte run ausgewertet



$sql = new mysql("localhost","xtStats","123","xtStats");


$qlastrunid = $sql->query("SELECT idRun FROM run order by date desc LIMIT 1;");
$idRun = $sql->result(0,'idRun',$qlastrunid);



$CppColumn  = new diagramm($sql,1,$idRun);
$HaColumn  = new diagramm($sql,2,$idRun);
$IntColumn  = new diagramm($sql,5,$idRun);


?>

<!DOCTYPE html>
<html>

<head>
<title>Cross Testing</title>

<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<link rel="stylesheet" type="text/css" href="css/style.css" />
<script src="scripts/jquery.min.js"></script>
<script src="scripts/highcharts.js"></script>
</head>

<body>

	<div id="main">

		<header>
			<div id="strapline">
				<div id="welcome_slogan">
					<h3>
						<span>Cross-Testing</span>
					</h3>
				</div>
				<!--close welcome_slogan-->
			</div>
			<!--close strapline-->
			<nav>
				<div id="menubar">
					<ul id="nav">
						<li><a href="index.php">Home</a></li>
						<li  class="current" ><a href="interpreter.php">Interpreter</a></li>

					</ul>
				</div>
				<!--close menubar-->
			</nav>
		</header>

		<!--close slideshow_container-->

		

			<div id="container" style="width: 100%; height: 400px;"></div>
		
		<!--close main-->
	</div>


	<script type="text/javascript">
  
   $(function () {
	  
	    $('#container').highcharts({
	        
	        chart: {
	            type: 'column'
	        },

	        plotOptions: {
                column: {
                    colorByPoint: true
                }
            },
            colors: [
                '#A40B0B',
                '#0BA50B'],

	        
	        title: {
	            text: 'Stacked column chart'
	        },
	        xAxis: {
	            categories: ['C++','Interpreter', 'Haskell']
	        },
	        yAxis: {
	            min: 0,
	            title: {
	                text: ' X-testing Interpreter Tests'
	            },
	            stackLabels: {
	                enabled: false,
	                style: {
	                    fontWeight: 'bold',
	                    color: (Highcharts.theme && Highcharts.theme.textColor) || 'gray'
	                }
	            }
	        },
	        legend: {
	          
	            verticalAlign: 'top',
	            y: 0,
	            floating: true,
	            backgroundColor: (Highcharts.theme && Highcharts.theme.background2) || 'white',
	            borderColor: '#CCC',
	            borderWidth: 1,
	            shadow: false
	        },
	        tooltip: {
	            formatter: function () {
	                return '<b>' + this.x + '</b><br/>' + this.series.name + ': ' + this.y + '<br/>' +
	                    'Total: ' + this.point.stackTotal;
	            }
	        },
	        plotOptions: {
	            column: {
	                stacking: 'normal',
	                dataLabels: {
	                    enabled: true,
	                    color: (Highcharts.theme && Highcharts.theme.dataLabelsColor) || 'white',
	                    style: {
	                        textShadow: '0 0 3px black, 0 0 3px black'
	                    }
	                }
	            }
	        },
	        series: [{
	            name: 'Fehlgeschlagene Tests',
	            data: [<?php echo( $CppColumn->Failed.",".$IntColumn->Failed.",".$HaColumn->Failed )?>]
	        }, {
	            name: 'Erfolgreiche Tests ',
	            data: [<?php echo( $CppColumn->Valid.",".$IntColumn->Valid.",".$HaColumn->Valid )?>]
	        }]
	    });
	});
</script>
</body>
</html>
<?php 
$sql->free_result();

$sql->close_connect();


?>