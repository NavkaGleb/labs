import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.function.Predicate;

public class DataReader implements Runnable {

    private final File              file;
    private final Lock              readLock;
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
                Thread.sleep(ThreadLocalRandom.current().nextInt(1500) + 1500);
                readLock.lock();
                System.out.printf("[%s]:\tLocked\n", Thread.currentThread().getName());

                Thread.sleep(500);

                var person = getRecord();

                if (person != null) {
                    System.out.print(ConsoleColor.MAGENTA);
                    System.out.printf("[%s]:\tFound person: (%s %s: %s)\n", Thread.currentThread().getName(), person.getFirstName(), person.getLastName(), person.getPhone());
                    System.out.print(ConsoleColor.RESET);
                }

                System.out.printf("[%s]:\tUnlocked\n", Thread.currentThread().getName());
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