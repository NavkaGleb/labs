public class Ivanov extends Burglar {

    private final ItemQueue storage;

    public Ivanov(ItemQueue storage, ItemQueue queue) {
        super(queue);
        this.storage = storage;
    }

    @Override
    public void action() {
        var item = storage.poll();

        stop = item == null;
        queue.add(item);
    }
}
