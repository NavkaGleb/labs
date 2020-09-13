#include <PolyphaseMergeSort.hpp>

int main() {

    std::fstream outfile("../files/numbers.bin", std::ios_base::out | std::ios_base::binary);

    // huembers
    for (int i = 15; i >= 1; --i)
        outfile.write(reinterpret_cast<char*>(&i), sizeof(i));

    outfile.close();

    ng::PolyphaseMergeSort mergeSort("../files/numbers.bin", 3, 2);
    mergeSort.run();

    std::cout << "!!!!!-----------------------------!!!!" << std::endl;
    std::fstream infile("../files/0.bin", std::ios_base::in | std::ios_base::binary);
    infile.seekg(0, std::ios_base::end);
    int end = infile.tellg();
    infile.seekg(0, std::ios_base::beg);
    int num;
    while (infile.tellg() != end) {

        infile.read(reinterpret_cast<char*>(&num), sizeof(num));
        std::cout << num << " ";

    }
    std::cout << std::endl;
    infile.close();

    infile.open("../files/1.bin", std::ios_base::in | std::ios_base::binary);
    infile.seekg(0, std::ios_base::end);
    end = infile.tellg();
    infile.seekg(0, std::ios_base::beg);
    while (infile.tellg() != end) {
        infile.read(reinterpret_cast<char*>(&num), sizeof(num));
        std::cout << num << " ";
    }
    std::cout << std::endl;
    infile.close();

    infile.open("../files/2.bin", std::ios_base::in | std::ios_base::binary);
    infile.seekg(0, std::ios_base::end);
    end = infile.tellg();
    infile.seekg(0, std::ios_base::beg);
    while (infile.tellg() != end) {
        infile.read(reinterpret_cast<char*>(&num), sizeof(num));
        std::cout << num << " ";
    }
    std::cout << std::endl;
    infile.close();

    return 0;

}