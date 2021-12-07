package com.ngng.lab3;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class BoardManager implements Drawable {

    public static final double CHECKER_BORDER_MULTIPLIER = 1.15;
    public static final double QUEEN_INNER_CIRCLE_MULTIPLIER = 0.7;
    public static final double CHECKER_RADIUS_MULTIPLIER = 0.35;

    private final Paint queenPaint;
    private final int   boardSize;
    private int         cellSize;
    private int         checkerSize;
    private Checker[][] table;

    public BoardManager(int boardSize) {
        this.boardSize = boardSize;
        this.queenPaint = new Paint();
        
        this.queenPaint.setColor(Color.rgb(50, 50, 50));

        initCheckers();
    }

    private void initCheckers() {
        table = new Checker[boardSize][boardSize];

        initWhites();
        initBlacks();
    }

    private void initWhites() {
        for (int row = 0; row < 3; ++row) {
            initRow(row, Checker.Color.WHITE);
        }
    }

    private void initBlacks() {
        for (int row = boardSize - 1; row > boardSize - 3 - 1; --row) {
            initRow(row, Checker.Color.BLACK);
        }
    }

    private void initRow(int row, Checker.Color color) {
        for (int i = 0; i < boardSize; i++) {
            if ((row + i) % 2 == 1) {
                table[row][i] = new Checker(color);
            }
        }
    }

    @Override
    public void draw(Canvas canvas) {
        cellSize = canvas.getWidth() / boardSize;
        checkerSize = (int) (cellSize * CHECKER_RADIUS_MULTIPLIER);

        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if ((j + i) % 2 == 1 && table[i][j] != null) {
                    drawChecker(i, j, table[i][j], canvas);
                }
            }
        }
    }

    private void drawChecker(int row, int column, Checker checker, Canvas canvas) {
        int cx = column * cellSize + cellSize / 2;
        int cy = row * cellSize + cellSize / 2;

        canvas.drawCircle(cx, cy, (float) (checkerSize * CHECKER_BORDER_MULTIPLIER), queenPaint);
        canvas.drawCircle(cx, cy, checkerSize, checker.updatePaint());

        if (checker.getState() == Checker.State.QUEEN) {
            canvas.drawCircle(cx, cy, (int)(checkerSize * QUEEN_INNER_CIRCLE_MULTIPLIER), queenPaint);
        }
    }

    public Checker getChecker(int x, int y) {
        return isInBoard(x) && isInBoard(y) ? table[y][x] : null;
    }

    public Checker getChecker(Coordinates coordinates) {
        return table[coordinates.y][coordinates.x];
    }

    public Coordinates find(Checker checker) {
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                if (table[i][j] == null) {
                    continue;
                }

                if (table[i][j].equals(checker)) {
                    return new Coordinates(j, i);
                }
            }
        }

        return null;
    }

    public void remove(int x, int y) {
        table[y][x] = null;
    }

    public boolean move(Checker checker, int x, int y) {
        boolean haveBeaten = false;

        Coordinates found = find(checker);

        if (found != null && !found.equals(new Coordinates(x, y))) {
            haveBeaten = beat(found.x, found.y, x, y);
        }

        table[y][x] = checker;

        if (found != null) {
            remove(found.x, found.y);
        }

        return haveBeaten;
    }

    private boolean beat(int xStart, int yStart, int xEnd, int yEnd) {
        boolean haveBeaten = false;
        
        int xDirection = xEnd - xStart;
        int yDirection = yEnd - yStart;
        
        if (xDirection == 0 || yDirection == 0) {
            return false;
        }
        
        xDirection = xDirection / Math.abs(xDirection);
        yDirection = yDirection / Math.abs(yDirection);

        int xCurrent = xStart;
        int yCurrent = yStart;

        while (xEnd - xCurrent != 0) {
            xCurrent += xDirection;
            yCurrent += yDirection;

            if (table[yCurrent][xCurrent] != null) {
                haveBeaten = true;
            }

            table[yCurrent][xCurrent] = null;
        }
        
        return haveBeaten;
    }

    public void updateQueens() {
        // BLACKS
        queensRow(0, Checker.Color.BLACK);

        // WHITES
        queensRow(boardSize - 1, Checker.Color.WHITE);
    }

    private void queensRow(int i, Checker.Color black) {
        for (Checker checker : table[i]) {
            if (checker == null) {
                continue;
            }

            if (checker.getColor() == black) {
                checker.setState(Checker.State.QUEEN);
            }
        }
    }

    public List<Coordinates> buildAvailable(Checker checker) {
        Coordinates coordinates = find(checker);
        List<Coordinates> result = new ArrayList<>();

        int x = coordinates.x;
        int y = coordinates.y;

        if (checker.getState() == Checker.State.NORMAL) {
            if (checker.getColor() == Checker.Color.BLACK) {
                tryMove(x, y, -1, -1, checker, result);
                tryMove(x, y,  1, -1, checker, result);

                tryBeat(x, y, -1, 1, checker, result);
                tryBeat(x, y,  1, 1, checker, result);
            } else {
                tryBeat(x, y, -1, -1, checker, result);
                tryBeat(x, y,  1, -1, checker, result);

                tryMove(x, y, -1, 1, checker, result);
                tryMove(x, y,  1, 1, checker, result);
            }

        } else if (checker.getState() == Checker.State.QUEEN) {
            moveInVector(x, y,  1,  1, checker, result);
            moveInVector(x, y,  1, -1, checker, result);
            moveInVector(x, y, -1,  1, checker, result);
            moveInVector(x, y, -1, -1, checker, result);
        }

        return result;
    }

    private void moveInVector(int x, int y, int vx, int vy, Checker current, List<Coordinates> result) {
        while (isInBoard(x + vx) && isInBoard(y + vy)) {
            x = x + vx;
            y = y + vy;

            Checker other = getChecker(x, y);

            if (other == null) {
                result.add(new Coordinates(x, y));
            } else {
                if (other.getColor() == current.getColor()) {
                    return;
                }

                if (isInBoard(x + vx) && isInBoard(y + vy)) {
                    other = getChecker(x + vx, y + vy);

                    if (other == null) {
                        result.add(new Coordinates(x + vx, y + vy));
                    }

                    return;
                }
            }
        }
    }

    private void beatInVector(int x, int y, int vx, int vy, Checker current, List<Coordinates> res) {
        while (isInBoard(x + vx) && isInBoard(y + vy)) {
            x = x + vx;
            y = y + vy;

            Checker other = getChecker(x, y);

            if (other == null) {
                continue;
            }

            if (other.getColor() == current.getColor()) {
                return;
            }

            if (isInBoard(x + vx) && isInBoard(y + vy)) {
                other = getChecker(x + vx, y + vy);

                if (other == null) {
                    res.add(new Coordinates(x + vx, y + vy));
                }

                return;
            }
        }
    }

    private void tryMove(int x, int y, int dx, int dy, Checker current, List<Coordinates> result) {
        if (isInBoard(x, dx) && isInBoard(y, dy)) {
            Checker checkerOther = getChecker(x + dx, y + dy);

            if (checkerOther == null) {
                result.add(new Coordinates(x + dx, y + dy));
            } else if (checkerOther.getColor() != current.getColor() && isInBoard(x, 2 * dx) && isInBoard(y, 2 * dy)) {
                checkerOther = getChecker(x + 2 * dx, y + 2 * dy);

                if (checkerOther == null) {
                    result.add(new Coordinates(x + 2 * dx, y + 2 * dy));
                }
            }
        }
    }

    public Map<Checker, List<Coordinates>> getAvailableListByColor(Checker.Color color) {
        Map<Checker, List<Coordinates>> result = new HashMap<>();

        for (Checker[] row : table) {
            for (Checker checker : row) {
                if (checker == null) {
                    continue;
                }

                if (checker.getColor() == color) {
                    List<Coordinates> availableForThisChecker = buildAvailable(checker);
                    result.put(checker, availableForThisChecker);
                }
            }
        }

        return result;
    }

    private void tryBeat(int x, int y, int dx, int dy, Checker current, List<Coordinates> result) {
        if (isInBoard(x, dx) && isInBoard(y, dy)) {
            Checker checkerOther = getChecker(x + dx, y + dy);

            if (checkerOther != null && checkerOther.getColor() != current.getColor() && isInBoard(x, 2 * dx) && isInBoard(y, 2 * dy)) {
                checkerOther = getChecker(x + 2 * dx, y + 2 * dy);

                if (checkerOther == null) {
                    result.add(new Coordinates(x + 2 * dx, y + 2 * dy));
                }
            }
        }
    }

    private boolean isInBoard(int position, int offset) {
        return isInBoard(position + offset);
    }

    private boolean isInBoard(int position) {
        return (position >= 0) && position < boardSize;
    }

    public List<Coordinates> canBeat(Checker checker) {
        List<Coordinates> res = new ArrayList<>();
        Coordinates coordinates = find(checker);
        int x = coordinates.x;
        int y = coordinates.y;

        if (checker.getState() == Checker.State.NORMAL) {
            tryBeat(x, y, -1, 1, checker, res);
            tryBeat(x, y, 1, 1, checker, res);
            tryBeat(x, y, -1, -1, checker, res);
            tryBeat(x, y, 1, -1, checker, res);
        } else {
            beatInVector(x, y, 1, 1, checker, res);
            beatInVector(x, y, 1, -1, checker, res);
            beatInVector(x, y, -1, 1, checker, res);
            beatInVector(x, y, -1, -1, checker, res);
        }

        return res;
    }

    public int countByColor(Checker.Color color) {
        int count = 0;
        
        for (Checker[] row : table) {
            for (Checker checker : row) {
                if (checker != null && checker.getColor() == color) {
                    ++count;
                }
            }
        }
        
        return count;
    }

    public boolean isDraw(Checker.Color color) {
        Map<Checker, List<Coordinates>> available = getAvailableListByColor(color);

        for (Map.Entry<Checker, List<Coordinates>> entry : available.entrySet()) {
            if (!entry.getValue().isEmpty()) {
                return false;
            }
        }
        
        return true;
    }
}
