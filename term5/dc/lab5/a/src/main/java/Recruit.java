import java.util.concurrent.ThreadLocalRandom;

public class Recruit {

    public enum Direction {
        LEFT,
        RIGHT;

        @Override
        public String toString() {
            return switch (this) {
                case LEFT -> ConsoleColor.RED + "<" + ConsoleColor.RESET;
                case RIGHT -> ConsoleColor.GREEN + ">" + ConsoleColor.RESET;
            };
        }

        public Direction turn() {
            return switch (this) {
                case LEFT -> RIGHT;
                case RIGHT -> LEFT;
            };
        }

        public boolean isGoingLeft() {
            return this == Direction.LEFT;
        }

        public boolean isGoingRight() {
            return this == Direction.RIGHT;
        }
    }

    public Direction direction = Math.random() < 0.5 ? Direction.LEFT : Direction.RIGHT;

    public Direction getDirection() {
        return direction;
    }

    public boolean isGoingLeft() {
        return direction == Direction.LEFT;
    }

    public boolean isGoingRight() {
        return direction == Direction.RIGHT;
    }

    public void turn() {
        switch (direction) {
            case LEFT -> direction = Direction.RIGHT;
            case RIGHT -> direction = Direction.LEFT;
        }
    }
}
