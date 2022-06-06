package spring.service.mapper;

import spring.dto.dish.DishReadDto;
import spring.dto.dish.DishWriteDto;
import spring.persistence.entity.Dish;
import org.mapstruct.Mapper;

@Mapper(componentModel = "spring", config = CommonMapper.class)
public interface DishMapper extends CommonMapper<DishReadDto, DishWriteDto, Dish> {
}
