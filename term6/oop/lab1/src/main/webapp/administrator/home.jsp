<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <title>Product List</title>
    <%@ include file="/bootstrap-css.html" %>
    <script src="../products/scripts/products-administrator.js"></script>
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
    <h3 class="text-center">Products List</h3>
    <hr>
    <a type="button" class="btn btn-info" href="http://localhost:8080/administrator/users-list.jsp">Users</a>
    <button class="btn btn-danger" id="logout" role="button">Log out</button>
    <a type="button" class="btn btn-warning float-right" href="http://localhost:8080/products/product-manager.jsp">Add
        product</a>
    <hr>
    <%@ include file="../products/products.html" %>
</div>
</body>
<%@ include file="/bootstrap-js.html" %>
</html>