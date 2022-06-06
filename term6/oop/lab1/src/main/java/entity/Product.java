package entity;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class Product {

    private int id;
    private String name;
    private int price;
    private String description;
    private ProductType type;

    public Product(String name, int price, String description, ProductType type) {
        this.name = name;
        this.price = price;
        this.description = description;
        this.type = type;
    }

    public Product(int id, String name, int price, String description, ProductType type) {
        this.id = id;
        this.name = name;
        this.price = price;
        this.description = description;
        this.type = type;
    }
}
