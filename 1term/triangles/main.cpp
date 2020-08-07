#include <iostream>
#include <cmath>
#include <climits>


using namespace std;


int partition(int arr[], int start, int end) {

    int pivot = arr[end];
    int pivot_index = start - 1;

    for (int i = start; i < end; i++)
        if (arr[i] < pivot) {
            pivot_index++;
            swap(arr[pivot_index], arr[i]);
        }

    swap(arr[pivot_index + 1], arr[end]);

    return pivot_index + 1;

}


void quick_sort(int arr[], int start, int end) {

    if (start < end) {

        int pi = partition(arr, start, end);
        quick_sort(arr, start, pi - 1);
        quick_sort(arr, pi + 1, end);

    }

}


int main() {

    int n;
    int sides[100] = {0};
    bool answer = false;

    cin >> n;

    for (int i = 0; i < n; i++)
        cin >> sides[i];

    quick_sort(sides, 0, n - 1);

    for (int i = 0; i < n; i++)
        cout << sides[i] << " ";
    cout << "\nresult:\n\n";

    int prev_side_2 = 0;
    int prev_side_3 = 0;
//    int last_step;

    for (int i = 0; i < n - 2; i++) {

        if (sides[i] != sides[i - 1]) {
            for (int j = i + 1; j < n - 1; j++) {
                for (int k = j + 1; k < n; k++) {
                    if (sides[i] * sides[i] + sides[j] * sides[j] > sides[k] * sides[k] && (sides[j] != prev_side_2 || sides[k] != prev_side_3)) {

                        prev_side_2 = sides[j];
                        prev_side_3 = sides[k];
                        answer = true;
                        cout << "( " << sides[i] << ", " <<  sides[j] << ", " << sides[k] << " )\n";

                    }
                }
            }
        }

        prev_side_2 = 0;
        prev_side_3 = 0;

    }

    if (!answer)
        cout << "no triangles(\n";

    return 0;

}