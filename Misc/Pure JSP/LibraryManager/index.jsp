<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ include file="./components/userLogic.inc.jsp" %>
<%@ include file="./components/dbConn.inc.jsp" %>

<%
	String userid = request.getParameter("userid");
	String password = request.getParameter("password");
	String hint = "";
	
	// Check user exist and password.
	if(request.getMethod().equals("POST")) {
		String sql = "SELECT * FROM manager WHERE name = ? AND password = ?";
		ps = conn.prepareStatement(sql);
		ps.setString(1,userid);
		ps.setString(2,password);
		rs = ps.executeQuery();
		rs.last();
				
		if (rs.getRow() == 1) {
			username = rs.getString("name");
			session.setAttribute("username", username);
			hasLoggedin = true;
		} else {
			hint = "<span class='w3-text-red'>用户名或密码错误<br/>提示: 用户名: <b>yue</b> 密码: <b>123</b><br/><br/></span>";
		}
	}
	
	if (hasLoggedin) { 
		response.sendRedirect("./readlist.jsp");
	}
%>
<head>
	<meta http-equiv="content-type" content="text/html;charset=utf-8">
	<link rel="stylesheet" href="./static/w3.css">
</head>
<body class="w3-indigo">
	<header class="w3-container w3-indigo w3-padding-64 w3-padding-jumbo">
		<h1 class="w3-xxxlarge w3-padding-16"></h1>
	</header>
	<div class="w3-row w3-light-grey">
		<div class="w3-col s6">&nbsp;</div>
		<div class="w3-col s6">
			<form class="w3-container w3-padding-16" style="max-width:500px;" method="post">
				<h1>图书管理系统</h1>
				<%=hint%>
				<label>管理员名称</label>
				<input name="userid" class="w3-input" type="text">
				<br/>
				<label>管理员密码</label>
				<input name="password" class="w3-input" type="text">
				<br/>
				<button class="w3-btn w3-blue" type="submit">登陆</button>
				<button class="w3-btn w3-grey" type="reset">重置</button>
			</form>
		</div>
	</div>
	<footer class="w3-container w3-center ">
		<p class="w3-opacity">Copyright &copy; 2017 - 图书管理系统<br/>
		JSP 课程设计作业</p>
	</footer>
</body>