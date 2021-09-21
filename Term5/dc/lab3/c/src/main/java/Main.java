import java.util.Arrays;
import java.util.LinkedList;

public class Main {

    public static void main(String[] args) {
        // Init
        var table = new Table();
        var mediator = new Mediator(table);
        var smokers = new LinkedList<>(Arrays.asList(
            new Smoker(table, Component.TOBACCO),
            new Smoker(table, Component.PAPER),
            new Smoker(table, Component.MATCHES)
        ));

        // Run
        mediator.start();

        for (var smoker : smokers) {
            smoker.start();
        }
    }
}
