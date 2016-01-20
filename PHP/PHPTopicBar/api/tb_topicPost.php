<?php
/*
	PHP TopicBar_发表主题-API:
		通过POST传递下列参数：
			$name = $_POST['UID']; 用户名
			$password = $_POST['Passwd']; 密码
			$title = $_POST ['title']; 评论正文
			$content = $_POST ['content']; 评论正文
		也可以在确认登陆的情况下，通过POST和SESSION传递下列参数：
			$name=$_SESSION['UID']; 用户名（登陆则不需要密码）
			$title = $_POST ['title']; 评论正文
			$content = $_POST ['content']; 评论正文
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


//send comment
	if(isset($name)){
		$title = $_POST ['title'];
		$content = $_POST ['content'];
		if (get_magic_quotes_gpc ()) {
			$name = stripslashes ( $name);
			$title = stripslashes ( $title);
			$content = stripslashes ( $content );
		}
		$prevContent = utf8_substr(RemoveXSS(strip_tags($content)),0,27);
		$content= RemoveXSS(UBB2HTML(strip_tags($content)));
		$name=$pdo->quote($name);
		$title=$pdo->quote($title);
		$prevContent=$pdo->quote($prevContent);
		$content=$pdo->quote($content);

		$sql=$pdo->prepare("insert into 
						tb_topic(Name,Lastreply_Name,Title,preview_content)
						values($name,$name,$title,$prevContent);");
		if(!$sql->execute()) {
			$pmstate=  "0";
		} else {
			//LAST_INSERT_ID()在这里不知道会不会有问题
			$sql=$pdo->prepare("insert into tb_reply(TopicID,Name,content,Time,ip)
						values(LAST_INSERT_ID(),$name,$content,now(),'127.0.0.1')");
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