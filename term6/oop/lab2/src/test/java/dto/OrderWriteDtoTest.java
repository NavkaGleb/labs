package dto;

import spring.dto.order.OrderWriteDto;
import org.junit.Assert;
import org.junit.Test;

import java.util.ArrayList;

public class OrderWriteDtoTest {
    @Test
    public void testGettersSetters() {
        var dto = new OrderWriteDto();

        dto.setStatus("Now");
        Assert.assertEquals("Now", dto.getStatus());

        dto.setUsername("name");
        Assert.assertEquals("name", dto.getUsername());

        dto.setDishes(new ArrayList<>());
        Assert.assertTrue(dto.getDishes().isEmpty());
    }

    @Test
    public void testEqual() {
        var dto1 = new OrderWriteDto();
        Assert.assertTrue(dto1.equals(dto1));

        var dto2 = new OrderWriteDto();
        dto2.setUsername("name");
        Assert.assertFalse(dto1.equals(dto2));

        dto1.setUsername("name");
        Assert.assertTrue(dto1.equals(dto2));

        var o = new Object();
        Assert.assertFalse(dto1.equals(o));
    }

    @Test
    public void testHashCode() {
        var dto1 = new OrderWriteDto();
        var dto2 = new OrderWriteDto();
        var dto3 = new OrderWriteDto();
        dto3.setUsername("name");

        Assert.assertEquals(dto1.hashCode(), dto1.hashCode());
        Assert.assertEquals(dto1.hashCode(), dto2.hashCode());
        Assert.assertTrue(dto1.hashCode() != dto3.hashCode());
        Assert.assertTrue(dto2.hashCode() != dto3.hashCode());
        Assert.assertTrue(dto3.hashCode() == dto3.hashCode());
    }

    @Test
    public void testToString() {
        var dto = new OrderWriteDto();
        Assert.assertTrue(dto.toString() != null);
        Assert.assertNotNull(dto.toString());
    }
}