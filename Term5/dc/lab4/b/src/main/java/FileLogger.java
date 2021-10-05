import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.concurrent.ThreadLocalRandom;

public class FileLogger implements Runnable {

    private final Garden        garden;
    private final ReadWriteLock lock;
    private final File          file = new File("garden.txt");

    public FileLogger(Garden garden, ReadWriteLock lock) {
        this.garden = garden;
        this.lock = lock;

        FileWriter writer;

        try {
            writer = new FileWriter(file, false);
            writer.write("");
            writer.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        try {
            while (true) {
                Thread.sleep(ThreadLocalRandom.current().nextInt(1000) + 1000);
                lock.readLock();

                System.out.printf("%-20s Locked\n", "[" + Thread.currentThread().getName() + "]:");
                System.out.printf("%-20s Working\n", "[" + Thread.currentThread().getName() + "]:");

                Thread.sleep(500);

                var writer = new FileWriter(file, true);
                writer.write(garden + "\n");
                writer.flush();

                System.out.printf("%-20s Unlocked\n", "[" + Thread.currentThread().getName() + "]:");

                lock.readUnlock();
            }
        } catch (InterruptedException | IOException e) {
            e.printStackTrace();
        }
    }
}
