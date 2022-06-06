package spring.rest.controller;

import spring.dto.user.UserReadDto;
import spring.service.OrderService;
import spring.service.UserService;
import spring.service.mapper.UserMapper;
import lombok.RequiredArgsConstructor;
import lombok.experimental.FieldDefaults;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import javax.annotation.security.RolesAllowed;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

@RestController
@RequestMapping("/users")
@RequiredArgsConstructor
@FieldDefaults(makeFinal = true)
@CrossOrigin("http://localhost:4200")
public class UserController {

    private UserService userService;
    private OrderService orderService;
    private UserMapper userMapper;

    @GetMapping("/{username}")
    @RolesAllowed({"USER", "ADMIN"})
    public ResponseEntity<UserReadDto> findByUsername(@PathVariable String username) {
        var optionalUser = userService.findByUsername(username)
                .map(userMapper::entityToDto);
        if (optionalUser.isPresent()) {
            var user = optionalUser.get();
            optionalUser = Optional.of(user.setOrders(orderService.getAllByUserId(user.getId())));
        }
        return ResponseEntity.of(optionalUser);
    }

    @PatchMapping("/{username}")
    @RolesAllowed({"ADMIN", "USER"})
    public ResponseEntity<UserReadDto> updateBalance(@PathVariable String username, @RequestParam Integer balance) {
        var optionalUser = userService.findByUsername(username);
        if (optionalUser.isPresent()) {
            var user = optionalUser.get();
            userService.updateUser(user.setBalance(balance));
            return ResponseEntity.ok(userMapper.entityToDto(user)
                    .setOrders(orderService.getAllByUserId(user.getId()))
            );
        } else {
            return ResponseEntity.notFound().build();
        }
    }

    @GetMapping()
    @RolesAllowed("ADMIN")
    public ResponseEntity<List<UserReadDto>> findAll() {
        var users = userService.findAll().
                stream().
                map(userMapper::entityToDto).
                collect(Collectors.toList());
        return ResponseEntity.ok(users);
    }
}
