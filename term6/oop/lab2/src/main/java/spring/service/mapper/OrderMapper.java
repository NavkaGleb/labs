package spring.service.mapper;

import spring.dto.order.OrderReadDto;
import spring.persistence.entity.Order;
import org.mapstruct.Mapper;
import spring.dto.order.OrderWriteDto;

@Mapper(componentModel = "spring", config = CommonMapper.class)
public interface OrderMapper extends CommonMapper<OrderReadDto, OrderWriteDto, Order> {
}
