package spring.dto.dish;

import lombok.Value;

@Value
public class DishWriteDto {
    String name;
    Double price;
    String description;
}
