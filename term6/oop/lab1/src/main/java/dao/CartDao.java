package dao;

import entity.CartItem;
import service.CartService;

import java.util.List;

public interface CartDao {

    void removeShoppingCartItemWithId(int id);

    List<CartService.ShoppingCartProductInfo> getProductsFromCart(int userId);

    void addProductToCart(CartItem item);
}
