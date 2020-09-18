#include <PolyphaseMergeSort.hpp>
#include <Random.hpp>

#include "functional.hpp"

void generateReversedData(const std::string& filepath, const int& n) {

    std::ofstream outfile(filepath, std::ios_base::binary);

    if (!outfile.is_open())
        throw std::invalid_argument("failed to open the file | main::generateReversedData");

    // huembers
    for (int i = n; i >= 1; --i)
        outfile.write(reinterpret_cast<char*>(&i), sizeof(i));

    outfile.close();

}

template <typename T>
void generateRandomData(const std::string& filepath, const int& n, const T& left, const T& right) {

    std::ofstream outfile(filepath, std::ios_base::binary);

    if (!outfile.is_open())
        throw std::invalid_argument("failed to open the file | main::generateRandomData");

    T num;

    // huembers
    for (int i = n; i >= 1; --i) {

        num = ng::random::irand<T>(left, right);
        outfile.write(reinterpret_cast<char*>(&num), sizeof(num));

    }

    outfile.close();

}

void print(const std::string& filepath) {

    std::fstream infile(filepath, std::ios_base::in | std::ios_base::binary);

    if (!infile.is_open())
        throw std::invalid_argument("failed to open the file | main::print");


    int end = 0;
    int num = 0;

    infile.seekg(0, std::ios_base::end);
    end = infile.tellg();
    infile.seekg(0, std::ios_base::beg);

    while (infile.tellg() != end) {

        infile.read(reinterpret_cast<char*>(&num), sizeof(num));
        std::cout << num << " ";

    }

    std::cout << std::endl;
    infile.close();

}

int main() {

//    ng::functional::main();

    int numbers = 1000000;
    std::string inpath = "../files/data.bin";
    std::string outpath = "../files/result.bin";
    int filesCount = 5;
    int chunkSize = 100;
    bool init = false;

    std::cout << "do you want init variables or use default? [1/0]" << std::endl;
    std::cin >> init;

    if (init) {

        std::cout << "enter amount of numbers: ";
        std::cin >> numbers;

        std::cout << "enter path to data: ";
        std::cin >> inpath;

        std::cout << "enter path to result: ";
        std::cin >> outpath;

        std::cout << "enter files count: ";
        std::cin >> filesCount;

        std::cout << "enter chunk size: ";
        std::cin >> chunkSize;

    }


//    generateRandomData<int>(inpath, numbers, -10, 10);
    generateReversedData(inpath, numbers);

    ng::PolyphaseMergeSort<int> mergeSort(inpath, outpath, filesCount, chunkSize);
    mergeSort.run();

//    print(outpath);

    system("pause");

}