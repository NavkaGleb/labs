import java.util.concurrent.ThreadLocalRandom;

public record Nature(Garden garden, ReadWriteLock lock) implements Runnable {

    @Override
    public void run() {
        try {
            while (true) {
                Thread.sleep(ThreadLocalRandom.current().nextInt(1000) + 1000);
                lock.writeLock();

                System.out.print(ConsoleColor.GREEN);
                System.out.printf("%-20s Locked\n", "[" + Thread.currentThread().getName() + "]:");
                System.out.printf("%-20s Working\n", "[" + Thread.currentThread().getName() + "]:");

                garden.evolve((Plant plant) -> {
                    var state = plant.getState();

                    if (ThreadLocalRandom.current().nextDouble() >= state.getValue()) {
                        switch (state) {
                            case BEST -> plant.setState(Plant.State.GOOD);
                            case GOOD, BAD -> plant.setState(Plant.State.BAD);
                        }
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
