package service;

import dao.ProductDao;
import dao.impl.ProductDaoImpl;
import entity.*;

import java.util.List;

public class ProductsService {

    public static void editProduct(Product product) {
        ProductDao productDao = new ProductDaoImpl();
        productDao.editProduct(product);
    }

    public static Product getProductWithId(int id) {
        ProductDao productDao = new ProductDaoImpl();
        return productDao.getProductWithId(id);
    }

    public static void removeProductWithId(int id) {
        ProductDao productDao = new ProductDaoImpl();
        productDao.removeProductWithId(id);
    }

    public static void addProduct(Product product) {
        ProductDao productDao = new ProductDaoImpl();
        productDao.addProduct(product);
    }

    public static ProductType getProductType(int id) {
        ProductDao productDao = new ProductDaoImpl();
        return productDao.getProductType(id);
    }

    public static List<ProductType> getProductTypes() {
        ProductDao productDao = new ProductDaoImpl();
        return productDao.getProductTypes();
    }

    public static List<Product> getProducts() {
        ProductDao productDao = new ProductDaoImpl();
        return productDao.getProducts();
    }
}
