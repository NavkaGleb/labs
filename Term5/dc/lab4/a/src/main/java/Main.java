import java.io.File;
import java.io.IOException;
import java.util.Objects;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

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

        new Thread(new DataWriter(file, readWriteLock), "Writer").start();
        new Thread(new DataReader(file, readWriteLock, (Person person) -> Objects.equals(person.getFirstName(), "first_name_2")), "Reader-1").start();
        new Thread(new DataReader(file, readWriteLock, (Person person) -> Objects.equals(person.getPhone(), "33333")), "Reader-2").start();


//        new Thread(new DataFinder(file, rwLock, recordsNumber), "Finder-2").start();
    }
}
