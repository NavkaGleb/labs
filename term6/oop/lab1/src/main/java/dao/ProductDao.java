package dao;

import entity.Product;
import entity.ProductType;

import java.util.List;

public interface ProductDao {

    void editProduct(Product product);
    Product getProductWithId(int id);

    void removeProductWithId(int id);
    void addProduct(Product product);

    ProductType getProductType(int id);
    List<ProductType> getProductTypes();
    List<Product> getProducts();
}
