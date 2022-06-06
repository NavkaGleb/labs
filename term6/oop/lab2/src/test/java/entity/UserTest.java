package entity;

import spring.persistence.entity.User;
import org.junit.Assert;
import org.junit.Test;

import java.util.ArrayList;


public class UserTest {
    @Test
    public void testGettersSetters() {
        var dto = new User();

        dto.setId(1);
        Assert.assertEquals(Integer.valueOf(1), dto.getId());

        dto.setBalance(200);
        Assert.assertEquals(Integer.valueOf(200), dto.getBalance());

        dto.setOrders(new ArrayList<>());
        Assert.assertTrue(dto.getOrders().isEmpty());

        dto.setUsername("user");
        Assert.assertEquals("user", dto.getUsername());
    }

    @Test
    public void testEqual() {
        var dto1 = new User();
        Assert.assertTrue(dto1.equals(dto1));

        var dto2 = new User();
        dto2.setId(5);
        Assert.assertFalse(dto1.equals(dto2));

        dto1.setId(5);
        Assert.assertTrue(dto1.equals(dto2));

        var o = new Object();
        Assert.assertFalse(dto1.equals(o));
    }

    @Test
    public void testHashCode() {
        var dto1 = new User();
        var dto2 = new User();
        var dto3 = new User();
        dto3.setId(5);

        Assert.assertEquals(dto1.hashCode(), dto1.hashCode());
        Assert.assertEquals(dto1.hashCode(), dto2.hashCode());
        Assert.assertTrue(dto1.hashCode() != dto3.hashCode());
        Assert.assertTrue(dto2.hashCode() != dto3.hashCode());
        Assert.assertTrue(dto3.hashCode() == dto3.hashCode());
    }

    @Test
    public void testToString() {
        var dto = new User();
        Assert.assertTrue(dto.toString() != null);
        Assert.assertNotNull(dto.toString());
    }
}