<?php 
	session_start(); 
	$_SESSION['UID'] = "BLumiaAPI";
?>
<html>
	<head>
		<title>API Test Page</title>
		<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
		<link rel="stylesheet" href="http://cdn.bootcss.com/bootstrap/3.3.2/css/bootstrap.min.css">
		<link rel="stylesheet" href="http://cdn.bootcss.com/bootstrap/3.3.2/css/bootstrap-theme.min.css">
		<link rel="stylesheet" href="http://cdn.bootcss.com/font-awesome/4.2.0/css/font-awesome.min.css">
    <link rel="stylesheet" type="text/css" media="screen" href="../files/components/bootstrap-markdown/bootstrap-markdown.min.css">
		<script src="http://cdn.bootcss.com/jquery/1.11.2/jquery.min.js"></script>
		<script src="http://cdn.bootcss.com/bootstrap/3.3.2/js/bootstrap.min.js"></script>
  <script src="../files/components/bootstrap-markdown/markdown.js"></script>
  <script src="../files/components/bootstrap-markdown/to-markdown.js"></script>
  <script src="../files/components/bootstrap-markdown/bootstrap-markdown.js"></script>
  <script src="../files/components/jquery/jquery.hotkeys.js"></script>
	</head>
	<body>
		<div class="container">  
			<div class="page-header">
				<h1>API测试页<small>随时变动页面_发帖API测试</small></h1>
			</div>
			</ul>
			<div class="row">
				<form role="form" method=post action='tb_topicPost.php'>
						
					<input class="form-control" type=text ID="title" name="title" value="标题"/>
						
					<textarea data-provide="markdown" name=content rows="6" placeholder="你想要发点什么捏?"></textarea>
			
					<button type="submit" class="btn btn-default">发表</button>
			
				</form>
				<?php if(isset($_SESSION['UID'])) echo "wow"; ?>
			</div>
		</div>
	</body>
</html>