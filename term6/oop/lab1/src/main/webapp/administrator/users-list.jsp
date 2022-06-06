<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <title>Users List</title>
    <%@ include file="/bootstrap-css.html" %>
    <script src="scripts/display-users.js"></script>
</head>
<body>
<%
    String userName = null;
    String type = "";
    Cookie[] cookies = request.getCookies();
    if (cookies != null) {
        for (Cookie cookie : cookies) {
            if (cookie.getName().equals("user")) userName = cookie.getValue();
            else if (cookie.getName().equals("type")) type = cookie.getValue();
        }
    }
    if (userName == null || !type.equals("administrator")) {
        response.sendRedirect("http://localhost:8080/login.jsp");
    }
%>
<div class="container" id="container">
    <br>
    <h3 class="text-center">Products View</h3>
    <hr>
    <a type="button" class="btn btn-primary" href="http://localhost:8080/administrator/home.jsp">Home</a>
    <button class="btn btn-danger" id="logout" role="button">Log out</button>
    <hr>
    <table id="users" class="table table-striped">
        <thead class="thead-dark">
        <th>
            User name
        </th>
        <th>
            Actions
        </th>
        </thead>
    </table>
</div>
</body>
<%@ include file="/bootstrap-js.html" %>
</html>