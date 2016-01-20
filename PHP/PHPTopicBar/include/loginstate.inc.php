<?php
//login state
	if (isset($_SESSION['UID'])){
		$name=$_SESSION['UID'];
		$password="phpTopicBar";//don't care about this when user is online
		$hadLogin = true;
	} else {
		if (isset($_POST['UID'])){
			$name=$_POST['UID'];
			$password=md5($_POST['Passwd']);
			$hadLogin=false;
		} else {
			echo $_SESSION['UID'];
			echo "illagel access!!";
			exit(1);
		}
	}
//User Exist
	if ($hadLogin) {
		$sql=$pdo->prepare("select * from tb_users where Name=?");
		$sql->execute(array($name));
	} else {
		$sql=$pdo->prepare("select * from tb_users where Name=? and Password=?");
		$sql->execute(array($name,$password));
	}
	$res=$sql->fetchAll();
	if(count($res)==0){
		echo "User_Information_Error";
		exit(1);
	}
?>