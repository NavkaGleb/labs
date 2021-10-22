public class ReadWriteLock {

    private int readCount = 0;
    private int writeCount = 0;
    private int writeRequests = 0;

    public synchronized void readLock() throws InterruptedException {
        while (writeCount > 0 || writeRequests > 0) {
            wait();
        }

        readCount++;
    }

    public synchronized void readUnlock() {
        readCount--;
        notifyAll();
    }

    public synchronized void writeLock() throws InterruptedException {
        writeRequests++;

        while (readCount > 0 || writeCount > 0) {
            wait();
        }

        writeRequests--;
        writeCount++;
    }

    public synchronized void writeUnlock() {
        writeCount--;
        notifyAll();
    }
}
