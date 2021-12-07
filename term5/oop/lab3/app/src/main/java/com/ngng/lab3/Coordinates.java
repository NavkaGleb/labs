package com.ngng.lab3;

public class Coordinates {

    public int x;
    public int y;

    public Coordinates(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public static Coordinates toCoordinates(float x, float y, int fieldSize, int cellSize) {
        Coordinates result = new Coordinates(
            (int)(x / cellSize),
            (int)(y / cellSize)
        );

        return result.x >= fieldSize || result.y >= fieldSize ? null : result;
    }

    @Override
    public boolean equals(Object object) {
        if (!(object instanceof Coordinates)) {
            return false;
        }

        Coordinates coordinates = (Coordinates)(object);

        return coordinates.x == x && coordinates.y == y;
    }

    @Override
    public int hashCode() {
        return x + y * 1000;
    }
}
