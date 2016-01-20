<?php
/*
	PHP TopicBar_获取回复列表-API:
		通过POST传递下列参数：
			$p = $_POST['p']; 待获取页码
			$tid = $_POST['tid']; 话题编号
	返回值：
		json编码的主题对应的回复
		此过程不判断登陆状态
*/
//Vars
	require_once('../include/globalvar.inc.php');
	require_once('../include/dbinfo.inc.php');
	
//Prepare
	$p=isset($_POST['p']) ? $_POST['p'] : 0;
	if($p<0){$p=0;}
	$front=intval($p*$PAGE_ITEMS);

	$tid=intval($_POST['tid']);
	if(!isset($_POST['tid'])) {
		echo "没有Topic ID信息";
		exit(0);
	}
	$sql=$pdo->prepare("select * from tb_reply where TopicID=? order by Time asc limit $front,$PAGE_ITEMS");
	$sql->execute(array($tid));
	$replyList=$sql->fetchAll();

	echo json_encode($replyList);
?>