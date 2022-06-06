package dao.impl;

import connection.ConnectionPool;
import dao.CartDao;
import entity.CartItem;
import service.CartService;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Logger;

public class CartDaoImpl implements CartDao {

    private static final String ADD_PRODUCT_TO_SHOPPING_CART_QUERY = "INSERT INTO shopping_cart(user_id, product_id, quantity) VALUES (?, ?, ?)";
    private static final String GET_SHOPPING_CART_ITEM_QUERY = "SELECT shopping_cart.id, quantity FROM shopping_cart WHERE product_id = ? AND user_id = ?";
    private static final String UPDATE_SHOPPING_CART_ITEM_QUERY = "UPDATE shopping_cart SET quantity = ? WHERE id = ?";
    private static final String GET_PRODUCTS_FROM_SHOPPING_CART_QUERY = "SELECT shopping_cart.id, shopping_cart.quantity, p.name, p.price FROM shopping_cart INNER JOIN product p on p.id = shopping_cart.product_id WHERE user_id = ?";
    private static final String REMOVE_SHOPPING_CART_ITEM_WITH_ID_QUERY = "DELETE FROM shopping_cart WHERE shopping_cart.id = ?";

    private static final Logger log = Logger.getLogger(CartDaoImpl.class.getName());

    @Override
    public void removeShoppingCartItemWithId(int id) {
        ConnectionPool cp = ConnectionPool.getConnectionPool();

        try (Connection connection = cp.getConnection()) {
            PreparedStatement prepareStatement = connection.prepareStatement(REMOVE_SHOPPING_CART_ITEM_WITH_ID_QUERY);
            prepareStatement.setInt(1, id);

            if (prepareStatement.executeUpdate() <= 0) {
                log.info("Cannot remove shopping cart item with id " + id);
            }

            cp.releaseConnection(connection);
        } catch (SQLException | InterruptedException e) {
            log.warning("Problems with connection");
        }
    }

    @Override
    public List<CartService.ShoppingCartProductInfo> getProductsFromCart(int userId) {
        log.info("Getting information about products in the shopping cart.");

        List<CartService.ShoppingCartProductInfo> products = new ArrayList<>();
        ConnectionPool cp = ConnectionPool.getConnectionPool();

        try (Connection connection = cp.getConnection()) {
            log.info("Connected to the database.");
            PreparedStatement ps = connection.prepareStatement(GET_PRODUCTS_FROM_SHOPPING_CART_QUERY);
            ps.setInt(1, userId);
            ResultSet rs = ps.executeQuery();

            while (rs.next()) {
                int shoppingCartId = rs.getInt(1);
                int quantity = rs.getInt(2);
                String productName = rs.getString(3);
                int productPrice = rs.getInt(4);

                products.add(new CartService.ShoppingCartProductInfo(shoppingCartId, productName, productPrice * quantity, quantity));
            }

            cp.releaseConnection(connection);
        } catch (SQLException | InterruptedException e) {
            log.warning("Problems with connection");
        }

        return products;
    }

    @Override
    public void addProductToCart(CartItem item) {
        if (item == null) {
            log.info("Cannot add product because it was null.");
            return;
        }

        ConnectionPool cp = ConnectionPool.getConnectionPool();

        try (Connection connection = cp.getConnection()) {
            PreparedStatement prepareStatement = connection.prepareStatement(GET_SHOPPING_CART_ITEM_QUERY);
            prepareStatement.setInt(1, item.getProductId());
            prepareStatement.setInt(2, item.getUserId());
            ResultSet rs = prepareStatement.executeQuery();

            if (rs.next()) {
                log.info("There was product item in the shopping cart, increasing its quantity");

                int id = rs.getInt(1);
                int quantity = rs.getInt(2);

                prepareStatement = connection.prepareStatement(UPDATE_SHOPPING_CART_ITEM_QUERY);
                prepareStatement.setInt(1, item.getQuantity() + quantity);
                prepareStatement.setInt(2, id);
            } else {
                log.info("There was no product item in the shopping cart, adding it to the cart");

                prepareStatement = connection.prepareStatement(ADD_PRODUCT_TO_SHOPPING_CART_QUERY);
                prepareStatement.setInt(1, item.getUserId());
                prepareStatement.setInt(2, item.getProductId());
                prepareStatement.setInt(3, item.getQuantity());
            }

            cp.releaseConnection(connection);

            if (prepareStatement.executeUpdate() <= 0) {
                log.info("Cannot add product to the shopping cart.");
            }
        } catch (SQLException | InterruptedException e) {
            log.warning("Problems with connection");
        }
    }
}
