package dao;

import entity.User;
import service.UserService;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

public interface UserDao {

    void editBlocked(int id, boolean blocked);
    void registerUser(User user);

    UserService.UserInfo findUser(String name, String password);
    List<User> getClientUsers();
    User getUser(int id);
    User getUserFromResultSet(ResultSet rs) throws SQLException;
}
