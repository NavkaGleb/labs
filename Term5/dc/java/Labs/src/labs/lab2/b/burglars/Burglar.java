package labs.lab2.b.burglars;

import labs.lab2.b.ItemQueue;

public abstract class Burglar implements Runnable {

    protected final ItemQueue   queue;
    protected boolean           stop = false;

    private final Thread        thread = new Thread(this);

    public Burglar(ItemQueue itemQueue) {
        this.queue = itemQueue;

        this.thread.start();
    }

    @Override
    public void run() {
        while (!stop) {
            action();
        }
    }

    public abstract void action();

    public Thread getThread() {
        return thread;
    }
}
