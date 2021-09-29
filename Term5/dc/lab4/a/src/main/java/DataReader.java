import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.function.Predicate;

public class DataReader implements Runnable {

    private final File file;
    private final Lock readLock;
    private final Predicate<Person> predicate;

    public DataReader(File file, ReadWriteLock readWriteLock, Predicate<Person> predicate) {
        this.file = file;
        this.readLock = readWriteLock.readLock();
        this.predicate = predicate;
    }

    @Override
    public void run() {
        while (true) {
            try {
                Thread.sleep(Utils.randomInt(1500, 2500));
                readLock.lock();
                System.out.printf("[%s] locked\n", Thread.currentThread().getName());

                var person = getRecord();

                Thread.sleep(500);

                if (person != null) {
                    System.out.printf("[%s] found record: (%s, %s)\n", Thread.currentThread().getName(), person.getFirstName(), person.getPhone());
                }

                System.out.printf("[%s] unlocked\n", Thread.currentThread().getName());
                readLock.unlock();
            } catch (InterruptedException | FileNotFoundException e) {
                e.printStackTrace();
            }
        }
    }

    private Person getRecord() throws FileNotFoundException {
        var scanner = new Scanner(file);

        while (scanner.hasNext()) {
            var firstName = scanner.next();
            var lastName = scanner.next();
            var phone = scanner.next();

            var person = new Person(firstName, lastName, phone);

            if (predicate.test(person)) {
                return person;
            }
        }

        return null;
    }
}