import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class Mediator implements Runnable {

    private final Table             table;
    private final List<Component>   components;

    public Mediator(Table table) {
        this.table = table;

        this.components = new ArrayList<>(Arrays.asList(
            Component.TOBACCO,
            Component.PAPER,
            Component.MATCHES
        ));
    }

    @Override
    public void run() {
        while (true) {
            try {
                table.acquire();

                var component1 = table.getComponent(0);
                var component2 = table.getComponent(1);

                if (component1 == null && component2 == null) {
                    shuffleComponents();

                    table.putComponent(0, components.get(0));
                    table.putComponent(1, components.get(1));

                    Thread.sleep(1000);
                    System.out.printf(
                        "Mediator put on the table %s and %s!\n",
                        components.get(0),
                        components.get(1)
                    );
                }

                table.release();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void start() {
        new Thread(this).start();
    }

    private void shuffleComponents() {
        Collections.shuffle(components);
    }
}
