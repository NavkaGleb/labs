package controller;

import spring.dto.order.OrderReadDto;
import spring.persistence.entity.User;
import spring.rest.controller.OrderController;
import spring.service.OrderService;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnitRunner;

import java.util.List;

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.when;

@RunWith(MockitoJUnitRunner.class)
public class OrderControllerTest {
    @InjectMocks
    private OrderController controller;

    @Mock
    private OrderService orderService;

    @Mock
    private List<OrderReadDto> listDTO;

    @Test
    public void testGetAllOrders() {
        when(orderService.getAll()).thenReturn(listDTO);
        assertEquals(controller.getAll(), listDTO);
    }

    @Test
    public void testGetAllOrdersForUser() {
        User user = new User().setId(1);
        when(orderService.getAllByUserId(user.getId())).thenReturn(listDTO);
        assertEquals(controller.getAllForUser(user.getId()), listDTO);
    }
}