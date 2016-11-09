<html>
<meta charset="utf-8">
<title>Private Playlist</title>
<script src="http://cdn.bootcss.com/jquery/3.1.1/jquery.min.js"></script>
<ul id="folderlist">
<?php
	$curPath = dirname(__FILE__);
	$fileList = scandir($curPath);
	
	foreach($fileList as $oneFileName) {
		if($oneFileName == "." || $oneFileName == "..") continue;
		$oneFilePath = $curPath."/".$oneFileName;
		if (is_dir($oneFilePath) && file_exists($oneFilePath."/GetPlaylist.php")) {
			echo "<li><a aim='{$oneFileName}'>{$oneFileName}/</a></li>";
		}
	}
?>
</ul>
<ol id="playlist">
</ol>
Now Playing: <span id="nowPlaying"></span><br/>
<audio controls></audio><br/>
<button id="btn-play">Play</button>
<button id="btn-prev">Prev</button>
<button id="btn-next">Next</button>
<button id="btn-pause">Pause</button>
<button id="btn-loop">Loop</button>
<button id="btn-order">Order</button>

<script>
$(function() {
	var Player = {
		path : 'Waterflame/',
		data : null,
		audio : null,
		currentIndex : -1,
		audioTag : $('audio'),
		playlist : $('#playlist'),
		nowPlaying : $('#nowPlaying'),
		
		playAtIndex: function(i) {
			Player.audio.src = Player.path + Player.data[i];
			Player.audio.play();
			Player.currentIndex = i;
			Player.nowPlaying.html(decodeURIComponent(Player.data[Player.currentIndex]));
		},
 
		freshPlaylist : function() {
			var innerHtml = '';
			var len = Player.data.length;
			var songTitle = '';
			for (var i = 0; i < len; i++) {
				songTitle = decodeURIComponent(Player.data[i]);
				innerHtml += '<li><a index=' + i + '>' + songTitle + '</a></li>';
			}
			Player.playlist.html(innerHtml);
			
			// everytime after update playlist dom, do this (why?)
			$('#playlist a').click(function() {
				Player.playAtIndex($(this).attr('index'));
			});
		},
 
		init : function() {
			// Should load via ajax
			$.ajax({
				type: "GET",
				url: "./SampleFolder/GetPlaylist.php",
				dataType: "json",
				async : false,
				success: function(data){
					Player.data = eval(data);
				}
			});
			Player.freshPlaylist();
		},
 
		ready : function() {
			Player.audio = Player.audioTag.get(0);

			$('*').on('click', 'button', function() {
				Player.nowPlaying.html(decodeURIComponent(Player.data[Player.currentIndex]));
			});
 
			$('#btn-play').click(function() {
				Player.audio.play();
				if (Player.currentIndex == -1) {
					$('#btn-next').click();
				}
			});
 
			$('#btn-pause').click(function() {
				Player.audio.pause();
			});
 
			$('#btn-next').click(function() {
				if (Player.currentIndex == -1) {
					Player.currentIndex = 0;
				} else if (Player.currentIndex == (Player.data.length - 1)) {
					Player.currentIndex = 0;
				} else {
					Player.currentIndex++;
				}
				Player.audio.src = Player.path + Player.data[Player.currentIndex];
				Player.audio.play();
			});
 
			$('#btn-prev').click(function() {
				if (Player.currentIndex == -1) {
					Player.currentIndex = 0;
				} else if (Player.currentIndex == 0) {
					Player.currentIndex = (Player.data.length - 1);
				} else {
					Player.currentIndex--;
				}
				Player.audio.src = Player.path + Player.data[Player.currentIndex];
				Player.audio.play();
			});
 
			$('#btn-loop').click(function() {
				Player.audio.onended = function() {
					Player.audio.load();
					Player.audio.play();
				};
			});
 
			$('#btn-order').click(function() {
				Player.audio.onended = function() {
					$('#btn-next').click();
				};
			});
 
			$('#btn-random').click(function() {
				Player.audio.onended = function() {
					var i = parseInt((Player.data.length - 1) * Math.random());
					Player.playAtIndex(i);
				};
			});
 
			$('#playlist a').click(function() {
				Player.playAtIndex($(this).attr('index'));
			});
			
			$('#folderlist a').click(function() {
				Player.path = $(this).attr('aim') + '/';
				$.ajax({
					type: "GET",
					url: "./" + Player.path + "GetPlaylist.php",
					dataType: "json",
					async : false,
					success: function(data){
						Player.data = eval(data);
						Player.freshPlaylist();
					}
				});
			});
		}
	};
 
	Player.init();
	Player.ready();
});
</script>
</html>
