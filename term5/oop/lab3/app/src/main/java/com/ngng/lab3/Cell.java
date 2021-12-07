package com.ngng.lab3;

import android.graphics.Paint;

public class Cell {

    public enum State {
        IDLE,
        ACTIVE
    }

    public enum Color {
        BLACK(android.graphics.Color.rgb(50, 50, 50)),
        WHITE(android.graphics.Color.WHITE),
        ACTIVE(android.graphics.Color.rgb(255, 150, 10));

        private final int value;

        Color(int value) {
            this.value = value;
        }

        public int getValue() {
            return value;
        }
    }

    private final Color color;
    private final Paint paint;
    
    private final int idleColor;

    public Cell(Color color) {
        this.color = color;
        this.paint = new Paint();
        this.idleColor = this.color.getValue();

        this.paint.setColor(this.idleColor);
    }

    public Paint getPaint() {
        return paint;
    }

    public void setState(State state) {
        switch (state) {
            case IDLE:
                paint.setColor(idleColor);
                break;

            case ACTIVE:
                paint.setColor(Color.ACTIVE.getValue());
                break;
        }
    }

    public Color getColor() {
        return color;
    }
}
