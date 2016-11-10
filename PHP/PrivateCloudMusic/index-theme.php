<html>
<meta charset="utf-8">
<title>Private Playlist</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="./w3.css">
<script src="./jquery-3.0.0.min.js"></script>
<style>.w3-sidenav ul li { padding-left: 2px;}</style>

<nav class="w3-sidenav w3-collapse w3-light-grey w3-animate-left w3-card-2" style="z-index: 3; width: 250px; display: none;" id="mySidenav">
<header class="w3-container w3-dark-grey">
	<h2>Folders <a href="javascript:void(0)" onclick="w3_close()" class="w3-right w3-xlarge w3-hide-large w3-closenav" title="close sidenav">×</a></h2>
</header>
<ul class="w3-ul" id="folderlist">
<?php
	$curPath = dirname(__FILE__);
	$fileList = scandir($curPath);
	
	foreach($fileList as $oneFileName) {
		if($oneFileName == "." || $oneFileName == "..") continue;
		$oneFilePath = $curPath."/".$oneFileName;
		if (is_dir($oneFilePath) && file_exists($oneFilePath."/GetPlaylist.php")) {
			echo "<a aim='{$oneFileName}'><li>{$oneFileName}/</li></a>";
		}
	}
?>
</ul>
</nav>
<div class="w3-overlay w3-hide-large w3-animate-opacity" onclick="w3_close()" style="cursor: pointer; display: none;" id="myOverlay"></div>
<div class="w3-main" style="margin-left:250px;">
<header class="w3-container w3-red w3-top">
  <h2><span class="w3-opennav w3-xlarge w3-left w3-hide-large" onclick="w3_open()" id="openNav">&#9776;</span>&nbsp;Private Cloud Music</h2>
</header>
<header class="w3-container w3-yellow"><h2>You can't see me</h2></header>
<ul class="w3-ul w3-hoverable" style="margin-bottom: 120px;" id="playlist">
</ul>
</div>
<div class="w3-bottom" style="z-index: 6;">
	<div class="w3-panel w3-Brown">
		<div class="w3-row">
			<div class="w3-col s3 w3-container w3-left-align">
				<p><span id="curTime">0:00</span></p>
			</div>
			<div class="w3-col s6 w3-container w3-center">
				<p id="nowPlaying">Not playing at all.</p>
				<audio></audio>
			</div>
			<div class="w3-col s3 w3-container w3-right-align">
				<p><span id="totalTime">0:00</span></p>
			</div>
		</div>
		<div class="w3-container">
			<div id="progressbar" class="w3-progress-container">
				<div id="bufferbar" class="w3-progressbar" style="background-color:#AAA; width:0%"></div>
				<div id="timebar" class="w3-progressbar w3-blue" style="width:0%"></div>
			</div>
		</div>
		<div class="w3-container w3-center" style="padding:6px 0px;">
			<button class="w3-btn w3-tiny" id="btn-play">Play</button>
			<button class="w3-btn w3-tiny" id="btn-prev">Prev</button>
			<button class="w3-btn w3-tiny" id="btn-next">Next</button>
			<button class="w3-btn w3-tiny" id="btn-pause">Pause</button>
			<button class="w3-btn w3-tiny" id="btn-loop">Loop</button>
			<button class="w3-btn w3-tiny" id="btn-order">Order</button>
		</div>
	</div>
</div> 

<script>
// formatTime by Yeye Chris
function formatTime(t) {
	var m=Math.floor(t/60),s=Math.round(t-Math.floor(t/60)*60);
	if(s<10)return m+":0"+s;
	else if(s==60)return (m+1)+":00";
	else return m+":"+s;
}

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
				innerHtml += '<a index=' + i + '><li>' + songTitle + '</li></a>';
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
			Player.audio.ontimeupdate = function() {
				$('#curTime').html(formatTime(Player.audio.currentTime));
				$('#totalTime').html(formatTime(Player.audio.duration));
				$('#timebar').css('width',Player.audio.currentTime/Player.audio.duration*100+"%");
				var r=0; // Yeye Chris Begin {
				for(var i=0;i<Player.audio.buffered.length;++i)
				r=r<Player.audio.buffered.end(i)?Player.audio.buffered.end(i):r; // Chris Always Yeye }
				$('#bufferbar').css('width',r/Player.audio.duration*100+"%");
			};
			
			
			$('#progressbar').on('click', function(e) {
				var sr=this.getBoundingClientRect();
				var p=(e.clientX-sr.left)/sr.width;
				Player.audio.currentTime=Player.audio.duration*p;
			});
			
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

<script>
function w3_open() {
    document.getElementById("mySidenav").style.display = "block";
    document.getElementById("myOverlay").style.display = "block";
}
function w3_close() {
    document.getElementById("mySidenav").style.display = "none";
    document.getElementById("myOverlay").style.display = "none";
}
</script>
</html>