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
						<li class="current"><a href="interface.php">Interface</a></li>
						<li><a href="performance.php">Performance</a></li>
						<li><a href="screencast.php">Screencast</a></li>
					</ul>
				</div>
				<!--close menubar-->
			</nav>
		</header>

		<!--close slideshow_container-->

		

			<div id="container" class="interface">


<h1>Aufbau</h1>
Der abstrakte Syntaxgraph wird serialisiert und als .csv-Datei gespeichert.
Jede Rail-Funktion wird durch ihren Funktionsnamen in eckigen Klammern eingeleitet: [funcName].
<br />
Danach folgt eine Semikolon-separierte Liste aller Knoten der Funktion inklusive deren Eigenschaften:

<ul>
	<li>Id des Knotens</li>
	<li>Railbefehl</li>
	<li>Id des 1. Folgeknotens oder 0, falls kein Knoten vorhanden ist</li>
	<li>Id des 2. Folgeknotens oder 0, falls kein Knoten vorhanden ist</li>
</ul>

<pre>
[Funktionsname 1]
Id ; Railbefehl ; 1. Adjazenz-Id ; 2. Adjazenz-Id
Id ; Railbefehl ; 1. Adjazenz-Id ; 2. Adjazenz-Id
...

[Funktionsname 2]
Id ; Railbefehl ; 1. Adjazenz-Id ; 2. Adjazenz-Id
Id ; Railbefehl ; 1. Adjazenz-Id ; 2. Adjazenz-Id
...
</pre>


<br /><br />			
<h1>Beispiel</h1>
<pre>
[main]
1;[0];2;0
2;[0];3;0
3;v;7;5
4;[False];5;0
5;o;6;0
6;#;0;0
7;[True];8;0
8;o;9;0
9;#;0;0

</pre>
			
			</div>
		
		<!--close main-->
	</div>


	
</body>
</html>