package labs.lab2.a;

import java.util.concurrent.atomic.AtomicInteger;

public record Beehive(int flockCount) {

    public record ScanForestRegion(Forest forest, AtomicInteger freeRegion) implements Runnable {

        @Override
        public void run() {
            int currentRegion;

            while ((currentRegion = freeRegion.decrementAndGet()) >= 0) {
                for (int x = 0; x < forest.getWidth(); ++x) {
                    if (!forest.tryToKillWinniePooh(x, currentRegion)) {
                        continue;
                    }

                    freeRegion.set(0);

                    System.out.printf("Winnie was found and killed on: (%d, %d) by ", x, currentRegion);
                    System.out.println(Thread.currentThread().getName());
                }
            }
        }
    }

    public int getFlockCount() {
        return flockCount;
    }
}
