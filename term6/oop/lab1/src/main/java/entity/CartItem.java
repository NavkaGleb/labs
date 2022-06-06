package entity;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class CartItem {

    private int userId;
    private int productId;
    private int quantity;

    public CartItem(int userId, int productId, int quantity) {
        this.userId = userId;
        this.productId = productId;
        this.quantity = quantity;
    }
}
