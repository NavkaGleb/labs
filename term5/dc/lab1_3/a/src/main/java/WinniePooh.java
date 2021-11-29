public class WinniePooh implements Runnable {

    private final HoneyPot honeyPot;

    public WinniePooh(HoneyPot honeyPot) {
        this.honeyPot = honeyPot;
    }

    public void startSleep() {
        new Thread(this).start();
    }

    private void eatAllHoney() {
        honeyPot.clear();
    }

    @Override
    public void run() {
        while (true) {
            try {
                honeyPot.waitForFullness();

                System.out.printf("%s", "WinniePooh woke up and ate all the honey!!!\n");
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            eatAllHoney();
        }
    }
}
