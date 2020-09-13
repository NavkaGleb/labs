#include <PolyphaseMergeSort.hpp>
#include <Random.hpp>

int main() {

    std::fstream outfile("../files/data.bin", std::ios_base::out | std::ios_base::binary);

    // huembers
    for (int i = 23; i >= 1; --i) {

        auto num = ng::random::frand<float>(0.f, 10.f);
        std::cout << num << " ";
        outfile.write(reinterpret_cast<char*>(&num), sizeof(num));

    }
    std::cout << std::endl;

    outfile.close();

    ng::PolyphaseMergeSort<int> mergeSort("../files/data.bin", "../files/result.bin", 5, 5);
    mergeSort.run();

    std::fstream infile("../files/result.bin", std::ios_base::in | std::ios_base::binary);

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