package entity;

public class Admin extends User {
    public Admin(int id, String name, String password) {
        super(id, name, password, UserType.ADMINISTRATOR, false);
    }

    public Admin(int id, String name, String password, boolean blocked) {
        super(id, name, password, UserType.ADMINISTRATOR, blocked);
    }
}
