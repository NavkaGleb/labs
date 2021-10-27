import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.atomic.AtomicBoolean;

public class StringHandler implements Runnable {

    private String              string;
    private final AtomicBoolean isDone;
    private final CyclicBarrier barrier;

    private int abCharacterCounter = 0;

    public StringHandler(int characterCount, AtomicBoolean isDone, CyclicBarrier barrier) {
        this.string = getRandomString(characterCount);

        this.isDone = isDone;
        this.barrier = barrier;
    }

    public String getColoredString() {
        var builder = new StringBuilder();

        for (int i = 0; i < string.length(); ++i) {
            switch (string.charAt(i)) {
                case 'A', 'B' -> builder.append(ConsoleColor.GREEN).append(string.charAt(i)).append(ConsoleColor.RESET);
                default -> builder.append(string.charAt(i));
            }
        }

        return builder.toString();
    }

    public int getAbCharacterCounter() {
        return abCharacterCounter;
    }

    private static String getRandomString(int characterCount) {
        var builder = new StringBuilder();

        for (int i = 0; i < characterCount; ++i) {
            var characterProbability = ThreadLocalRandom.current().nextInt(4);

            switch (characterProbability) {
                case 0 -> builder.append('A');
                case 1 -> builder.append('B');
                case 2 -> builder.append('C');
                case 3 -> builder.append('D');
            }
        }

        return builder.toString();
    }

    @Override
    public void run() {
        try {
            while (!isDone.get()) {
                var modifiedString = new StringBuilder(string);

                abCharacterCounter = 0;

                for (int i = 0; i < string.length(); ++i) {
                    if (ThreadLocalRandom.current().nextBoolean()) {
                        modifiedString.setCharAt(i, switch (modifiedString.charAt(i)) {
                            case 'A' -> 'C';
                            case 'C' -> 'A';
                            case 'B' -> 'D';
                            case 'D' -> 'B';
                            default -> throw new IllegalStateException("Unexpected value: " + modifiedString.charAt(i));
                        });
                    }

                    if (modifiedString.charAt(i) == 'A' || modifiedString.charAt(i) == 'B') {
                        abCharacterCounter++;
                    }
                }

                string = modifiedString.toString();

                barrier.await();
            }
        } catch (InterruptedException | BrokenBarrierException e) {
            e.printStackTrace();
        }
    }
}
