<?php
	$curPath = dirname(__FILE__);
	$fileList = scandir($curPath);
	$musicList = array();
	
	function GIVEMETHEFUCKINGUTF8($text) {
		return iconv(mb_detect_encoding($text, mb_detect_order(), true), "UTF-8", $text);
    }
	
	function getFileExtension($fileName) {
		$explodeArr = explode('.',$fileName);
		$explodeArr = array_reverse($explodeArr);  
		return strtolower($explodeArr[0]);
	}
	
	foreach($fileList as $oneFileName) {
		if (getFileExtension($oneFileName) == 'mp3') {
			array_push($musicList, rawurlencode(GIVEMETHEFUCKINGUTF8($oneFileName)));
		}
	}
	
	echo json_encode($musicList);