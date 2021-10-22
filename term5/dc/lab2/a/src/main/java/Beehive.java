import java.util.concurrent.atomic.AtomicInteger;

public record Beehive(int flockCount) {

    public record ScanForestRegion(Forest forest, AtomicInteger freeColumn) implements Runnable {

        @Override
        public void run() {
            int currentColumn;

            while ((currentColumn = freeColumn.decrementAndGet()) >= 0) {
                if (isExistWinniePoohInRow(currentColumn)) {
                    return;
                }
            }
        }

        private boolean isExistWinniePoohInRow(int column) {
            for (int x = 0; x < forest.getWidth(); ++x) {
                if (!forest.tryToKillWinniePooh(x, column)) {
                    continue;
                }

                freeColumn.set(0);

                System.out.printf("Winnie was found and killed on: (%d, %d) by ", x, column);
                System.out.println(Thread.currentThread().getName());

                return true;
            }

            return false;
        }
    }

    public int getFlockCount() {
        return flockCount;
    }
}
