public class Smoker implements Runnable {

    private final Table     table;
    private final Component component;

    public Smoker(Table table, Component component) {
        this.table = table;
        this.component = component;
    }

    @Override
    public void run() {
        while (true) {
            try {
                table.acquire();

                var component1 = table.getComponent(0);
                var component2 = table.getComponent(0);

                if (component1 != null && component2 != null && component1 != component && component2 != component) {
                    table.grabComponents();

                    Thread.sleep(500);
                    System.out.println("Smoker with component " + component + " grab components!");
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
}
