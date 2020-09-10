#include <PolyphaseMergeSort.hpp>

int main() {

    ng::PolyphaseMergeSort mergeSort("../Files/data.txt", 4, 2);
    mergeSort.run();

    return 0;

}
