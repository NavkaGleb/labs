import java.util.Random;

public class Main {

    private static final Random random = new Random();

    private static class Config {
        public static int STORAGE_SIZE = 10000;
        public static int MIN_COST = 10;
        public static int MAX_COST = 100;
    }

    private static Storage generateStorage() {
        var result = new Storage();
        var sum = 0;

        for (int i = 0; i < Config.STORAGE_SIZE; ++i) {
            var item = new StorageItem(random.nextInt(Config.MAX_COST) + Config.MIN_COST);

            result.add(item);
            sum += item.getCost();
        }

        System.out.println("Storage is filled with products, total cost: " + sum);

        return result;
    }

    public static void main(String[] args) {
        var storage = generateStorage();
        var midpoint = new TransferQueue();
        var truck = new TransferQueue();

        var ivanov = new Ivanov(storage, midpoint);
        var petrov = new Petrov(midpoint, truck);
        var nechiporchuk = new Nechiporchuk(truck);

        try {
            ivanov.getThread().join();
            petrov.getThread().join();
            nechiporchuk.getThread().join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        System.out.println("Truck is loaded, total cost: " + nechiporchuk.getSum());
    }
}
