package com.ngng.lab3;

import android.graphics.Paint;

public class Checker {

    public enum State {
        NORMAL,
        QUEEN
    }

    public enum Color {
        BLACK(android.graphics.Color.rgb(20, 20, 20)),
        WHITE(android.graphics.Color.rgb(200, 200, 200)),
        ERROR(android.graphics.Color.rgb(0, 250, 0));

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
    private State state = State.NORMAL;

    public Checker(Color color) {
        this.color = color;
        this.paint = new Paint();
        this.paint.setStyle(Paint.Style.FILL);
    }

    public Color getColor() {
        return color;
    }

    public State getState() {
        return state;
    }

    public void setState(State state) {
        this.state = state;
        updatePaint();
    }

    public Paint updatePaint() {
        switch (color) {
            case BLACK:
                paint.setColor(Color.BLACK.getValue());
                break;

            case WHITE:
                paint.setColor(Color.WHITE.getValue());
                break;

            default:
                paint.setColor(Color.ERROR.getValue());
                break;
        }

        return paint;
    }
}
