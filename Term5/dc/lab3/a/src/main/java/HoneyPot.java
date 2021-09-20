import java.util.concurrent.CountDownLatch;

public class HoneyPot {

    private final int       size;
    private CountDownLatch  honeyLeft;

    public HoneyPot(int size) {
        this.size = size;
        this.honeyLeft = new CountDownLatch(size);
    }

    public boolean isNeedMoreHoney() {
        return honeyLeft.getCount() != 0;
    }

    public void putHoney() {
        honeyLeft.countDown();
    }

    public void waitForFullness() throws InterruptedException {
        honeyLeft.await();
    }

    public void clear() {
        assert honeyLeft.getCount() == 0;

        this.honeyLeft = new CountDownLatch(size);
    }
}
