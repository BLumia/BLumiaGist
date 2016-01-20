<?php session_start(); ?>
<html>
	<head>
		<title>PHP TopicBar</title>
		<?php require_once('./include/globalcssjs.inc.php'); ?>
	</head>
<?php
//Vars
	require_once('./include/globalvar.inc.php');
	require_once('./include/dbinfo.inc.php');
//Prepare
	$p=isset($_GET['p']) ? $_GET['p'] : 0;
	if($p<0){$p=0;}
	$front=intval($p*$PAGE_ITEMS);
	
	$sql=$pdo->prepare("select * from tb_topic order by Lastreply_Time desc limit $front,$PAGE_ITEMS");
	$sql->execute();
	$topicList=$sql->fetchAll();
	$topicCount=count($topicList);
	
	$sql=$pdo->prepare("select * from tb_topic");
	$sql->execute();
	$totalTopic=$sql->fetchAll();
	$totalCount=count($totalTopic);
//Main Code
	require("./template/".$PAGE_TEMPLATE."/index.php");
?>
</html>
