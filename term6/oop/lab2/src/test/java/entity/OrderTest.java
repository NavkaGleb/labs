package entity;

import spring.persistence.entity.Order;
import spring.persistence.entity.User;
import org.junit.Assert;
import org.junit.Test;

import java.util.ArrayList;

public class OrderTest {
    @Test
    public void testGettersSetters() {
        var dto = new Order();

        dto.setId(1);
        Assert.assertEquals(Integer.valueOf(1), dto.getId());

        dto.setDishes(new ArrayList<>());
        Assert.assertTrue(dto.getDishes().isEmpty());

        dto.setUser(new User().setUsername("Alex"));
        Assert.assertEquals("Alex", dto.getUser().getUsername());

        dto.setStatus(Order.OrderStatus.ACCEPTED);
        Assert.assertEquals(Order.OrderStatus.ACCEPTED, dto.getStatus());
    }

    @Test
    public void testEqual() {
        var dto1 = new Order();
        Assert.assertTrue(dto1.equals(dto1));

        var dto2 = new Order();
        dto2.setId(5);
        Assert.assertFalse(dto1.equals(dto2));

        dto1.setId(5);
        Assert.assertTrue(dto1.equals(dto2));

        var o = new Object();
        Assert.assertFalse(dto1.equals(o));
    }

    @Test
    public void testHashCode() {
        var dto1 = new Order();
        var dto2 = new Order();
        var dto3 = new Order();
        dto3.setId(5);

        Assert.assertEquals(dto1.hashCode(), dto1.hashCode());
        Assert.assertEquals(dto1.hashCode(), dto2.hashCode());
        Assert.assertTrue(dto1.hashCode() != dto3.hashCode());
        Assert.assertTrue(dto2.hashCode() != dto3.hashCode());
        Assert.assertTrue(dto3.hashCode() == dto3.hashCode());
    }

    @Test
    public void testToString() {
        var dto = new Order();
        Assert.assertTrue(dto.toString() != null);
        Assert.assertNotNull(dto.toString());
    }
}