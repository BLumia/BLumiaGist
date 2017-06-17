<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ include file="./components/userLogic.inc.jsp" %>
<%@ include file="./components/dbConn.inc.jsp" %>

<%
	if (!hasLoggedin) {
		response.sendRedirect("./index.jsp");
	}

	String sql="SELECT `book`.`bookid`, `ISBN`, `bookname`, `type`, `bookcase`, `barcode`, `autor`, `price`, `ridcount` `readcount`  FROM `book`, ( SELECT `bookid`, COUNT(`borrowid`) `ridcount` FROM `borrow` GROUP BY `bookid` ) `borrowed` WHERE `borrowed`.`bookid`=`book`.`bookid` ORDER BY `readcount` DESC";
	ps = conn.prepareStatement(sql);
	rs = ps.executeQuery();
%>

<head>
	<meta http-equiv="content-type" content="text/html;charset=utf-8">
	<link rel="stylesheet" href="./static/w3.css">
</head>
<body class="w3-indigo">
	<%@include file="./components/header.jsp" %>
	
	<div class="w3-container w3-light-grey w3-hoverable">
		<h2>借阅排行榜</h2>
		<div class="w3-card">
			<table class="w3-table w3-text-black w3-striped">
			<tr class="w3-red">
				<th>排名</th>
				<th>图书条形码</th>
				<th>图书名称</th>
				<th>图书类型</th>
				<th>书架</th>
				<th>出版社</th>
				<th>作者</th>
				<th>定价</th>
				<th>借阅次数</th>
			</tr>
<%
	int i = 1;
	while(rs.next()) {
		String ISBN = rs.getString("barcode");
		String press = rs.getString("ISBN");
		String bookname = rs.getString("bookname");
		int readCnt = rs.getInt("readcount");
		String booktype = "计算机"; //rs.getString("type");
		String auther = rs.getString("autor");
		float price = rs.getFloat("price");
		String locate = rs.getString("bookcase");
%>
			<tr>
				<td><%=i%></td>
				<td><%=ISBN%></td>
				<td><%=bookname%></td>
				<td><%=booktype%></td>
				<td><%=locate%></td>
				<td><%=press%></td>
				<td><%=auther%></td>
				<td>￥ <%=price%></td>
				<td><%=readCnt%></td>
			</tr>
<%
		i++;
	}
%>
			</table>
		</div>
		<br/>
	</div>
	
	<%@include file="./components/footer.jsp" %>
</body>
<%
	rs.close();
	ps.close();
%>