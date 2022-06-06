package spring.service.mapper;

import spring.dto.user.UserReadDto;
import spring.dto.user.UserWriteDto;
import spring.persistence.entity.User;
import org.mapstruct.Mapper;

@Mapper(componentModel = "spring", config = CommonMapper.class)
public interface UserMapper extends CommonMapper<UserReadDto, UserWriteDto, User> {
}
