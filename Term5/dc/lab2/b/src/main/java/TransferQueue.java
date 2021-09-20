public class TransferQueue implements ItemQueue {

    private StorageItem item;
    private boolean     isValueSet = false;

    synchronized public StorageItem poll() {
        while (!isValueSet) {
            try {
                wait();
            } catch (InterruptedException e) {
                System.out.println("Interrupted");
            }
        }

        isValueSet = false;
        notify();
        return item;
    }

    synchronized public boolean add(StorageItem item) {
        while(isValueSet) {
            try {
                wait();
            } catch (InterruptedException e) {
                System.out.println("Interrupted");
            }
        }

        this.item = item;
        isValueSet = true;
        notify();

        return true;
    }
}
