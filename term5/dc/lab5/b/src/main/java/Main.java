import java.util.Arrays;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.atomic.AtomicBoolean;

public class Main {

    public static class Config {
        public static final int STRING_HANDLER_COUNT = 4;
        public static final int CHARACTER_COUNT = 15;
    }

    public static void main(String[] args) throws InterruptedException {
        var handlers = new StringHandler[Config.STRING_HANDLER_COUNT];

        var isDone = new AtomicBoolean(false);

        var barrier = new CyclicBarrier(Config.STRING_HANDLER_COUNT, () -> {
            try {
                Thread.sleep(200);
                System.out.println();

                var counters = Arrays.stream(handlers).mapToInt(StringHandler::getAbCharacterCounter).toArray();

                for (int i = 0; i < Config.STRING_HANDLER_COUNT; ++i) {
                    System.out.printf("Handler %d: %s, %d\n", i, handlers[i].getColoredString(), counters[i]);
                }

                if (((counters[0] == counters[1]) || (counters[2] == counters[3])) && ((counters[0] == counters[2]) || (counters[1] == counters[3]))) {
                    isDone.set(true);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        });

        for (int i = 0; i < Config.STRING_HANDLER_COUNT; ++i) {
            handlers[i] = new StringHandler(Config.CHARACTER_COUNT, isDone, barrier);
        }

        var threads = Arrays.stream(handlers).map(Thread::new).toArray(Thread[]::new);

        for (var thread : threads) {
            thread.start();
        }

        for (var thread : threads) {
            thread.join();
        }
    }
}
