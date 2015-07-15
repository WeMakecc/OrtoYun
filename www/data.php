<?php
/***********************************************************************************/

$sensorData = array();
$json_a = array();
$labelTitle = "";

/***********************************************************************************/

if ( !empty($_GET['sensor']) ) {
	
	$sensors = parse_ini_file("config.ini",true);
	$sensor = $sensors[$_GET['sensor']];
	
	$rows = file("orto.log");
	$pos  = $sensor['pos'];
	
	/***********************************************************************************/
	
	foreach ($rows as $line) {
        $vals = explode(" ", $line);
	
		$data   = explode( "/", $vals[0]);
		$giorno = $data[0];
		$mese   = $data[1];
		$anno   = $data[2];
		       
        $time  = explode( ":", $vals[1]);
		$ora   = $time[0];
		$min   = $time[1];
		$sec   = $time[2];
		
        $sensorData[ $anno ][ $mese ][ $giorno ][ $ora ][ $min ] = $vals[($pos-1)];
	}
	 
	/***********************************************************************************/
	
	
}
/***********************************************************************************/

$viewdata = ( !empty($_GET['data']) )? $_GET['data'] : $anno . $mese . $giorno;
$vdata = getValData( $sensorData[substr($viewdata,0,4)], $viewdata );

if ( !is_array($vdata) || empty($vdata) ) $vdata = array("0" => 0);
ksort( $vdata );

/***********************************************************************************/

function getValData( $arr = array(), $data = null ) {
	$vmese    = substr($data,4,2);
	$vgiorno  = substr($data,6,2);
	$vora     = substr($data,8,2);
	
	if ( !empty( $vmese ) ) { $arr = $arr[$vmese]; } 
	else {
		if ( count( $arr ) <= 0) return $arr;
		foreach ($arr as $m => $value) { $_arr[$m]= end($value); }
		$arr = $_arr;
	}
	
	if ( !empty( $vgiorno ) ) { $arr = $arr[$vgiorno]; } 
	else {
		if ( count( $arr ) <= 0) return $arr;
		foreach ($arr as $g => $value) { $_arr[$g]= end($value); }
		$arr = $_arr;
	}
	
	if ( !empty( $vora ) ) {
		if ( count( $arr[$vora] ) <= 0) return $arr;
		foreach ($arr[$vora] as $h => $value) { if (($h%2) == 0) $_arr[$h] = $value; } $arr = $_arr; } 
	else {
		if ( count( $arr ) <= 0) return $arr;
		foreach ($arr as $h => $value) { $_arr[$h] = end($value); }
		$arr = $_arr;
	}
	
	return $arr;
}

/***********************************************************************************/

function getPrevVal( $data = null, $rows = array() ) {
	$vanno    = substr($data,0,4);
	$vmese    = substr($data,4,2);
	$vgiorno  = substr($data,6,2);
	$vora     = substr($data,8,2);
	
	if 		(!empty($vora) )	{ $_formato = "YmdH"; $_data = date('YmdH', mktime ($vora-1,0,0,$vmese,$vgiorno,$vanno)); }
	else if (!empty($vgiorno) ) { $_formato = "Ymd";  $_data = date('Ymd', mktime (0,0,0,$vmese,$vgiorno-1,$vanno)); }
	else if (!empty($vmese) )   { $_formato = "Ym";   $_data = date('Ym', mktime ($vora,0,0,$vmese-1,$vanno)); }
	else                        { $_formato = "Y";    $_data = $data; }
	
	if ( $_data >= getFirstData( $rows, $_formato ) ) { $data = $_data; }
	return $data;
}

/***********************************************************************************/

function getNextVal( $data = null, $rows = array() ) {
	$vanno    = substr($data,0,4);
	$vmese    = substr($data,4,2);
	$vgiorno  = substr($data,6,2);
	$vora     = substr($data,8,2);
	
	if (!empty($vora) )    		{ $_formato = "YmdH"; $_data = date('YmdH', mktime ($vora+1,0,0,$vmese,$vgiorno,$vanno)); }
	else if (!empty($vgiorno) ) { $_formato = "Ymd";  $_data = date('Ymd', mktime (0,0,0,$vmese,$vgiorno+1,$vanno)); }
	else if (!empty($vmese) )   { $_formato = "Ym";   $_data = date('Ym', mktime ($vora,0,0,$vmese+1,$vanno)); }
	else                        { $_formato = "Y";    $_data = $data; }
	
	if ( $_data <= getEndData( $rows, $_formato ) ) { $data = $_data; }
	return $data;
}

/***********************************************************************************/

function getFirstData( $date, $formato ) {
	$lastdate = explode(" ", reset( $date ));
	
    $data   = explode( "/", $lastdate[0]);
	$giorno = $data[0];
	$mese   = $data[1];
	$anno   = $data[2];
	       
    $time  = explode( ":", $lastdate[1]);
	$ora   = $time[0];
	
	if ( $formato == "YmdH" ) { $_date = $anno . $mese . $giorno . $ora; }
	if ( $formato == "Ymd" )  { $_date = $anno . $mese . $giorno; }
	if ( $formato == "Ym" )   { $_date = $anno . $mese; }
	
	return $_date;
}

/***********************************************************************************/

function getEndData( $date, $formato ) {
	$lastdate = explode(" ", end( $date ));
	
    $data   = explode( "/", $lastdate[0]);
	$giorno = $data[0];
	$mese   = $data[1];
	$anno   = $data[2];
	       
    $time  = explode( ":", $lastdate[1]);
	$ora   = $time[0];
	
	if ( $formato == "YmdH" ) { $_date = $anno . $mese . $giorno . $ora; }
	if ( $formato == "Ymd" )  { $_date = $anno . $mese . $giorno; }
	if ( $formato == "Ym" )   { $_date = $anno . $mese; }
	
	return $_date;
}

/***********************************************************************************/

function formatData( $data = null ) {
	$anno    = substr($data,0,4);
	$mese    = substr($data,4,2);
	$giorno  = substr($data,6,2);
	$ora     = substr($data,8,2);
	
	$data = (!empty($giorno))? $giorno . "/" . $mese . "/" . $anno : $mese . "/" . $anno;
	if (!empty($ora)) { $data .= " h: " . $ora;  }
	
	return $data;
}

/***********************************************************************************/

//die("<pre>" . print_r( $vdata,true ) . "</pre>");

?>
	<head>
		<title>InternetOfGreens :: <?php echo $sensor['label']; ?></title>
		<script src="jss/Chart.min.js"></script>
		<!--script src="jss/chart-min.js"></script>
		<script src="jss/jquery-min.js"></script>
		<script>
	    window.onload = function () {   
			var lineChartDataAir = {
				labels : [<?php foreach ($vdata as $k => $v) { echo "\"" . $k . "\","; } ?>],
				datasets : [
					{
						fillColor : "rgba(255,255,255,0)", // sfondo
						strokeColor : "rgba(31,107,193,1)", // linea
						pointColor : "rgba(19,77,187,1)",  // puntini
						pointStrokeColor : "#fff",
						data : [<?php foreach ($vdata as $k => $h) { echo  $h . ","; } ?>]
					}
				]	
			}
		    
		    function respondCanvas(){ 
		    	var c = $('#canvas_humy');
		    	var container = $(c).parent();
		    	c.attr('width', $(container).width() );
		        c.attr('height', (($(container).height()/2)-45) );
				var myLine = new Chart(document.getElementById("canvas_chart").getContext("2d")).Line(lineChartDataAir);
		    }
		    
		    window.onorientationchange = respondCanvas;
			respondCanvas();
	    }
		</script-->
		<script>
		var lineChartData = {
			labels : [<?php foreach ($vdata as $k => $v) { echo "\"" . $k . "\","; } ?>],
			datasets : [
				{
					label: "My First dataset",
					fillColor : "rgba(<?php echo $sensor['fillColor']; ?>)",
					strokeColor : "rgba(<?php echo $sensor['strokeColor']; ?>)",
					pointColor : "rgba(<?php echo $sensor['pointColor']; ?>)",
					pointStrokeColor : "#<?php echo $sensor['pointStrokeColor']; ?>",
					pointHighlightFill : "#<?php echo $sensor['pointHighlightFill']; ?>",
					pointHighlightStroke : "rgba(<?php echo $sensor['pointHighlightStroke']; ?>)",
					data : [<?php foreach ($vdata as $k => $h) { echo  $h . ","; } ?>]
				}
			]

		}

		window.onload = function(){
			var ctx = document.getElementById("canvas_chart").getContext("2d");
			window.myLine = new Chart(ctx).Line(lineChartData, {
				responsive: true
			});
		}
	
		</script>
		<meta name = "viewport" content = "initial-scale = 1, user-scalable = no">
		<meta http-equiv="refresh" content="60; url=<?php echo $_SERVER['REQUEST_URI']; ?>">
		<link href="css/iog.css" rel="stylesheet" type="text/css">
	</head>
	<body>	
		<div id="container">
			<div id="label"><?php echo $sensor['label'] . " " . formatData($viewdata); ?></div>
			<div id="label">
				<button id="pulsante" onclick="window.location.assign('?sensor=<?php echo $_GET['sensor']. "&data=" . getEndData( $rows, "Ym" ); ?>');">Mensile</button>
				<button id="pulsante" onclick="window.location.assign('?sensor=<?php echo $_GET['sensor']. "&data=" . getEndData( $rows, "Ymd" ); ?>');">Giornaliero</button>
				<button id="pulsante" onclick="window.location.assign('?sensor=<?php echo $_GET['sensor']. "&data=" . getEndData( $rows, "YmdH" ); ?>');">Orario</button>
			</div>
			<div style="width:99%; margin: 10px 25px;">
				<div>
					<canvas id="canvas_chart" height="70%"> </canvas>
				</div>
			</div>
			<div id="label" style="text-align: center;">
				<?php if (getPrevVal( $viewdata,$rows ) < $viewdata) { ?><a href="data.php?sensor=<?php echo $_GET['sensor']. "&data=" . getPrevVal( $viewdata,$rows ); ?>"><img src="images/left-arrow-icon.gif" width="32" height="32" border="0" /></a><?php } else { ?><img src="images/left-arrow-icon.gif" width="32" height="32" border="0" /><?php } ?>
				&nbsp; &nbsp; &nbsp; &nbsp; 
				<a href="index.php"><img src="images/home-icon.gif" border="0"></a>
				&nbsp; &nbsp; &nbsp; &nbsp; 
				<?php if (getNextVal( $viewdata,$rows ) > $viewdata) { ?><a href="data.php?sensor=<?php echo $_GET['sensor']. "&data=" . getNextVal( $viewdata,$rows ); ?>"><img src="images/right-arrow-icon.gif" width="32" height="32" border="0" /></a><?php } else { ?><img src="images/right-arrow-icon.gif" width="32" height="32" border="0" /><?php } ?>
			</div>
		</div>
	</body>
</html>
