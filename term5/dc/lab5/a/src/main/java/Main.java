import java.util.Arrays;
import java.util.concurrent.atomic.AtomicBoolean;

public class Main {

    private static class Config {
        public static final int RECRUIT_LINE_COUNT = 3;
        public static final int RECRUIT_COUNT = 50;
    }

    private static final AtomicBoolean isDone = new AtomicBoolean(false);
    private static final RecruitLine[] recruitLines = new RecruitLine[Config.RECRUIT_LINE_COUNT];
    private static final CyclicBarrier barrier = new CyclicBarrier(Config.RECRUIT_LINE_COUNT, Main::barrier);

    public static void main(String[] args) throws InterruptedException {
        for (int i = 0; i < Config.RECRUIT_LINE_COUNT; ++i) {
            recruitLines[i] = new RecruitLine(Config.RECRUIT_COUNT, isDone, barrier);
        }

        setLeftRight();

        var threads = Arrays.stream(recruitLines).map(Thread::new).toArray(Thread[]::new);

        for (var thread : threads) {
            thread.start();
        }

        for (var thread : threads) {
            thread.join();
        }
    }

    private static void setLeftRight() {
        for (int i = 0; i < recruitLines.length; ++i) {
            if (i == 0) {
                recruitLines[i].setLeft(null);
            } else {
                recruitLines[i].setLeft(recruitLines[i - 1].getRight());
            }

            if (i == recruitLines.length - 1) {
                recruitLines[i].setRight(null);
            } else {
                recruitLines[i].setRight(recruitLines[i + 1].getLeft());
            }
        }
    }

    private static void barrier()  {
        try {
            Thread.sleep(20);

            var modified = false;
            var string = new StringBuilder();

            for (int i = 0; i < Config.RECRUIT_LINE_COUNT; ++i) {
                string.append(recruitLines[i]);

                modified = modified || recruitLines[i].isModified();
            }

            System.out.printf("Line: %s\n", string);

            if (!modified) {
                isDone.set(true);
            } else {
                setLeftRight();
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
