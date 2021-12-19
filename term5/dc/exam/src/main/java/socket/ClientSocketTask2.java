package socket;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

import util.Customer;

public class ClientSocketTask2 {

    private static final int PORT = 9876;

    public static void logCustomers(List<Customer> customers) {
        if (customers.isEmpty()) {
            System.out.println("None!");
            return;
        }

        for (var customer : customers) {
            System.out.println(customer);
        }
    }

    public static void main(String[] args) throws IOException, ClassNotFoundException, InterruptedException {
        Socket socket;
        ObjectOutputStream oos;
        ObjectInputStream ois;

        InetAddress host = InetAddress.getLocalHost();
        Scanner scan = new Scanner(System.in);

        while (true) {
            System.out.println(
                """
                Choose option:
                1 - display Customers in alphabetic order
                2 - display Customers which card number in interval of 'a' and 'b'
                3 - exit
                """
            );

            socket = new Socket(host.getHostName(), PORT);
            oos = new ObjectOutputStream(socket.getOutputStream());

            System.out.println("Sending request to Socket Server");

            int commandIndex = scan.nextInt();

            if (commandIndex == 3) {
                socket = new Socket(host.getHostName(), PORT);
                oos = new ObjectOutputStream(socket.getOutputStream());

                System.out.println("Sending close Request");

                oos.writeInt(commandIndex);
                oos.flush();

                break;
            }

            switch (commandIndex) {
                case 1 -> {
                    String message = "" + commandIndex;

                    oos.writeBytes(message);
                    oos.flush();
                }

                case 2 -> {
                    System.out.println("Enter 'a'");
                    int a = scan.nextInt();

                    System.out.println("Enter 'b'");
                    int b = scan.nextInt();

                    String message = "" + commandIndex + "#" + a + "#" + b;

                    oos.writeBytes(message);
                    oos.flush();
                }
            }

            System.out.println("Results: ");
            ois = new ObjectInputStream(socket.getInputStream());

            logCustomers((ArrayList<Customer>) ois.readObject());

            ois.close();
            oos.close();

            Thread.sleep(100);
        }

        oos.writeInt(3);

        System.out.println("bb :)");
    }
}
