package com.ngng.lab3;

import android.graphics.Canvas;

public class Board implements Drawable {

    private final Cell[][]      cells;
    private final int           boardSize;
    private int                 cellSize;

    public Board(int boardSize) {
        this.boardSize = boardSize;
        this.cells = new Cell[boardSize][boardSize];

        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                cells[j][i] = new Cell((i + j) % 2 == 0 ? Cell.Color.WHITE : Cell.Color.BLACK);
            }
        }
    }

    @Override
    public void draw(Canvas canvas) {
        cellSize = canvas.getWidth() / boardSize;

        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                drawCell(j, i, cells[j][i], canvas);
            }
        }
    }

    private void drawCell(int x, int y, Cell cell, Canvas canvas) {
        canvas.drawRect(
            x * cellSize,
            y * cellSize,
            x * cellSize + cellSize,
            y * cellSize + cellSize,
            cell.getPaint()
        );
    }

    public Cell getCell(Coordinates coordinates) {
        return !isInBoard(coordinates) ? null : cells[coordinates.x][coordinates.y];
    }

    public int getCellSize() {
        return cellSize;
    }

    public void unselectAll() {
        for (Cell[] row : cells) {
            for (Cell cell : row) {
                cell.setState(Cell.State.IDLE);
            }
        }
    }

    private boolean isInBoard(Coordinates coordinates) {
        return isInBoard(coordinates.x) && isInBoard(coordinates.y);
    }

    private boolean isInBoard(int value) {
        return value >= 0 && value < boardSize;
    }
}
