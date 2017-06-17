<%--
	Do Logout.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ page import="java.util.*" %>
<%
	session.invalidate();
	response.sendRedirect("./index.jsp");
%>

