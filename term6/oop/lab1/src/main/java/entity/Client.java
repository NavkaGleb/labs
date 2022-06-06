package entity;

import lombok.Getter;
import lombok.Setter;

import java.util.ArrayList;

@Getter
@Setter
public class Client extends User {

    private ArrayList<Product> shoppingCart;

    public Client(int id, String name, String password) {
        super(id, name, password, UserType.CLIENT, false);
        shoppingCart = new ArrayList<>();
    }

    public Client(int id, String name, String password, boolean blocked) {
        super(id, name, password, UserType.CLIENT, blocked);
        shoppingCart = new ArrayList<>();
    }
}
