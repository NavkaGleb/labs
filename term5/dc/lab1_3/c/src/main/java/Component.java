public enum Component {

    TOBACCO,
    PAPER,
    MATCHES;

    public String toString() {
        return switch (this) {
            case TOBACCO -> "Tobacco";
            case PAPER -> "Paper";
            case MATCHES -> "Matches";
        };
    }
}
