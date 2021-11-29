public interface ItemQueue {

    StorageItem poll();
    boolean add(StorageItem t);
}
