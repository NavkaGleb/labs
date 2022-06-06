package dto;

import spring.dto.order.OrderDishDto;
import org.junit.Assert;
import org.junit.Test;

public class OrderDishDtoTest {
    @Test
    public void testGettersSetters() {
        var dto = new OrderDishDto();

        dto.setQuantity(1);
        Assert.assertEquals(Integer.valueOf(1), dto.getQuantity());

        dto.setDishName("Naggets");
        Assert.assertEquals("Naggets", dto.getDishName());
    }

    @Test
    public void testEqual() {
        var dto1 = new OrderDishDto();
        Assert.assertTrue(dto1.equals(dto1));

        var dto2 = new OrderDishDto();
        dto2.setQuantity(5);
        Assert.assertFalse(dto1.equals(dto2));

        dto1.setQuantity(5);
        Assert.assertTrue(dto1.equals(dto2));

        var o = new Object();
        Assert.assertFalse(dto1.equals(o));
    }

    @Test
    public void testHashCode() {
        var dto1 = new OrderDishDto();
        var dto2 = new OrderDishDto();
        var dto3 = new OrderDishDto();
        dto3.setQuantity(5);

        Assert.assertEquals(dto1.hashCode(), dto1.hashCode());
        Assert.assertEquals(dto1.hashCode(), dto2.hashCode());
        Assert.assertTrue(dto1.hashCode() != dto3.hashCode());
        Assert.assertTrue(dto2.hashCode() != dto3.hashCode());
        Assert.assertTrue(dto3.hashCode() == dto3.hashCode());
    }

    @Test
    public void testToString() {
        var dto = new OrderDishDto();
        Assert.assertTrue(dto.toString() != null);
        Assert.assertNotNull(dto.toString());
    }
}