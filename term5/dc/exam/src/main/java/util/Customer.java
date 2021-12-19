package util;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

public class Customer implements Serializable, Comparable<Customer> {

    private String  id;
    private String  name;
    private String  surname;
    private String  address;
    private Integer cardNumber;
    private String  bankAccount;

    public Customer(String id, String name, String surname, String address, Integer cardNumber, String bankAccount) {
        this.id = id;
        this.name = name;
        this.surname = surname;
        this.address = address;
        this.cardNumber = cardNumber;
        this.bankAccount = bankAccount;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getSurname() {
        return surname;
    }

    public void setSurname(String surname) {
        this.surname = surname;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public Integer getCardNumber() {
        return cardNumber;
    }

    public void setCardNumber(Integer cardNumber) {
        this.cardNumber = cardNumber;
    }

    public String getBankAccount() {
        return bankAccount;
    }

    public void setBankAccount(String bankAccount) {
        this.bankAccount = bankAccount;
    }

    @Override
    public int compareTo(Customer other) {
        return this.surname.compareTo(other.surname);
    }

    @Override
    public String toString() {
        return "id: " + id +
                "\nname: " + name +
                "\nsurname: " + surname +
                "\naddress: " + address +
                "\ncardNumber: " + cardNumber +
                "\nbankAccount: " + bankAccount;
    }

    public static List<Customer> getList() {
        return new ArrayList<>() {
            {
                add(new Customer("01", "Sasha", "Sashenkov",  "Kyiv",           77365278, "123453"));
                add(new Customer("02", "Pavel", "Volya",      "Kyiv",           12535422, "123453"));
                add(new Customer("03", "Igor",  "Kirichenko", "Kyiv",           37466352, "123453"));
                add(new Customer("04", "Max",   "Chi",        "Militopol",      14563635, "123453"));
                add(new Customer("10", "Alex",  "Centsov",    "Belaya Tserkov", 75237567, "123453"));
            }
        };
    }
}
