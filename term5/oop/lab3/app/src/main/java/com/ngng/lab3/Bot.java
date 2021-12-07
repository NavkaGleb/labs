package com.ngng.lab3;

import java.util.List;
import java.util.Map;
import java.util.Random;

public class Bot {

    private static final Random random = new Random();

    // Returns null if no available
    public static Coordinates chooseChecker(BoardManager boardManager, Checker.Color color, long delay) {
        sleep(delay);

        Map<Checker, List<Coordinates>> available = boardManager.getAvailableListByColor(color);
        Coordinates result = null;

        for (Map.Entry<Checker, List<Coordinates>> entry : available.entrySet()) {
            if (!entry.getValue().isEmpty()) {
                result = boardManager.find(entry.getKey());
                break;
            }
        }

        return result;
    }

    public static Coordinates chooseMove(BoardManager boardManager, Checker checker, long delay) {
        sleep(delay);

        List<Coordinates> available = boardManager.buildAvailable(checker);

        return available.isEmpty() ? null : available.get(random.nextInt(available.size()));
    }

    private static void sleep(long delay) {
        try {
            Thread.sleep(delay);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}
