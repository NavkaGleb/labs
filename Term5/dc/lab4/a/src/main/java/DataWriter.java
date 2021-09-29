import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReadWriteLock;

public class DataWriter implements Runnable {
    private final File file;
    private final Lock writeLock;
    private final AtomicInteger recordCount = new AtomicInteger();
    private int counter = 0;

    public DataWriter(File file, ReadWriteLock readWriteLock) {
        this.file = file;
        this.writeLock = readWriteLock.writeLock();
    }

    @Override
    public void run() {
        while (true) {
            try {
                Thread.sleep(Utils.randomInt(1500, 2500));
                writeLock.lock();
                System.out.printf("[%s] locked\n", Thread.currentThread().getName());

                if (Math.random() < 0.6) {
                    String firstName = "first_name_" + counter;
                    String lastName = "last_name_" + counter;
                    String phone = String.valueOf(counter).repeat(5);

                    addRecord(new Person(firstName, lastName, phone));
                } else {
                    deleteRecord(Utils.randomInt(1, recordCount.get()));
                }

                System.out.printf("[%s] unlocked\n", Thread.currentThread().getName());
                writeLock.unlock();
            } catch (InterruptedException | IOException e) {
                e.printStackTrace();
            }
        }
    }

    private void addRecord(Person person) throws IOException {
        var writer = new FileWriter(file, true);

        counter++;
        recordCount.incrementAndGet();
        writer.write(person.toString() + " ");

        String threadName = Thread.currentThread().getName();
        String firstName = person.getFirstName();
        String lastName = person.getLastName();
        String phone = person.getPhone();

        System.out.printf("[%s] added record: (%s, %s: %s)\n", threadName, firstName, lastName, phone);
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
                recordCount.decrementAndGet();
                String threadName = Thread.currentThread().getName();

                System.out.printf("[%s] removed record: (%s, %s: %s)\n", threadName, firstName, lastName, phone);
            }
        }

        var writer = new FileWriter(file, false);

        for (var person : people) {
            writer.write(person.toString() + " ");
        }

        writer.flush();
    }
}