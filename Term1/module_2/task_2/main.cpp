#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct custom_vector {

    vector<double> points;
    int length = 0;

    void input() {

        double cur_p;

        cout << "please, enter length of vector: ";
        cin >> length;

        for (int i = 0; i < length; i++) {

            cin >> cur_p;
            points.push_back(cur_p);

        }

    }

    void output() {

        cout << "( ";
        for (int i = 0; i < length - 1; i++)
            cout << points[i] << ", ";
        cout << points[length - 1] << " )" << endl;

    }

    custom_vector add(custom_vector a) {

        custom_vector new_vector;
        new_vector.length = length;

        for (int i = 0; i < length; i++)
            new_vector.points.push_back(points[i] + a.points[i]);

        return new_vector;

    }

    double scalar_product(custom_vector a) {

        double result = 0;

        for (int i = 0; i < length; i++)
            result += points[i] * a.points[i];

        return result;

    }

    double norma() {

        double sum = 0;

        for (auto i : points)
            sum += i * i;

        return sqrt(sum);

    }

};

int main() {

    custom_vector v;
    custom_vector a;

    v.input();
    a.input();

    v.add(a).output();

    cout << v.scalar_product(a) << endl;
    cout << v.norma() << endl;

    return 0;

}
