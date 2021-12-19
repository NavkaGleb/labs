package socket;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import util.Customer;

class Callback {

    public boolean shouldEnd = false;
}

class CustomerIterator implements Runnable {

    private final Socket          socket;
    private final Callback        callback;
    private final List<Customer>  customers;

    public CustomerIterator(Socket socket, Callback callback, List<Customer> customers) {

        this.callback = callback;
        this.socket = socket;
        this.customers = customers;
    }

    @Override
    public void run() {
        try {
            InputStreamReader ois = new InputStreamReader(socket.getInputStream());

            System.out.println("Receiving input");

            BufferedReader reader = new BufferedReader(ois);
            String message = reader.readLine();
            String[] splitMessage = message.split("#");
            String commandIndex = splitMessage[0];

            System.out.println("Command " + splitMessage[0]);

            if (commandIndex.equals("3")) {
                System.out.println("Close command");
                callback.shouldEnd = true;
                return;
            }

            List<Customer> result = new ArrayList<>();

            switch (commandIndex) {
                case "1" -> {
                    result = customers;
                    Collections.sort(result);
                }

                case "2" -> {
                    int a = Integer.parseInt(splitMessage[1]);
                    int b = Integer.parseInt(splitMessage[2]);

                    for (Customer customer : customers) {
                        if (customer.getCardNumber() > a && customer.getCardNumber() < b) {
                            result.add(customer);
                        }
                    }
                }
            }

            ObjectOutputStream oos = new ObjectOutputStream(socket.getOutputStream());
            oos.writeObject(result);
            ois.close();
            oos.close();
            socket.close();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

public class ServerSocketTask2 {

    private static final int PORT = 9876;

    public static void main(String[] args) throws IOException {
        var server = new ServerSocket(PORT);
        var callback = new Callback();

        var customers = Customer.getList();

        while (!callback.shouldEnd) {
            System.out.println("Waiting for the client request");
            Socket socket = server.accept();

            CustomerIterator iterator = new CustomerIterator(socket, callback, customers);

            iterator.run();
        }

        System.out.println("Shutting down Socket server!!");
        server.close();
    }
}

