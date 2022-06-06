package dto;

import spring.dto.user.UserReadDto;
import org.junit.Assert;
import org.junit.Test;

import java.util.ArrayList;

public class UserReadDtoTest {
    @Test
    public void testGettersSetters() {
        var dto = new UserReadDto();

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
        var dto1 = new UserReadDto();
        Assert.assertTrue(dto1.equals(dto1));

        var dto2 = new UserReadDto();
        dto2.setId(5);
        Assert.assertFalse(dto1.equals(dto2));

        dto1.setId(5);
        Assert.assertTrue(dto1.equals(dto2));

        var o = new Object();
        Assert.assertFalse(dto1.equals(o));
    }

    @Test
    public void testHashCode() {
        var dto1 = new UserReadDto();
        var dto2 = new UserReadDto();
        var dto3 = new UserReadDto();
        dto3.setId(5);

        Assert.assertEquals(dto1.hashCode(), dto1.hashCode());
        Assert.assertEquals(dto1.hashCode(), dto2.hashCode());
        Assert.assertTrue(dto1.hashCode() != dto3.hashCode());
        Assert.assertTrue(dto2.hashCode() != dto3.hashCode());
        Assert.assertTrue(dto3.hashCode() == dto3.hashCode());
    }

    @Test
    public void testToString() {
        var dto = new UserReadDto();
        Assert.assertTrue(dto.toString() != null);
        Assert.assertNotNull(dto.toString());
    }
}