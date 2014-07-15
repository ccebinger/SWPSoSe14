<?php
include 'db_connect.php';
include 'query_auswertung.php';
include 'diagramm.php';


// Das ist ein Sicherheisproblem (mysql injection) ...
#$idRun = $_GET["idrun"] ; // wenn null wird einfach der letzte run ausgewertet



$sql = new mysql("localhost","xtStats","123","xtStats");


$qlastrunid = $sql->query("SELECT idRun FROM run order by date desc LIMIT 1;");
$idRun = $sql->result(0,'idRun',$qlastrunid);


$result = $sql->query("SELECT type, ROUND(AVG(durationMs)) AS avg, COUNT(durationMs) AS count FROM `result` WHERE idRun='".$idRun."' AND blame=0 AND testcaseId=0 AND (type=2 OR type=5) GROUP BY type ORDER BY type ASC;");
$statsCompile = array();
while($line=mysql_fetch_array($result)) {
	$statsCompile[] = $line;
}

#var_dump($statsCompile[0]['avg'] / ($statsCompile[0]['avg'] + $statsCompile[1]['avg']));
#var_dump($statsCompile[1]['avg'] / ($statsCompile[0]['avg'] + $statsCompile[1]['avg']));



$result = $sql->query("SELECT type, ROUND(AVG(durationMs)) AS avg, COUNT(durationMs) AS count FROM `result` WHERE idRun='".$idRun."' AND blame=0 AND testcaseId>0 AND (type=1 OR type=2 OR type=5) GROUP BY type ORDER BY type ASC;");
$statsExec = array();
while($line=mysql_fetch_array($result)) {
	$statsExec[] = $line;
}


// var_dump($statsExec[0]['avg']); # Interpreter
// var_dump($statsExec[1]['avg']); # C++
// var_dump($statsExec[2]['avg']); # Haskell


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
						<li><a href="interpreter.php">Interpreter</a></li>
						<li><a href="interface.php">Interface</a></li>
						<li class="current"><a href="performance.php">Performance</a></li>
					</ul>
				</div>
				<!--close menubar-->
			</nav>
		</header>

		<!--close slideshow_container-->
			
			
			

			<div id="container">
			
				<div class="perf">
					<h1>Durchschnittliche Zeit zum Kompilieren (weniger ist besser)</h1>
					<div id="chartCompile" class="perfGraph"></div>
					<script type="text/javascript">
						$(function () {
						    $('#chartCompile').highcharts({
						        chart: { type: 'column' },
						        plotOptions: {
					               column: {
					                   colorByPoint: true
					               }
					           },
					           colors: ['#A40B0B'],
						       title: { text: '' },
						       xAxis: { categories: ['C++', 'Haskell'] },
						       yAxis: {
						            min: 0,
						            title: {
						                text: 'Dauer in ms'
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
						                return '<b>' + this.x + '</b><br/>' + this.y + ' ' + this.series.name;
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
						            name: 'ms average',
						            data: [
						            	<?php echo $statsCompile[0]['avg'];?>,
						            	<?php echo $statsCompile[1]['avg'];?>
						            ]
						        }]
						    });
						});
					</script>
				</div>
				
				<div class="perf">
					<h1>Durchschnittliche Testzeit (weniger ist besser)</h1>
					<div id="chartTesttime" class="perfGraph"></div>
					<script type="text/javascript">
						$(function () {
						    $('#chartTesttime').highcharts({
						        chart: { type: 'column' },
						        plotOptions: {
					               column: {
					                   colorByPoint: true
					               }
					           },
					           colors: ['#A40B0B'],
						       title: { text: '' },
						       xAxis: { categories: ['Interpreter', 'C++', 'Haskell'] },
						       yAxis: {
						            min: 0,
						            title: {
						                text: 'Dauer in ms'
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
						                return '<b>' + this.x + '</b><br/>' + this.y + ' ' + this.series.name;
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
						            name: 'ms average',
						            data: [
						            	<?php echo $statsExec[0]['avg']; #Interpreter ?>,
						            	<?php echo $statsExec[1]['avg']; #C++ ?>,
						            	<?php echo $statsExec[2]['avg']; #Haskell ?>
						            ]
						        }]
						    });
						});
					</script>
				</div>
				
				
			
			</div>
		
		<!--close main-->
	</div>


	
	
	
	
	
	
	
	
	
	
	
	

	
	
	
	
	
	
	
	
	
	
	
	
	
</body>
</html>
<?php 
$sql->free_result();

$sql->close_connect();


?>