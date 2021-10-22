import java.nio.charset.StandardCharsets;
import java.util.Formatter;
import java.util.concurrent.ThreadLocalRandom;

public record Gardener(Garden garden, ReadWriteLock lock) implements Runnable {

    @Override
    public void run() {
        try {
            while (true) {
                Thread.sleep(ThreadLocalRandom.current().nextInt(1000) + 1000);
                lock.writeLock();

                System.out.print(ConsoleColor.MAGENTA);
                System.out.printf("%-20s Locked\n", "[" + Thread.currentThread().getName() + "]:");
                System.out.printf("%-20s Working\n", "[" + Thread.currentThread().getName() + "]:");

                garden.revive((Plant plant) -> {
                    if (plant.getState() == Plant.State.BAD) {
                        plant.setState(Plant.State.BEST);
                    }
                });

                System.out.printf("%-20s Unlocked\n", "[" + Thread.currentThread().getName() + "]:");
                System.out.print(ConsoleColor.RESET);

                lock.writeUnlock();
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
