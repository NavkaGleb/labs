package rmi;

import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import util.Customer;

interface RMIServer extends Remote {

    List<Customer> displayInAlphabetic() throws RemoteException;
    List<Customer> displayInIntervalOfCardNumber(Integer a, Integer b) throws RemoteException;
}

public class ServerRmiTask2 {

    static class Service extends UnicastRemoteObject implements RMIServer {

        List<Customer> customers;

        Service() throws RemoteException {
            super();
            customers = Customer.getList();
        }

        @Override
        public List<Customer> displayInAlphabetic() {
            List<Customer> result = customers;

            Collections.sort(result);

            return result;
        }

        @Override
        public List<Customer> displayInIntervalOfCardNumber(Integer a, Integer b) {
            List<Customer> result = new ArrayList<>();

            for (Customer customer: customers) {
                if (customer.getCardNumber() > a && customer.getCardNumber() < b) {
                    result.add(customer);
                }
            }

            return result;
        }
    }

    public static void main(String[] args) throws RemoteException {
        Registry registry = LocateRegistry.createRegistry(123);
        RMIServer service = new Service();

        registry.rebind("exam", service);

        System.out.println("Server started!");
    }
}

