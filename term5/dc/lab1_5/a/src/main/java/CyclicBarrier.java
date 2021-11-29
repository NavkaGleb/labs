public class CyclicBarrier {

    private final int       parties;
    private int             waitingParties;
    private final Runnable  action;

    public CyclicBarrier(int parties, Runnable action) {
        this.parties = parties;
        this.waitingParties = parties;
        this.action = action;
    }

    public synchronized void await() throws InterruptedException {
        waitingParties--;

        if (waitingParties > 0) {
            wait();
        } else {
            waitingParties = parties;
            notifyAll();
            action.run();
        }
    }
}
