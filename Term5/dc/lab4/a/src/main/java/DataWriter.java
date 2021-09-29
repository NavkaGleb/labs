import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReadWriteLock;

public class DataWriter implements Runnable {

    private final File          file;
    private final Lock          writeLock;
    private final AtomicInteger personCount = new AtomicInteger();
    private int                 personId = 0;

    public DataWriter(File file, ReadWriteLock readWriteLock) {
        this.file = file;
        this.writeLock = readWriteLock.writeLock();
    }

    @Override
    public void run() {
        while (true) {
            try {
                Thread.sleep(ThreadLocalRandom.current().nextInt(1500) + 1500);

                writeLock.lock();
                System.out.printf("[%s]:\tLocked\n", Thread.currentThread().getName());

                if (Math.random() < 0.6 || personCount.get() == 0) {
                    String firstName = "first_name_" + personId;
                    String lastName = "last_name_" + personId;
                    String phone = String.valueOf(personId).repeat(5);

                    addRecord(new Person(firstName, lastName, phone));
                } else {
                    deleteRecord(ThreadLocalRandom.current().nextInt(personCount.get()));
                }

                System.out.printf("[%s]:\tUnlocked\n", Thread.currentThread().getName());
                writeLock.unlock();
            } catch (InterruptedException | IOException e) {
                e.printStackTrace();
            }
        }
    }

    public int getPersonId() {
        return personId;
    }

    private void addRecord(Person person) throws IOException {
        var writer = new FileWriter(file, true);

        personId++;
        personCount.incrementAndGet();
        writer.write(person.toString() + " ");

        String threadName = Thread.currentThread().getName();
        String firstName = person.getFirstName();
        String lastName = person.getLastName();
        String phone = person.getPhone();

        System.out.print(ConsoleColor.GREEN);
        System.out.printf("[%s]:\tAdded person: (%s %s: %s)\n", threadName, firstName, lastName, phone);
        System.out.print(ConsoleColor.RESET);

        writer.flush();
    }

    private void deleteRecord(int index) throws IOException {
        var scanner = new Scanner(file);
        var people = new ArrayList<Person>();

        for (int i = 0; scanner.hasNext(); ++i) {
            String firstName = scanner.next();
            String lastName = scanner.next();
            String phone = scanner.next();

            var person = new Person(firstName, lastName, phone);

            if (i != index) {
                people.add(person);
            } else {
                personCount.decrementAndGet();
                String threadName = Thread.currentThread().getName();

                System.out.print(ConsoleColor.RED);
                System.out.printf("[%s]:\tRemoved person: (%s %s: %s)\n", threadName, firstName, lastName, phone);
                System.out.print(ConsoleColor.RESET);
            }
        }

        var writer = new FileWriter(file, false);

        for (var person : people) {
            writer.write(person.toString() + " ");
        }

        writer.flush();
    }
}