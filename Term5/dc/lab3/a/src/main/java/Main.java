public class Main {

    public static class Config {
        public static int HONEYPOT_SIZE = 40;
        public static int BEE_COUNT = 4;
    }

    public static void main(String[] args) {
        var honeyPot = new HoneyPot(Config.HONEYPOT_SIZE);
        var winniePooh = new WinniePooh(honeyPot);

        winniePooh.startSleep();

        for (var i = 0; i < Config.BEE_COUNT; ++i) {
            new Bee(honeyPot).startWork();
        }
    }
}
