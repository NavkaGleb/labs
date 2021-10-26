public class CyclicBarrier {

    private final int       parties;
    private int             waitingFor;
    private final Runnable  action;

    public CyclicBarrier(int parties, Runnable action) {
        this.parties = parties;
        this.waitingFor = parties;
        this.action = action;
    }

    public synchronized void await() throws InterruptedException {
        waitingFor--;

        if (waitingFor > 0) {
            wait();
        } else {
            waitingFor = parties;
            notifyAll();
            action.run();
        }
    }
}
