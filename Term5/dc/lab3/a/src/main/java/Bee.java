public class Bee implements Runnable {

    private static int beeId = 0;

    private final HoneyPot honeyPot;

    public Bee(HoneyPot honeyPot) {
        this.honeyPot = honeyPot;
    }

    public void startWork() {
        new Thread(this, "Bee" + beeId++).start();
    }

    @Override
    public void run() {
        while (true) {
            if (honeyPot.isNeedMoreHoney()) {
                honeyPot.putHoney();

                System.out.printf("%s puts honey!\n", Thread.currentThread().getName());

                try {
                    var sleepTime = (int)(Math.random() * 1000 + 300);
                    Thread.sleep(sleepTime);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
