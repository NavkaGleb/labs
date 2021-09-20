import java.util.concurrent.atomic.AtomicInteger;

public class Forest {

    private final int           width;
    private final int           height;

    private final boolean[][]   area;

    public Forest(int width, int height) {
        this.width = width;
        this.height = height;

        this.area = new boolean[width][height];
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }

    public void setWinniePooh(WinniePooh winniePooh) {
        int x = winniePooh.getX();
        int y = winniePooh.getY();

        assert x > 0 && x < width;
        assert y > 0 && y < height;

        area[x][y] = true;
    }

    public boolean tryToKillWinniePooh(int x, int y) {
        if (!area[x][y]) {
            return false;
        }

        area[x][y] = false;
        return true;
    }

    public void searchWinniePooh(Beehive beehive) {
        AtomicInteger freeColumn = new AtomicInteger(height);

        for (int i = 0; i < beehive.getFlockCount(); ++i) {
            (new Thread(new Beehive.ScanForestRegion(this, freeColumn), "Flock" + i)).start();
        }
    }
}
