<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <title>Product Cart</title>
    <%@ include file="/bootstrap-css.html" %>
    <script src="scripts/display-cart.js"></script>
</head>
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
    if (userName == null || !type.equals("client")) {
        response.sendRedirect("http://localhost:8080/login.jsp");
    }
%>
<body>
<div class="container" id="container">
    <br>
    <h3 class="text-center">Products View</h3>
    <hr>
    <a type="button" class="btn btn-primary" href="http://localhost:8080/client/home.jsp">Home</a>
    <button class="btn btn-danger" id="logout" role="button">Log out</button>
    <hr>
    <table id="product-cart" class="table table-striped">
        <thead class="thead-dark">
        <th>
            Product name
        </th>
        <th>
            Quantity
        </th>
        <th>
            Full price
        </th>
        <th>
            Actions
        </th>
        </thead>
    </table>

    <p id="total-price">Total price = 0</p>
    <a type="button" id="place-order" class="btn btn-success">Place order</a>
</div>
</body>
<%@ include file="/bootstrap-js.html" %>
</html>