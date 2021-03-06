<html>
<meta charset="utf-8">
<title>Private Playlist</title>
<script src="./jquery-3.0.0.min.js"></script>
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
		loop: 0,
		audioTag : $('audio'),
		playlist : $('#playlist'),
		nowPlaying : $('#nowPlaying'),
		
		playAtIndex: function(i) {
			Player.audio.pause(); // neccessary ?
			Player.currentIndex = i;
			Player.audio.src = (Player.path + Player.data[i]);
			Player.audio.load(); // neccessary ?
			Player.audio.play();
			window.history.replaceState("","Test Title","#/"+Player.path+Player.data[Player.currentIndex]+"/"); // title seems be fucked.
			Player.nowPlaying.html(decodeURIComponent(Player.data[Player.currentIndex]));
		},
		
		freshData: function() {
			$.ajax({
				type: "GET",
				url: "./" + Player.path + "GetPlaylist.php",
				dataType: "json",
				async : false,
				success: function(data){
					Player.data = eval(data);
				}
			});
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
			Player.audio = Player.audioTag.get(0);
			
			var re = new RegExp("[#][/](.*[/])(.*.mp3)[/]");
			var urlMatch = re.exec(location.href);
			if (urlMatch != null) {
				Player.path = urlMatch[1];
				Player.audio.src = (Player.path + urlMatch[2]);
				Player.audio.play();
				Player.nowPlaying.html(decodeURIComponent(urlMatch[2]));
			}
			
			Player.freshData();
			Player.freshPlaylist();
			
			$('#btn-loop').html(Player.loop == 1 ? 'Loop: on' : 'Loop: off');
		},
 
		ready : function() {
			$('*').on('click', 'button', function() {
				if(Player.data[Player.currentIndex]) Player.nowPlaying.html(decodeURIComponent(Player.data[Player.currentIndex]));
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
				Player.loop = 1 - Player.loop;
				if (Player.loop == 1) {
					Player.audio.loop = true;
					$('#btn-loop').html('Loop: on');
				} else {
					Player.audio.loop = false;
					$('#btn-loop').html('Loop: off');
				}
			});
 
			$('#btn-order').click(function() {
				Player.audio.onended = function() {
					if (Player.loop == 0) {
						$('#btn-next').click();
					}
				};
			});
			
			$('#folderlist a').click(function() {
				Player.path = $(this).attr('aim') + '/';
				Player.freshData();
				Player.freshPlaylist();
			});
		}
	};
 
	Player.init();
	Player.ready();
});
</script>
</html>