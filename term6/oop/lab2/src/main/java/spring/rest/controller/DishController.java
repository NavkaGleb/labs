package spring.rest.controller;

import spring.dto.dish.DishReadDto;
import spring.service.mapper.DishMapper;
import lombok.RequiredArgsConstructor;
import lombok.experimental.FieldDefaults;
import lombok.extern.slf4j.Slf4j;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import spring.service.DishService;

import javax.annotation.security.RolesAllowed;
import java.util.List;
import java.util.stream.Collectors;

@Slf4j
@RestController
@RequestMapping(path = "/dishes")
@RequiredArgsConstructor
@FieldDefaults(makeFinal = true)
@CrossOrigin("http://localhost:4200")
public class DishController {

    private DishMapper dishMapper;
    private DishService dishService;

    @GetMapping
    @RolesAllowed({"USER", "ADMIN"})
    public List<DishReadDto> findAll() {
        log.info("Retrieving all wares presented in the menu");
        return dishService.findAll().stream()
                .map(dishMapper::entityToDto)
                .collect(Collectors.toList());
    }
}
