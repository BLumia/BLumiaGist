<%--
	Logic about connect to database.
	Global variables: $conn, $ps and $rs.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ page import="java.sql.Connection" %>
<%@ page import="java.sql.DriverManager" %>
<%@ page import="java.sql.SQLException" %>
<%@ page import="java.sql.Statement" %>
<%@ page import="java.sql.PreparedStatement" %>
<%@ page import="java.sql.ResultSet" %>

<%
	String DB_URL = "jdbc:mysql://127.0.0.1:3306/book"; // Database name: book
	String DB_USER= "root";
	String DB_PWD = "root";
	try {
		Class.forName("com.mysql.jdbc.Driver");
	} 
	catch (ClassNotFoundException e) { 
		e.printStackTrace();
	}
	
	Connection conn = DriverManager.getConnection(DB_URL, DB_USER, DB_PWD);
	PreparedStatement ps = null;
	ResultSet rs = null; 
%>