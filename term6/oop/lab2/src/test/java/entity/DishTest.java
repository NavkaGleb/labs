package entity;

import spring.persistence.entity.Dish;
import org.junit.Assert;
import org.junit.Test;

public class DishTest {
    @Test
    public void testGettersSetters() {
        var dto = new Dish();

        dto.setId(1);
        Assert.assertEquals(Integer.valueOf(1), dto.getId());

        dto.setName("Name");
        Assert.assertEquals("Name", dto.getName());

        dto.setDescription("Yes");
        Assert.assertEquals("Yes", dto.getDescription());

        dto.setPrice(100.0);
        Assert.assertEquals(Double.valueOf(100.0), dto.getPrice());

        dto.setOrdered(1);
        Assert.assertEquals(Integer.valueOf(1), dto.getOrdered());
    }

    @Test
    public void testEqual() {
        var dto1 = new Dish();
        Assert.assertTrue(dto1.equals(dto1));

        var dto2 = new Dish();
        dto2.setId(5);
        Assert.assertFalse(dto1.equals(dto2));

        dto1.setId(5);
        Assert.assertTrue(dto1.equals(dto2));

        var o = new Object();
        Assert.assertFalse(dto1.equals(o));
    }

    @Test
    public void testHashCode() {
        var dto1 = new Dish();
        var dto2 = new Dish();
        var dto3 = new Dish();
        dto3.setId(5);

        Assert.assertEquals(dto1.hashCode(), dto1.hashCode());
        Assert.assertEquals(dto1.hashCode(), dto2.hashCode());
        Assert.assertTrue(dto1.hashCode() != dto3.hashCode());
        Assert.assertTrue(dto2.hashCode() != dto3.hashCode());
        Assert.assertTrue(dto3.hashCode() == dto3.hashCode());
    }

    @Test
    public void testToString() {
        var dto = new Dish();
        Assert.assertTrue(dto.toString() != null);
        Assert.assertNotNull(dto.toString());
    }
}