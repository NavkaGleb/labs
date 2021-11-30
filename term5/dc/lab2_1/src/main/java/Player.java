public class Player {

    public String  firstName;
    public String  lastName;
    public int     age;

    @Override
    public String toString() {
        return firstName + " " + lastName + ": " + age;
    }
}
