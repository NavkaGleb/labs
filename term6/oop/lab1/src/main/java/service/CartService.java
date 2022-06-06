package service;

import dao.CartDao;
import dao.impl.CartDaoImpl;
import entity.CartItem;

import java.util.List;

public class CartService {

    public static void removeAllItems(int userId) {
        List<ShoppingCartProductInfo> items = getProductsFromCart(userId);
        CartDao cartDao = new CartDaoImpl();

        for (ShoppingCartProductInfo item : items) {
            cartDao.removeShoppingCartItemWithId(item.shoppingCartId);
        }
    }

    public static void removeShoppingCartItemWithId(int id) {
        CartDao cartDao = new CartDaoImpl();
        cartDao.removeShoppingCartItemWithId(id);
    }

    public static List<ShoppingCartProductInfo> getProductsFromCart(int userId) {
        CartDao cartDao = new CartDaoImpl();
        return cartDao.getProductsFromCart(userId);
    }

    public static void addProductToCart(CartItem item) {
        CartDao cartDao = new CartDaoImpl();
        cartDao.addProductToCart(item);
    }

    public static class ShoppingCartProductInfo {

        public int shoppingCartId;
        public String productName;
        public int price;
        public int quantity;

        public ShoppingCartProductInfo(int shoppingCartId, String productName, int price, int quantity) {
            this.shoppingCartId = shoppingCartId;
            this.productName = productName;
            this.price = price;
            this.quantity = quantity;
        }
    }
}
