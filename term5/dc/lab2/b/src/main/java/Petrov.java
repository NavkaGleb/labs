public class Petrov extends Burglar {

    private final ItemQueue nextQueue;

    public Petrov(ItemQueue queue, ItemQueue nextQueue) {
        super(queue);
        this.nextQueue = nextQueue;
    }

    @Override
    public void action() {
        var item = queue.poll();

        stop = item == null;
        nextQueue.add(item);
    }
}
