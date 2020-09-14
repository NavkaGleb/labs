#include <PolyphaseMergeSort.hpp>
#include <Random.hpp>

int main() {

    std::string inpath;
    std::string outpath;
    int filesCount;
    int chunkSize;

    std::cout << "enter path to data:" << std::endl;
    std::cin >> inpath;

    std::cout << "enter path for result:" << std::endl;
    std::cin >> outpath;

    std::cout << "enter files count" << std::endl;
    std::cin >> filesCount;

    std::cout << "enter chunk size" << std::endl;
    std::cin >> chunkSize;

    std::fstream outfile(inpath, std::ios_base::out | std::ios_base::binary);

    // huembers
    for (int i = 200000; i >= 1; --i) {

//        auto num = ng::random::irand<int>(0, 20);
//        std::cout << num << " ";
//        std::cout << i << " ";
        outfile.write(reinterpret_cast<char*>(&i), sizeof(i));

    }
    std::cout << std::endl;

    outfile.close();

    ng::PolyphaseMergeSort<int> mergeSort(inpath, outpath, filesCount, chunkSize);
    mergeSort.run();

    std::fstream infile(outpath, std::ios_base::in | std::ios_base::binary);

    if (!infile.is_open())
        std::cerr << "failed to open the file" << std::endl;

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

    return 0;

}