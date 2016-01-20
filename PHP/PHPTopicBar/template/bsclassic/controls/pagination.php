<?php
/*
	Pagination Control Usage:
		Use $_GET['p'] to go to a page, 0 is the first page.
		$totalCount for total item count, and divide 20 is the page count.
		
*/
	$paraStr = "";
	//$paraCount = count($_GET); 
	foreach ($_GET as $key => $value) {
		if ($key == "p") continue;
		$paraStr .= "&" . $key . "=" . $value; 
	} 
?>

<ul class="pagination"> 
	<li><a href="?p=<?php echo ($p-1).$paraStr;?>">&laquo;</a></li>			
	<?php
		for($j=0;$j<=($totalCount/$PAGE_ITEMS);$j++) {
			if($j!=$p) {
				echo "<li><a href=\"?p=".$j.$paraStr."\">".($j+1)."</a></li>";
			} else {
				echo "<li class=\"active\"><a href=\"?p=".$j.$paraStr."\">".($j+1)."</a></li>";
			}
		}
	?>
	<li><a href="?p=<?php echo ($p+1).$paraStr;?>">&raquo;</a></li>
</ul>