import java.util.concurrent.Semaphore;

public class Table {

    private final Component[]   components = { null, null };
    private final Semaphore     semaphore = new Semaphore(1, true);
    private boolean             free = true;

    public Component getComponent(int index) {
        assert index >= 0 && index <= 1;
        assert !free;

        return components[index];
    }

    public void putComponent(int index, Component component) {
        assert index >= 0 && index <= 1;
        assert !free;

        components[index] = component;
    }

    public void grabComponents() {
        components[0] = null;
        components[1] = null;
    }

    public void acquire() throws InterruptedException {
        free = false;
        semaphore.acquire();
    }

    public void release() {
        free = true;
        semaphore.release();
    }
}
