public class Nechiporchuk extends Burglar {

    private int sum = 0;

    public Nechiporchuk(ItemQueue queue) {
        super(queue);
    }

    @Override
    public void action() {
        var item = queue.poll();

        if (item == null) {
            stop = true;
        } else {
            sum += item.getCost();
        }
    }

    public int getSum() {
        return this.sum;
    }
}
