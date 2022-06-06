package service;


import spring.persistence.entity.User;
import spring.persistence.repository.UserRepository;
import spring.service.UserService;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnitRunner;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.*;

@RunWith(MockitoJUnitRunner.class)
public class UserServiceTest {
    @InjectMocks
    private UserService service;

    @Mock
    private UserRepository userRepository;

    @Mock
    private List<User> list;

    @Mock
    private User mockUser;

    @Test
    public void testFindAll() {
        when(userRepository.findAll()).thenReturn(list);
        assertEquals(service.findAll(), list);
    }

    @Test
    public void testFindByUserName() {
        when(userRepository.findByUsername("user")).thenReturn(Optional.of(mockUser));
        assertEquals(service.findByUsername("user"), Optional.of(mockUser));
    }

    @Test
    public void testUpdateUser() {
        User user = new User();
        user.setId(2);
        user.setUsername("user");
        user.setOrders(new ArrayList<>());
        user.setBalance(200);
        when(userRepository.save(user)).thenReturn(mockUser);
        assertEquals(service.updateUser(user), mockUser);
    }
}

