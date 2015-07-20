<?php 
$sensors = parse_ini_file("config.ini",true);
$endValues = explode( " ",end(file("orto.log")));

$data  = date('Ymd');
?>
	<head>
		<title>InternetOfGreens</title>
		<link href="css/iog.css" rel="stylesheet" type="text/css">
	</head>
	<body>
		<div id="container">
			<div class="head-container">
				<img src="images/head_ortotecnologico.jpg" border="0">
			</div>
			<div class="list-container">
				<ul class="list-container-ul">
					<li class="list-item">Sensore</li>
					<?php foreach ($sensors as $var => $value) { ?>
						<li class="list-item">
							<ul>
								<li class="list-subitem"><a href="data.php?sensor=<?php echo $var."&data=".$data; ?>"><?php echo $value['label']; ?></a></li>
								<li class="list-subitem"><?php echo "<b>" . $endValues[($value['pos']-1)] . "</b> " . $value['unita']; ?></li>
							</ul>
						</li>
					<?php } ?>
				</ul>
			</div>
			<div class="head-container footer-container">
				Powered by <a href="http://www.wemake.cc" target="_blank"><img src="images/logo_wemake.gif" border="0" width="32"></a>
				<a href="http://www.mauroalfieri.it" target="_blank"><img src="images/logo_mauroalfieri.gif" border="0" width="32"></a>
				&nbsp; &nbsp;
			</div>
		</div>		
	</body>
</html>
