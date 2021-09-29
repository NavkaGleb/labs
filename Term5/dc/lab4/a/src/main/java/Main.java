import java.io.File;
import java.io.IOException;
import java.util.Objects;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.function.Predicate;

public class Main {

    private static final ReadWriteLock  readWriteLock = new ReentrantReadWriteLock();
    private static final File           file = new File("data.txt");

    public static void main(String[] args) {
        try {
            if (file.delete()) {
                System.out.println("File deleted");
            }

            if (file.createNewFile()) {
                System.out.println("File created");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        var writer = new DataWriter(file, readWriteLock);

        var fullNamePredicate = new Predicate<Person>() {

            @Override
            public boolean test(Person person) {
                int id = ThreadLocalRandom.current().nextInt(writer.getPersonId());

                String firstName = "first_name_" + id;
                String lastName = "last_name_" + id;

                return person.getFirstName().equals(firstName) && person.getLastName().equals(lastName);
            }
        };

        var phonePredicate = new Predicate<Person>() {

            @Override
            public boolean test(Person person) {
                String phone = String.valueOf(ThreadLocalRandom.current().nextInt(writer.getPersonId())).repeat(5);

                return person.getPhone().equals(phone);
            }
        };

        new Thread(writer, "Writer").start();
        new Thread(new DataReader(file, readWriteLock, fullNamePredicate), "Reader-1").start();
        new Thread(new DataReader(file, readWriteLock, phonePredicate), "Reader-2").start();
    }
}
