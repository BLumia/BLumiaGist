<body>
	<div class="container">  
		<div class="page-header">
			<h1>XXX 吧 <small>描述文字</small></h1>
		</div>
		<ul class="nav nav-tabs">
			<li role="presentation" class="active"><a href="index.php">看帖</a></li>
			<li role="presentation"><a href="recommend.php">精品</a></li>
			<li role="presentation"><a href="#">导航</a></li>
		</ul>
		<div class="row">
			<div class="col-md-9 col-sm-9 col-xs-12">
				<ol class="breadcrumb">
					<li class="active">【标题要长】帖子标题一定不能超过二十个字 </li>
				</ol>
			<?php 
				for($i=0;$i<$replyCount;$i++) { //reply list ------------
					$lou=$i+1+$p*$PAGE_ITEMS;
			?>
				<div class="row panel">
					<div class="col-sm-3 col-xs-3">
						<p class="text-center">
						<span class="avatar"><img src="http://himg.baidu.com/sys/portraitl/item/71c642616c6c616e63654c6f766572d40e" style="width: 64px; height: 64px"></span><br/>
						<i class="fa fa-user"></i>
						<?php echo "<a href=\"/userinfo.php?UID=".$replyList[$i]['Name']."\">".$replyList[$i]['Name']."</a>"; ?>
						</p>
					</div>
					<div class="col-sm-9 col-xs-9 reply-content">
						<?php echo $replyList[$i]['content']; ?>
						<p class="text-muted reply-info"><?php echo "#$lou 楼 ".$replyList[$i]['Time']; ?></p>
					</div>
				</div>
			<?php } //reply list end -------------------------------------?>
				<div id="insert">
				</div>
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
						<form role="form" method=post action='api/tb_replyPost.php'>
						
						<input class="form-control" type=hidden ID="TopicID" name="TopicID" value="<?php echo $tid;?>" readonly>
						
						<textarea class="form-control" data-provide="markdown" name=content rows="6" placeholder="说说你的想法吧~"></textarea>
			
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
<script> 
function insert(){ 
	//<button onclick="insert()">Insert Test</button> 
	$.ajax({
		type: 'POST',
		url: 'api/tb_getPostList_json.php',
		data: {'p':0,'tid':1},
		dataType:'json',
		beforeSend:function(){
			NProgress.start();
		},
		success:function(json){
			var insertText = json[1]['Name']; 
			console.log(json);
			console.log("我被执行过");
			document.getElementById("insert").innerHTML = document.getElementById("insert").innerHTML+insertText; 
		},
		complete:function(){ 
			NProgress.done();
		},
		error:function(jqXHR, textStatus, errorThrown){
			console.log("error: " + errorThrown);
		}
	});
	
	//var insertText = "<table><tr><td>any thing</td></tr></table>"; 
	//document.getElementById("insert").innerHTML = document.getElementById("insert").innerHTML+insertText; 
	//NProgress.done();
} 
</script> 
</body>
