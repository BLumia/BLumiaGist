<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ include file="./components/userLogic.inc.jsp" %>
<%@ include file="./components/dbConn.inc.jsp" %>

<%
	if (!hasLoggedin) {
		response.sendRedirect("./index.jsp");
	}
	
	// Init
	String name="default";
	String sex="男";
	String usertype="学生";
	String identype="身份证";
	String idnumber="22010420432465";
	int readerid = 0;
	int canBorrowCnt = 1;
	boolean gotSearchResult = false;

	if(request.getMethod().equals("POST")) {
		//request.setCharacterEncoding("UTF-8"); 
		String sql="SELECT * FROM `reader` WHERE `paperNumber`=?";
		ps = conn.prepareStatement(sql);
		String userid = request.getParameter("idcode");
		ps.setString(1,userid);
		rs = ps.executeQuery();
		rs.last();
		
		if (rs.getRow() == 1) {
			name = rs.getString("name");
			sex = rs.getInt("sex") == 1 ? "男" : "女";
			usertype = rs.getString("vocation");
			idnumber = rs.getString("paperNumber");
			readerid = rs.getInt("readerid");
			gotSearchResult = true;
		}
		
		sql="SELECT `book`.`bookid`, `ISBN`, `bookname`, `type`, `bookcase`, `barcode`, `price`, `borrowTime`, `backTime` FROM `book`, ( SELECT `bookid`, `readerid`, `borrowTime`, `backTime` FROM `borrow` GROUP BY `bookid` ) `borrowed` WHERE `borrowed`.`bookid`=`book`.`bookid` AND `borrowed`.`readerid` = ?";
		ps = conn.prepareStatement(sql);
		ps.setInt(1,readerid);
		rs = ps.executeQuery();
	}
%>

<head>
	<meta http-equiv="content-type" content="text/html;charset=utf-8">
	<link rel="stylesheet" href="./static/w3.css">
</head>
<body class="w3-indigo">
	<%@include file="./components/header.jsp" %>
	
	<div class="w3-container w3-light-grey w3-hoverable">
		<h2>图书借阅</h2>
		<div class="w3-card w3-row">
			<div class="w3-col s3"><h6><b>读者验证</b></h6></div>
			<div class="w3-col s3" style="text-align: right;"><h6>读者条形码：</h6></div>
			<form method="post">
			<div class="w3-col s4"><input class="w3-input w3-border" type="number" name="idcode" placeholder="2012022400001" value="411104199301284554" required></div>
			<div class="w3-col s2"><button class="w3-btn w3-blue" style="margin: 3px 9px;" type="submit">确定</button></div>
			</form>
		</div>
<% 
	if(gotSearchResult) {
%>
		<div class="w3-card w3-row">
			<div class="w3-col s4"><b>姓&nbsp;&nbsp;&nbsp;&nbsp;名：</b><%=name%></div>
			<div class="w3-col s4"><b>性&nbsp;&nbsp;&nbsp;&nbsp;别：</b><%=sex%></div>
			<div class="w3-col s4"><b>读者类型：</b><%=usertype%></div>
			<div class="w3-col s4"><b>证件类型：</b><%=identype%></div>
			<div class="w3-col s4"><b>证件号码：</b><%=idnumber%></div>
			<div class="w3-col s4"><b>可借数量：</b><%=canBorrowCnt%></div>
		</div>
		<div class="w3-card">
			<table class="w3-table w3-text-black w3-striped">
			<tr class="w3-red">
				<th>图书名称</th>
				<th>借阅时间</th>
				<th>应还时间</th>
				<th>出版社</th>
				<th>书架</th>
				<th>定价</th>
			</tr>
<%
		int i = 1;
		while(rs.next()) {
			String ISBN = rs.getString("barcode");
			String press = rs.getString("ISBN");
			String bookname = rs.getString("bookname");
			String booktype = "计算机"; //rs.getString("type");
			float price = rs.getFloat("price");
			String locate = rs.getString("bookcase");
			String borrowTime = rs.getString("borrowTime");
			String backTime = rs.getString("backTime");
%>
			<tr>
				<td><%=bookname%></td>
				<td><%=borrowTime%></td>
				<td><%=backTime%></td>
				<td><%=press%></td>
				<td><%=locate%></td>
				<td>￥ <%=price%></td>
			</tr>
<%
			i++;
		}
	} else {
		out.print("<br/>填写读者条形码并按下确定以查询<br/><br/>");
	}
%>
			</table>
		</div>
		<br/>
	</div>
	
	<%@include file="./components/footer.jsp" %>
</body>
<%
	if(request.getMethod().equals("POST")) {
		rs.close();
		ps.close();
	}
%>