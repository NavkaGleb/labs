package entity;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class ProductType {

    private final int id;
    private String name;
    private String description;

    public ProductType(int id, String name, String description) {
        this.id = id;
        this.name = name;
        this.description = description;
    }
}
