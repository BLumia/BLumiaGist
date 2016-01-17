<html>
<head>
<title>Bandwidth Usage Infopage</title>
<style>
#progress{
    width: 100%; 
    height: 20px;
    border:1px solid #ccc;
    border-radius: 5px;
    overflow: hidden;
    box-shadow: 0 0 5px 0px #ddd inset;
}

#progress span {
    display: inline-block;
    height: 100%;
    background: #2989d8; /* Old browsers */
    background: -moz-linear-gradient(45deg, #2989d8 33%, #7db9e8 34%, #7db9e8 59%, #2989d8 60%); /* FF3.6+ */
    background: -webkit-gradient(linear, left bottom, right top, color-stop(33%,#2989d8), color-stop(34%,#7db9e8), color-stop(59%,#7db9e8), color-stop(60%,#2989d8)); /* Chrome,Safari4+ */
    background: -webkit-linear-gradient(45deg, #2989d8 33%,#7db9e8 34%,#7db9e8 59%,#2989d8 60%); /* Chrome10+,Safari5.1+ */
    background: -o-linear-gradient(45deg, #2989d8 33%,#7db9e8 34%,#7db9e8 59%,#2989d8 60%); /* Opera 11.10+ */
    background: -ms-linear-gradient(45deg, #2989d8 33%,#7db9e8 34%,#7db9e8 59%,#2989d8 60%); /* IE10+ */
    background: linear-gradient(45deg, #2989d8 33%,#7db9e8 34%,#7db9e8 59%,#2989d8 60%); /* W3C */
    filter: progid:DXImageTransform.Microsoft.gradient( startColorstr='#2989d8', endColorstr='#2989d8',GradientType=1 ); /* IE6-9 fallback on horizontal gradient */
    background-size: 60px 30px;
    text-align: center;
    color:#fff;
}
</style>
</head>
<body>

<?php
	$VEID = "VEID_Goes_Here";
	$API_KEY = "API_KEY_Goes_Here";
	date_default_timezone_set("PRC");
	
	if(!isset($_GET['token']) || intval($_GET['token']) != 250) {
		echo "<h1>404 You are so Niubi.</h1>";
		exit(404);
	}
	$request = "https://api.kiwivm.it7.net/v1/getServiceInfo?veid={$VEID}&api_key={$API_KEY}";
	$serviceInfo = json_decode (file_get_contents ($request));
	//var_dump($serviceInfo);
	$BandwidthUsedGB = $serviceInfo->data_counter / 1024 / 1024 / 1024;
	$BandwidthTotalGB = $serviceInfo->plan_monthly_data / 1024 / 1024 / 1024;
	//echo $BandwidthTotalGB;
	$BandwidthUsedPercent = $serviceInfo->data_counter / $serviceInfo->plan_monthly_data;
	$BandwidthUsedPercent = round($BandwidthUsedPercent*100,3);
	$BandwidthLeftPercent = ($serviceInfo->plan_monthly_data - $serviceInfo->data_counter) / $serviceInfo->plan_monthly_data;
	$BandwidthLeftPercent = round($BandwidthLeftPercent*100,3);
	
	$CurrectTime = time();
	$BandwidthResetTime = $serviceInfo->data_next_reset;
	$TimeToReset = $BandwidthResetTime - $CurrectTime;
	$TimeToResetDays = ceil($TimeToReset/3600/24);
	
	if(isset($_GET['rls'])) {
		if(intval($_GET['rls']) == 1) {
			$displayRLS = true;
			$request = "https://api.kiwivm.it7.net/v1/getRateLimitStatus?veid={$VEID}&api_key={$API_KEY}";
			$rateLimitStatusInfo = json_decode (file_get_contents ($request));
			if ($rateLimitStatusInfo == NULL) {
				echo "API temporary not avaliable.";
				exit(0);
			}
			$leftReqIn15Min = $rateLimitStatusInfo->remaining_points_15min;
			$leftReqIn24Hours = $rateLimitStatusInfo->remaining_points_24h;
		} else {
			$displayRLS = false;
		}
	} else {
		$displayRLS = false;
	}
?>
<h1><?php echo "Bandwidth Usage at ".$serviceInfo->hostname; ?></h1>
<div id="progress">
    <span style="width: <?php echo $BandwidthLeftPercent; ?>%;"><?php echo $BandwidthLeftPercent; ?>% Left</span>
</div><br/>
<?php
	echo "Bandwidth Used ".$BandwidthUsedPercent."% <br/>".round($BandwidthUsedGB,3)."GB Used, ".round($BandwidthTotalGB,0)."GB in Total.<br/>";
	echo "Today is ".date('Y-m-d',$CurrectTime)." . The bandwidth will be clear at ".date('Y-m-d',$BandwidthResetTime)." , {$TimeToResetDays} day(s) left.<br/>";
	if($displayRLS) {
		echo "You can still query these info {$leftReqIn15Min} times in this 15 mins and {$leftReqIn24Hours} times in this 24 hours.";
	} else {
		echo "<a href='http://".$_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF']."?".$_SERVER['QUERY_STRING']."&rls=1'>See Rate Limit Status</a> <br/>(Please visit this page only when you need to know these info.)";
	}
?>


</body>
