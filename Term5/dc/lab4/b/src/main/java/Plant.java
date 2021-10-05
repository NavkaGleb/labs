public class Plant {

    public enum State {
        BEST(0.7),
        GOOD(0.5),
        BAD(0.0);

        private final double value;

        State(double state) {
            this.value = state;
        }

        public double getValue() {
            return value;
        }
    }

    private State state;

    public Plant() {
        state = State.BEST;
    }

    public State getState() {
        return state;
    }

    public void setState(State state) {
        this.state = state;
    }

    @Override
    public String toString() {
        return switch (state) {
            case BEST -> "8";
            case GOOD -> "0";
            case BAD -> "*";
        };
    }
}
