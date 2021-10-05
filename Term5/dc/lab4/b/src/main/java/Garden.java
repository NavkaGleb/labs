public class Garden {

    private final Plant[][] plants;

    public Garden(int width, int height) {
        assert width != 0 && height != 0;

        plants = new Plant[width][height];

        for (int i = 0; i < getWidth(); ++i) {
            for (int j = 0; j < getHeight(); ++j) {
                plants[i][j] = new Plant();
            }
        }
    }

    public int getWidth() {
        return plants.length;
    }

    public int getHeight() {
        return plants[0].length;
    }

    public void evolve(EvolvePlant evolvePlant) {
        for (int i = 0; i < getWidth(); ++i) {
            for (int j = 0; j < getHeight(); ++j) {
                evolvePlant.evolve(plants[i][j]);
            }
        }
    }

    public void revive(RevivePlant revivePlant) {
        for (int i = 0; i < getWidth(); ++i) {
            for (int j = 0; j < getHeight(); ++j) {
                revivePlant.revive(plants[i][j]);
            }
        }
    }

    @Override
    public String toString() {
        StringBuilder result = new StringBuilder();

        for (int i = 0; i < getWidth(); ++i) {
            for (int j = 0; j < getHeight(); ++j) {
                result.append(" ").append(plants[i][j]).append(" ");
            }

            result.append("\n");
        }

        return result.toString();
    }

    public String toColorString() {
        StringBuilder result = new StringBuilder();

        for (int i = 0; i < getWidth(); ++i) {
            for (int j = 0; j < getHeight(); ++j) {
                result.append(" ");

                result.append(switch (plants[i][j].getState()) {
                    case BEST -> ConsoleColor.GREEN_BOLD;
                    case GOOD -> ConsoleColor.YELLOW;
                    case BAD -> ConsoleColor.CYAN;
                });

                result.append(plants[i][j]).append(" ");
            }

            result.append("\n");
        }

        result.append(ConsoleColor.RESET);

        return result.toString();
    }
}
