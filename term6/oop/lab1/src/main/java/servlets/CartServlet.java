package servlets;

import com.google.gson.Gson;
import entity.CartItem;
import service.CartService;

import javax.servlet.annotation.WebServlet;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.List;
import java.util.logging.Logger;

@WebServlet("/servlets/cart/*")
public class CartServlet extends HttpServlet {

    private static final Logger log = Logger.getLogger(CartServlet.class.getName());

    @Override
    public void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException {
        log.info("Received get request");

        if (response == null || request == null) {
            throw new IllegalArgumentException("Response/request must not be null.");
        }

        Gson gson = new Gson();
        int userId = getCurrentUserId(request);

        if (request.getPathInfo() == null || request.getPathInfo().equals("/")) {
            log.info("Received request to get all cart items");

            List<CartService.ShoppingCartProductInfo> cartProducts = CartService.getProductsFromCart(userId);

            response
                .getWriter()
                .println(gson.toJson(cartProducts.toArray(new CartService.ShoppingCartProductInfo[]{})));

            return;
        }

        String[] urls = request.getPathInfo().split("/");

        if (urls.length == 2) {
            switch (urls[1]) {
                case "add-to-cart" -> {
                    log.info("Received request to add item to the cart");

                    int productId = Integer.parseInt(request.getParameter("product_id"));
                    int quantity = Integer.parseInt(request.getParameter("quantity"));

                    log.info("Received add product to the cart");

                    CartService.addProductToCart(new CartItem(userId, productId, quantity));
                }
                case "remove-from-cart" -> {
                    log.info("Received request to remove item from the cart");

                    int cartItemId = Integer.parseInt(request.getParameter("id"));

                    CartService.removeShoppingCartItemWithId(cartItemId);
                }
                case "remove-all-items" -> {
                    log.info("Received request to place order");

                    CartService.removeAllItems(userId);
                }
            }
        }
    }

    private static int getCurrentUserId(HttpServletRequest request) {
        String id = null;
        Cookie[] cookies = request.getCookies();

        if (cookies != null) {
            for (Cookie cookie : cookies) {
                if (cookie.getName().equals("user")) {
                    id = cookie.getValue();
                }
            }
        }

        if (id != null) {
            return Integer.parseInt(id);
        }

        else return -1;
    }
}
