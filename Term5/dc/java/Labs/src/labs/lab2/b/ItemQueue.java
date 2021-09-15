package labs.lab2.b;

public interface ItemQueue {

    public abstract StorageItem poll();
    public abstract boolean add(StorageItem t);
}
