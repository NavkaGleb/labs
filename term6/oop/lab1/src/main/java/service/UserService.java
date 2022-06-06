package service;

import dao.UserDao;
import dao.impl.UserDaoImpl;
import entity.*;

import java.util.List;

public class UserService {

    public static void editBlocked(int id, boolean blocked) {
        UserDao userDao = new UserDaoImpl();
        userDao.editBlocked(id, blocked);
    }

    public static void registerUser(User user) {
        UserDao userDao = new UserDaoImpl();
        userDao.registerUser(user);
    }

    public static UserInfo findUser(String name, String password) {
        UserDao userDao = new UserDaoImpl();
        return userDao.findUser(name, password);
    }

    public static List<User> getClientUsers() {
        UserDao userDao = new UserDaoImpl();
        return userDao.getClientUsers();
    }

    public static User getUser(int id) {
        UserDao userDao = new UserDaoImpl();
        return userDao.getUser(id);
    }

    public static class UserInfo {

        public int id;
        public UserType type;
        public boolean blocked;

        public UserInfo(int id, UserType type, boolean blocked) {
            this.id = id;
            this.type = type;
            this.blocked = blocked;
        }
    }
}
