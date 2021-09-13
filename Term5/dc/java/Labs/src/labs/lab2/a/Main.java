package labs.lab2.a;

import java.util.Random;

public class Main {

    private final static Random random = new Random();

    private static class Config {
        public static final int FOREST_WIDTH = 100;
        public static final int FOREST_HEIGHT = 100;
        public static final int BEE_FLOCK_COUNT = 4;
    }

    public static void main(String[] args) {
        Forest forest = new Forest(Config.FOREST_WIDTH, Config.FOREST_HEIGHT);
        Beehive beehive = new Beehive(Config.BEE_FLOCK_COUNT);
        WinniePooh winniePooh = new WinniePooh(random.nextInt(Config.FOREST_WIDTH), random.nextInt(Config.FOREST_HEIGHT));

        System.out.printf("Winnie set to position {%d, %d}\n", winniePooh.getX(), winniePooh.getY());

        forest.setWinniePooh(winniePooh);
        forest.findWinniePooh(beehive);
    }
}
