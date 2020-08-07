#include <iostream>
#include <string>
#include <iomanip>                          // for output numbers with 0 before
#include <map>
#include <istream>
#include <fstream>
#include <climits>

using namespace std;

int cache[50] = {0};
map<string,int> map_brackets;

int pow(int number, int exponent) {

    if (exponent == 0)
        return 1;
    if (exponent == 1)
        return number;
    if (exponent % 2 == 0)
        return pow(number * number, exponent / 2);
    return number * pow(number, exponent - 1);

}

int factorial(int n) {

    if (n == 0)
        return 1;
    return n * factorial(n - 1);

}

int fibonacci_numbers(int number) {

    if (number <= 1)
        return number;
    if (cache[number] == 0)
        cache[number] = fibonacci_numbers(number - 1) + fibonacci_numbers(number - 2);

    return cache[number];

    //return fibonacci_numbers(number - 1) + fibonacci_numbers(number - 2);

}

int greatest_common_divisor(int a, int b) {

    if (b == 0)
        return a;
    return greatest_common_divisor(b, a % b);

}

void hanoi(int amount, int start, int finish) {

    if (amount == 1)
        cout << "move disk 1 from " << start << " to " << finish << endl;
    else {

        int temp = 6 - start - finish;

        hanoi(amount - 1, start, temp);
        cout << "move disk " << amount << " from " << start << " to " << finish << endl;
        hanoi(amount - 1, temp, finish);

    }

}

void brackets(int n) {

    if (n == 0) {
        map_brackets.emplace("", 0);
        return;
    }

    brackets(n-1);

    for (auto m_pair = map_brackets.begin(); m_pair != map_brackets.end(); m_pair++) {
        if ((n - 1) == (*m_pair).second) {

            string prev_brackets_combination = (*m_pair).first;

            map_brackets.emplace("()" + prev_brackets_combination, n);
            map_brackets.emplace("(" + prev_brackets_combination + ")", n);
            map_brackets.emplace(prev_brackets_combination + "()", n);

        }
    }

}

void print_brackets(int n) {

    cout << "there are all valid brackets combinations for '" << n << "':\n\n";

    int i = 1;
    for (auto &m_pair : map_brackets) {
        if (m_pair.second == n) {
            cout << setfill('0') << setw(2) << i << ". " << m_pair.first << endl;
            i++;
        }
    }

}

int b_search_r(int *array, int start, int end, int x) {

    if (start <= end) {

        int middle = (start + end) / 2;

        if (x == array[middle])
            return middle;
        else if (x > array[middle] && start <= end)
            return b_search_r(array, middle + 1, end, x);
        else if (x < array[middle] && start <= end)
            return b_search_r(array, start, middle - 1, x);

    }

    return -1;

}

int b_search(const int *array, int start, int end, int x) {

    static int middle;

    while (start <= end) {

        middle = (start + end) / 2;

        if (x == array[middle])
        return middle;
        else if (x > array[middle])
        start = middle + 1;
        else if (x < array[middle])
        end = middle - 1;

    }

    return -1;

}

int main() {

    int n;
    int x;

    cin >> n;
    cin >> x;

    int *array = new int[n];

    for (int i = 0; i < n; i++)
        *(array + i) = i + 1;

    cout << "index = " << b_search_r(array, 0, n - 1, x) << endl;

    delete[] array;

    return 0;

}