<?php
include 'db_connect.php';
include 'query_auswertung.php';
include 'diagramm.php';

$path="";

$type_1=4;
$type_2=6;


$sql = new mysql("localhost","xtStats","123","xtStats");


$qlastrunid = $sql->query("SELECT idRun FROM run order by date desc LIMIT 1;");
$idRun = $sql->result(0,'idRun',$qlastrunid);


$CppToHaColumn  = new diagramm($sql,4,$idRun);
$HaToCppColumn  = new diagramm($sql,6,$idRun);

$CppPie  = new diagramm($sql,2,$idRun);
$CppAstCppPie  = new diagramm($sql,3,$idRun);
$HasPie  = new diagramm($sql,5,$idRun);
$HasToAstAstpie  = new diagramm($sql,7,$idRun);


?>

<!DOCTYPE html>
<html>

<head>
<title>xtesting</title>

<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<link rel="stylesheet" type="text/css" href="css/style.css" />
<script src="scripts/jquery.min.js"></script>
<script src="scripts/highcharts.js"></script>

<script type="text/javascript">

function chart(container,titel,FailedValue,ValidValue){
$(function () {
	    var chart;
	    
	    $(document).ready(function () {
	    	
	    	// Build the chart
	        $(container).highcharts({
	            chart: {
	                plotBackgroundColor: null,
	                plotBorderWidth: null,
	                plotShadow: false
	            },
	            title: {
	                text: titel
	            },
	            tooltip: {
	        	    pointFormat: '<b>{point.percentage:.1f}%</b>'
	            },
	            plotOptions: {
	                pie: {
	                    allowPointSelect: true,
	                    cursor: 'pointer',
	                    dataLabels: {
	                        enabled: false
	                    },
	                    showInLegend: false
	                }
	            },
	            series: [{
	                type: 'pie',
	                name: 'C++',
	                data: [
	                       {
	                           name: 'Failed',
	                           y: FailedValue,
	                           color:'#A40B0B' 
	                       }, {
	                           name: 'Valid',
	                           y: ValidValue,
	                           color: '#0BA50B'
	                       }
	                ]
	            }]
	        });
	    });
	    
	});}

</script>
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
						<li class="current"><a href="index.php">Home</a></li>
						<li><a href="interpreter.php">Interpreter</a></li>
						<li><a href="interface.php">Interface</a></li>
						<li><a href="performance.php">Performance</a></li>
					</ul>
				</div>
				<!--close menubar-->
			</nav>
		</header>

		<!--close slideshow_container-->



		<div id="container">
			
			<div id="container1"></div>
			
			<div id="pies">
				<div id="container2" style="float: left;width: 25%; height: 200px;"></div>
				<div id="container3" style="float: left;width: 25%; height: 200px;"></div>
				<div id="container4" style="float: left;width: 25%; height: 200px;"></div>
				<div id="container5" style="float: left;width: 25%; height: 200px;"></div>
			</div>
		</div>
		<!--close main-->
	</div>


	<script type="text/javascript">
  


   $(function () {
		  
	    $('#container1').highcharts({
	        
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
	            categories: ['C++ -&gt; AST -&gt; Haskell', 'Haskell -&gt; AST -&gt; C++']
	        },
	        yAxis: {
	            min: 0,
	            title: {
	                text: '% Testfälle'
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
	                return '<b>' + this.x + '</b><br/>' + this.y + '% ' + this.series.name;
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
	            name: 'fehlgeschlagen',
	            data: [<?php echo( $CppToHaColumn->Failed.",".$HaToCppColumn->Failed )?>]
	        }, {
	            name: 'erfolgreich',
	            data: [<?php echo( $CppToHaColumn->Valid.",".$HaToCppColumn->Valid) ?>]
	        }]
	    });
	});
  
   chart('#container2','C++',<?php echo( $CppPie->Failed)?>,<?php echo( $CppPie->Valid)?> );
   chart('#container3','C++ -&gt; Ast -&gt; C++',<?php echo( $CppAstCppPie->Failed)?>,<?php echo( $CppAstCppPie->Valid)?> );
   chart('#container4','Haskell',<?php echo( $HasPie->Failed)?>,<?php echo( $HasPie->Valid)?> );
   chart('#container5','Haskell -&gt; Ast -&gt; Haskell',<?php echo( $HasToAstAstpie->Failed)?>,<?php echo( $HasToAstAstpie->Valid)?> );
</script>
</body>
</html>
<?php 
$sql->free_result();

$sql->close_connect();


?>