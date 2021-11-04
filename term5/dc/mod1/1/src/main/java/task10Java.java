// Variant 10

import java.util.concurrent.ThreadLocalRandom;

class Config {
    public static final double MIN_STOCK_PRICE = 0.5;
    public static final double MAX_STOCK_PRICE = 1.5;

    public static final int STOCK_COUNT = 20;
    public static final int CLIENTS_COUNT = 3;
}

class Stock {
    private double          availablePercent = ThreadLocalRandom.current().nextDouble(0.0, 1.0);
    private final double    index = ThreadLocalRandom.current().nextDouble(1, 5);
    private double          price = ThreadLocalRandom.current().nextDouble(Config.MIN_STOCK_PRICE, Config.MAX_STOCK_PRICE);

    public double getPrice() {
        return price;
    }

    void buy(double percent) {
        if (availablePercent - percent >= 0.0) {
            price += index * percent;
            availablePercent -= percent;
        }
    }

    void sell(double percent) {
        if (availablePercent + percent <= 100.0) {
            price -= index * percent;
            availablePercent += percent;
        }
    }

    double getAvailablePercent() {
        return availablePercent;
    }
}

class StockExchange {
    private final double minIndex;
    private double index;

    private final Stock[] stocks;

    StockExchange(int stockCount) {
        stocks = new Stock[stockCount];

        for (int i = 0; i < stocks.length; ++i) {
            stocks[i] = new Stock();
            index += stocks[i].getPrice();
        }

        minIndex = stockCount + Config.MIN_STOCK_PRICE;
        index = minIndex;
    }

    int getStockCount() {
        return stocks.length;
    }

    double getAvailablePercent(int index) {
        return stocks[index].getAvailablePercent();
    }

    synchronized void buyStock(int index, double percent) {
        stocks[index].buy(percent);
        updateIndex();
    }

    synchronized void sellStock(int index, double percent) {
        stocks[index].sell(percent);
        updateIndex();
    }

    private void updateIndex() {
        var newIndex = 0.0;

        for (var stock : stocks) {
            newIndex += stock.getPrice();
        }

        index = newIndex;

        System.out.printf("StockExchange index: %f (minIndex: %f)\n", index, minIndex);

        if (index <= minIndex) {
            throw new RuntimeException("The index fell heavily");
        }
    }
}

class BuyCellClient implements Runnable {

    private final StockExchange stockExchange;

    public BuyCellClient(StockExchange stockExchange) {
        this.stockExchange = stockExchange;
    }

    public void buyStock() {
        int stock = ThreadLocalRandom.current().nextInt(stockExchange.getStockCount());
        double percent = stockExchange.getAvailablePercent(stock) * ThreadLocalRandom.current().nextDouble(0.5, 1.0);

        stockExchange.buyStock(stock, percent);

        var threadName = Thread.currentThread().getName();

        System.out.printf("%s: buy stock %d. Percent = %f\n", threadName, stock, percent);
    }

    public void sellStock() {
        int stock = ThreadLocalRandom.current().nextInt(stockExchange.getStockCount());
        double percent = (1.0 - stockExchange.getAvailablePercent(stock)) * ThreadLocalRandom.current().nextDouble(0.5, 1.0);

        stockExchange.sellStock(stock, percent);

        var threadName = Thread.currentThread().getName();

        System.out.printf("%s: sell stock %d. Percent = %f\n\n", threadName, stock, percent);
    }

    @Override
    public void run() {
        while (true) {
            int decision = ThreadLocalRandom.current().nextInt(2);

            switch (decision) {
                case 0 -> buyStock();
                case 1 -> sellStock();
            }

            try {
                Thread.sleep(ThreadLocalRandom.current().nextInt(400, 3000));
            } catch (InterruptedException | RuntimeException e) {
                e.printStackTrace();
                Thread.currentThread().interrupt();
            }
        }
    }
}

public class task10Java {

    public static void main(String[] args) {
        var stockExchange = new StockExchange(Config.STOCK_COUNT);

        var buyCellClients = new BuyCellClient[Config.CLIENTS_COUNT];

        for (int i = 0; i < buyCellClients.length; ++i) {
            buyCellClients[i] = new BuyCellClient(stockExchange);
            new Thread(buyCellClients[i], "Client" + i).start();
        }
    }
}
