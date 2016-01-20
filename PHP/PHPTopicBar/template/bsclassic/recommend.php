<body>
	<div class="container">  
		<div class="page-header">
			<h1>XXX 吧 <small>描述文字</small></h1>
		</div>
		<ul class="nav nav-tabs">
			<li role="presentation"><a href="index.php">看帖</a></li>
			<li role="presentation" class="active"><a href="recommend.php">精品</a></li>
			<li role="presentation"><a href="#">导航</a></li>
		</ul>
		<div class="row">
			<div class="col-md-9 col-sm-9 col-xs-12">
			<?php for($i=0;$i<$topicCount;$i++) { //topic list ------------ ?>
				<div class="row panel">
					<div class="col-sm-2 col-xs-2">
						<p class="text-center">
						<i class="fa fa-pencil-square-o"></i> <?php echo $topicList[$i]['ReplyCount'];?> 回复
						</p>
					</div>
					<div class="col-sm-6 col-xs-6">
						<?php echo "<a href=\"topic.php?tid=".$topicList[$i]['TopicID']."\">".$topicList[$i]['Title']."</a>"; ?>
						<br/><?php echo $topicList[$i]['preview_content'];?>...
					</div>
					<div class="col-sm-4 col-xs-4">
						<i class="fa fa-pencil-square-o"></i><?php echo $topicList[$i]['Name'];?><br/>
						<i class="fa fa-paper-plane"></i><?php echo $topicList[$i]['Lastreply_Name'];?>
					</div>
				</div>
			<?php } //topic list end --------------------------------------- ?>
				<nav style="text-align: center;">
					<?php require("./template/".$PAGE_TEMPLATE."/controls/pagination.php"); ?>
				</nav>
			</div>
			<?php require("./template/".$PAGE_TEMPLATE."/controls/sidebar.php"); ?>
		</div>
		<div class="row">
			<div class="col-md-12 col-sm-12 col-xs-12">
				<div class="col-sm-9 col-xs-12">
					<?php if (isset($hadLogin) && $hadLogin) { ?>
						<form role="form" method=post action='api/tb_topicPost.php'>
								
							<input class="form-control" type=text ID="title" name="title" value="标题">
								
							<textarea name=content data-provide="markdown" rows="6" placeholder="你想要发点什么捏?"></textarea>
					
							<button type="submit" class="btn btn-default">发表</button>
					
						</form>
					<?php } else { ?>
						<div class="form-group">
							<textarea class="form-control" name=content rows="6" placeholder="必须登陆后才能发表评论"></textarea>
						</div>
						
						<a href="login.php"><button class="btn btn-default">登陆</button></a>
						<a href="register.php"><button class="btn btn-default">注册</button></a>
					<?php } ?>
				</div>
			</div>
		</div>
	</div>
</body>