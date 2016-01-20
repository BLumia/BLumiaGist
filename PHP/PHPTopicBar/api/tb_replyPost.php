<?php
/*
	PHP TopicBar_发表评论-API:
		通过POST传递下列参数：
			$name = $_POST['UID']; 用户名
			$password = $_POST['Passwd']; 密码
			$content = $_POST ['content']; 评论正文
			$tid = $_POST['TopicID']; 要评论的Topic
		也可以在确认登陆的情况下，通过POST和SESSION传递下列参数：
			$name=$_SESSION['UID']; 用户名（登陆则不需要密码）
			$content = $_POST ['content']; 评论正文
			$tid = $_POST['TopicID']; 要评论的图
	返回值：
		成功 1
		失败 0
		如登陆则会自动跳回来源页面
		未登录而使用登陆接口则提示非法访问
*/
	session_start();
//Vars
	require_once('../include/functions.inc.php');
	require_once('../include/globalvar.inc.php');
	require_once('../include/dbinfo.inc.php');
	require_once('../include/loginstate.inc.php');
	$tid=$_POST['TopicID'];

//send comment
	if(isset($name)){
		$content = $_POST ['content'];
		if (get_magic_quotes_gpc ()) {
			$name = stripslashes ( $name);
			$tid = stripslashes ( $tid);
			$content = stripslashes ( $content );
		}
		//$content= SafeHTML(UBB2HTML($content));
		$content= RemoveXSS(UBB2HTML(strip_tags($content)));
		$name=$pdo->quote($name);
		$tid=$pdo->quote($tid);
		$content=$pdo->quote($content);
		
		$sql=$pdo->prepare("insert into tb_reply(TopicID,Name,content,Time,ip)
						values($tid,$name,$content,now(),'127.0.0.1')");
		if(!$sql->execute()){
			$pmstate=  "0";
		} else {
			$sql=$pdo->prepare("UPDATE `$DB_NAME`.`tb_topic` SET `Lastreply_Time` = now(), `Lastreply_Name`= $name, `ReplyCount` = `ReplyCount`+1 WHERE `tb_topic`.`TopicID` = $tid");
			$sql->execute();
			$pmstate=  "1";
		}
	}
	echo $pmstate;
	if ($hadLogin){
		echo "<script language='javascript'>\n";
		echo "history.go(-1);\n";
		echo "</script>";
	}
?>