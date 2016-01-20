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

	$tid=intval($_GET['tid']);
	if(!isset($_GET['tid'])) {
		echo "没有Topic ID信息";
		exit(0);
	}
	$sql=$pdo->prepare("select * from tb_reply where TopicID=? order by Time asc limit $front,$PAGE_ITEMS");
	$sql->execute(array($tid));
	$replyList=$sql->fetchAll();
	$replyCount=count($replyList);
	
	$sql=$pdo->prepare("select * from tb_reply where TopicID=?");
	$sql->execute(array($tid));
	$totalReply=$sql->fetchAll();
	$totalCount=count($totalReply);
//Main Code
	require("./template/".$PAGE_TEMPLATE."/topic.php");
?>
</html>
