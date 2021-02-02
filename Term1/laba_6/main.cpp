#include <iostream>
#include <fstream>
#include <random>

#define INT_SIZE 4
#define ARR_SIZE 25

using namespace std;

struct long_num {

    int value[ARR_SIZE] = {0};
    int length = 1;

    int len() {
        for (int i = 0; i < ARR_SIZE; i++)
            if (value[i] != 0)
                return ARR_SIZE - i;
        return 1;
    }

    void output() {
        for (int i = ARR_SIZE - length; i < ARR_SIZE; i++)
            cout << value[i];
    }

    void zeroing() {
        for (int i = ARR_SIZE - 1; i >= 0; i--)
            value[i] = 0;
        length = 1;
    }

};

void fill_in_file(const char*);
void long_amount(const char*);

int main() {

    // 1. when amount of all numbers isn't big number, so I can divide all numbers by amount
    // 2. when amount of all numbers is bid number, so I need to create division of long numbers ( sum and amount )

    const char* file_name = "../bin_file.txt";

    fill_in_file(file_name);
    long_amount(file_name);

    return 0;

}

void fill_in_file(const char* file_name) {

    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> random(-10, 10);

    int random_number;

    ofstream file;
    file.open(file_name, ios::binary | ios::out);

    if (file.fail()) {

        cout << "error opening file\n";
        return;

    } else {

        int n;
        cout << "please, enter amount of numbers:\n";
        cin >> n;

//        for (int i = 0; i < n; i++) {
//            cin >> random_number;
//            file.write((char*)&random_number, 1);
//        }

        for (int i = 0; i < n; i++) {
            random_number = random(mt);
            cout << random_number << endl;
            file.write((char*)&random_number, INT_SIZE);
        }

    }

    file.close();

    cout << "random numbers were successfully written to the bin_file.txt\ns";

}

void add_one(long_num& num) {

    int transfer = 0;

    num.value[ARR_SIZE - 1] += 1;
    transfer = num.value[ARR_SIZE - 1] / 10;
    num.value[ARR_SIZE - 1] %= 10;

    for (int i = ARR_SIZE - 2; i >= 0; i--) {

        num.value[i] += transfer;
        transfer = num.value[i] / 10;
        num.value[i] %= 10;

    }

}

void subtraction_abs(long_num a, long_num b, long_num &remainder) {

    int transfer = 0;
    int minuend;

    for (int i = ARR_SIZE - 1; i >= 0; i--) {

        minuend = a.value[i] - transfer - b.value[i];

        if (minuend < 0) {

            minuend += 10;
            remainder.value[i] = minuend % 10;
            transfer = 1;

        } else {

            remainder.value[i] = minuend % 10;
            transfer = 0;

        }

    }

    remainder.length = remainder.len();

}

int compare_abs(long_num a, long_num b) {

    // 1 = a > b
    // 0 = a == b
    // -1 = a < b

    if (a.length > b.length)
        return 1;
    if (a.length < b.length)
        return -1;

    for (int i = ARR_SIZE - a.length; i < ARR_SIZE; i++) {

        if (a.value[i] > b.value[i])
            return 1;
        if (a.value[i] < b.value[i])
            return -1;

    }

    return 0;

}

void equate_long_num(long_num& b, long_num a) {

    b.length = a.length;

    for (int i = 0; i < ARR_SIZE; i++) {

        if (i >= ARR_SIZE - a.length)
            b.value[i] = a.value[i];
        else
            b.value[i] = 0;

    }

}

void division(long_num sum, long_num amount, long_num& result, long_num& remainder) {

    int counter = 0;

    for (int i = ARR_SIZE - sum.length; i < ARR_SIZE; i++) {

        for (int j = ARR_SIZE - remainder.length; j < ARR_SIZE; j++)
            remainder.value[j - 1] = remainder.value[j];

        remainder.value[ARR_SIZE - 1] = sum.value[i];
        remainder.length = remainder.len();
        counter = 0;

        while (compare_abs(remainder, amount) >= 0)
            subtraction_abs(remainder, amount, remainder), counter++;

        result.value[i] = counter;

    }

    result.length = result.len();

}

bool equal_zero(long_num num) {

    return num.length == 1 && num.value[ARR_SIZE - 1] == 0;

}

void convert_remainder(long_num remainder, long_num amount, long_num& decimal_remainder) {

    long_num remainder_1;
    long_num result;

    for (int i = 0; i < 10 && !equal_zero(remainder); i++) {

        for (int j = ARR_SIZE - remainder.length; j < ARR_SIZE; j++)
            remainder.value[j - 1] = remainder.value[j];

        remainder.value[ARR_SIZE - 1] = 0;
        remainder.length = remainder.len();

        division(remainder, amount, result, remainder_1);
        equate_long_num(remainder, remainder_1);

        for (int j = ARR_SIZE - decimal_remainder.length; j < ARR_SIZE; j++)
            decimal_remainder.value[j - 1] = decimal_remainder.value[j];

        decimal_remainder.value[ARR_SIZE - 1] = result.value[ARR_SIZE - 1];
        decimal_remainder.length = i + 1;

        result.zeroing();
        remainder_1.zeroing();

    }

}

void long_average(long_num sum, long_num amount, long_num& result, long_num& remainder) {

    long_num decimal_remainder;

    division(sum, amount, result, remainder);
    convert_remainder(remainder, amount, decimal_remainder);

    cout << "\naverage = ";
    result.output();
    cout << '.';
    decimal_remainder.output();

}

void long_amount(const char* file_name) {

    int current_number;
    int transfer = 0;
    int digit = 0;
    long_num sum;
    long_num amount_of_numbers;
    long_num result;
    long_num remainder;

    ifstream file(file_name, ios::binary | ios::in);

    if (file.fail()) {

        cout << "error opening file\n";
        return;

    } else {

        while (file.read((char*)&current_number, INT_SIZE)) {

            if (current_number > 0) {

                for (int i = ARR_SIZE - 1; i >= 0; i--) {

                    digit = current_number % 10;
                    current_number /= 10;

                    sum.value[i] += digit + transfer;
                    transfer = sum.value[i] / 10;
                    sum.value[i] %= 10;

                }

                add_one(amount_of_numbers);
                transfer = 0;

            }

        }

        amount_of_numbers.length = amount_of_numbers.len();
        sum.length = sum.len();

        long_average(sum, amount_of_numbers, result, remainder);

    }

}


