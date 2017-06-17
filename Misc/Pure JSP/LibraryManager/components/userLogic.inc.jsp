<%--
	Logic about checking if user is already logged in or not.
	Global variable $hasLoggedin and $username is the result.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
	boolean hasLoggedin = false;
	String username = "default";

	if (session.getAttribute("username")!=null) {
		hasLoggedin = true;
		username = (String)session.getAttribute("username");
	}
%>