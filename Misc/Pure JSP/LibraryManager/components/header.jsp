<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<% 
	String __jspName = this.getClass().getSimpleName().replaceAll("_", ".");
%>
<header class="w3-container w3-indigo">
	<div>
		<h1 class="w3-xxxlarge">图书管理系统 <small class="w3-opacity" style="font-size: 45%;"> 当前登陆用户：<%=username%></small></h1>
	</div>
</header>

<nav class="w3-navbar w3-brown">
	<li><a class="w3-hover-black w3-padding-16" href="./readlist.jsp">主页</a></li>
	<li><a class="w3-hover-black w3-padding-16 <% if ("readlist.jsp".equals(__jspName)) out.print("w3-bottombar w3-border-blue"); %>" href="./readlist.jsp">阅读排行榜</a></li>
	<li><a class="w3-hover-black w3-padding-16 <% if ("borrow.jsp".equals(__jspName)) out.print("w3-bottombar w3-border-blue"); %>" href="./borrow.jsp">图书借阅</a></li>
	<li><a class="w3-hover-black w3-padding-16 <% if ("extend.jsp".equals(__jspName)) out.print("w3-bottombar w3-border-blue"); %>" href="./extend.jsp">图书续借</a></li>
	<li class="w3-right"><a href="./logout.jsp" class="w3-padding-16 w3-hover-black w3-hover-text-white">登出系统</a></li>
</nav>
