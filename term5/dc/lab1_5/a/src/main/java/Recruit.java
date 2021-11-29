import java.util.concurrent.ThreadLocalRandom;

public class Recruit {

    public enum Direction {
        LEFT,
        RIGHT;

        public String toColoredString() {
            return switch (this) {
                case LEFT -> ConsoleColor.BLUE_BOLD + "<" + ConsoleColor.RESET;
                case RIGHT -> ConsoleColor.YELLOW_BOLD + ">" + ConsoleColor.RESET;
            };
        }
    }

    private Direction direction;

    public Recruit() {
        this.direction = ThreadLocalRandom.current().nextBoolean() ? Direction.LEFT : Direction.RIGHT;
    }

    public Recruit(Recruit another) {
        this.direction = another.direction;
    }

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

    public String toColoredString() {
        return direction.toColoredString();
    }
}
