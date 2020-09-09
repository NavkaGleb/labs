#include <PolyphaseMergeSort.hpp>

int main() {

    ng::PolyphaseMergeSort mergeSort("../Files/data.txt", 6, 2);
    mergeSort.run();

    return 0;

}
