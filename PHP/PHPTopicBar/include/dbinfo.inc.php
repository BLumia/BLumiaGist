<?php
// DB Info
	$DB_NAME = "app_phptb"; //sae应用填写 "app_应用名称"，非sae应用填写对应数据库名称
// DB Connection
	if ($ON_SAE) {
		$pdo = new PDO('mysql:host='.SAE_MYSQL_HOST_M.';port='.SAE_MYSQL_PORT.';dbname='.SAE_MYSQL_DB, SAE_MYSQL_USER, SAE_MYSQL_PASS);
		$pdo->query("set names utf8;");
	} else {
		$pdo = new PDO("mysql:host=localhost;dbname=test","root","root");
		$pdo->query("set names utf8;");
	}
?>