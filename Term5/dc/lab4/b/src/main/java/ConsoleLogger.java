import java.util.concurrent.ThreadLocalRandom;

public record ConsoleLogger(Garden garden, ReadWriteLock lock) implements Runnable {

    @Override
    public void run() {
        try {
            while (true) {
                Thread.sleep(ThreadLocalRandom.current().nextInt(1000) + 1000);
                lock.readLock();

                System.out.print(ConsoleColor.CYAN);
                System.out.printf("%-20s Locked\n", "[" + Thread.currentThread().getName() + "]:");

                Thread.sleep(500);

                System.out.print(garden.toColorString());

                System.out.print(ConsoleColor.CYAN);
                System.out.printf("%-20s Unlocked\n", "[" + Thread.currentThread().getName() + "]:");
                System.out.print(ConsoleColor.RESET);

                lock.readUnlock();
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
