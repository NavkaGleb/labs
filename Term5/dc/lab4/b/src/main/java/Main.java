public class Main {

    public static void main(String[] args) {
        var lock = new ReadWriteLock();
        var garden = new Garden(10, 10);

        new Thread(new Gardener(garden, lock), "Gardener").start();
        new Thread(new Nature(garden, lock), "Nature").start();
        new Thread(new ConsoleLogger(garden, lock), "ConsoleLogger").start();
        new Thread(new FileLogger(garden, lock), "FileLogger").start();
    }
}
