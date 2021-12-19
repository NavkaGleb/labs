package rmi;

import util.Customer;

import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.util.List;
import java.util.Scanner;

public class ClientRmiTask2 {

    public static void logCustomers(List<Customer> customers) {
        if (customers.isEmpty()) {
            System.out.println("None!");
            return;
        }

        for (var customer : customers) {
            System.out.println(customer);
        }
    }

    public static void main(String[] args) {
        int choice;
        int a;
        int b;

        Scanner in = new Scanner(System.in);

        try {
            RMIServer rmiServer = (RMIServer) Naming.lookup("//localhost:123/exam");

            System.out.println(
                """
                Choose option:
                1 - display Customers in alphabetic order
                2 - display Customers which card number in interval of 'a' and 'n'
                """
            );

            choice = in.nextInt();

            switch (choice) {
                case 1 -> logCustomers(rmiServer.displayInAlphabetic());
                case 2 -> {
                    System.out.print("Enter 'a' value: ");
                    a = in.nextInt();

                    System.out.print("Enter 'b' value: ");
                    b = in.nextInt();

                    logCustomers(rmiServer.displayInIntervalOfCardNumber(a, b));
                }
            }
        } catch (RemoteException | NotBoundException | MalformedURLException e) {
            e.printStackTrace();
        }
    }
}
