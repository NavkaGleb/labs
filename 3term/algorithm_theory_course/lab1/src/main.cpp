#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include <Tape.hpp>

int f(int i, int p) {

    if (0 <= i && i < p - 1)
        return 0;

    if (i == p - 1)
        return 1;

    int sum = 0;

    for (int j = i - 1; j >= i - p; --j)
        sum += f(j, p);

    return sum;

}

int a(int level, int i, int n) {

    int sum = 0;

    for (int o = level + n - 3; o >= level + i - 2; --o)
        sum += f(o, n - 1);

    return sum;

}

int main() {

    std::string filename;
    int filesCount = 6;
    int chunkSize = 2;
    int sum = 0;

//    std::cin >> filesCount;

    std::vector<ng::Tape*> tapes(filesCount, nullptr);

    for (int i = 0; i < tapes.size(); ++i)
        tapes[i] = new ng::Tape("../Files/" + std::to_string(i + 1) + ".txt", std::ios_base::out);

    std::ifstream infile("../Files/data.txt");

    if (!infile.is_open())
        throw std::invalid_argument("failed to open the file");

    int number;
    int level = 1;
    std::vector<int> chunk(chunkSize);

    for (int i = 0, currentTape = 0; !infile.eof(); ++i) {

        infile >> number;
        chunk[i] = number;

        std::cout << "number = " << number << std::endl;

        if (i + 1 == chunk.size()) {

            // update capacity of tapes
            if (tapes[currentTape]->full()) {

                std::cout << "---update tape capacity" << std::endl;
                for (int j = 0; j < tapes.size(); ++j)
                    tapes[j]->capacity(a(level, j + 1, tapes.size()));

                ++level;
                currentTape = 0;

            }

            // write data to tape
            std::cout << "write to tape -> " << currentTape << std::endl;
            for (const auto& e : chunk)
                std::cout << e << " ";
            std::cout << std::endl;
            tapes[currentTape]->write(chunk);
            tapes[currentTape]->write("\n");
            tapes[currentTape]->incrementSize();

            // clear chunk
            std::fill_n(chunk.begin(), chunk.size(), -666);

            // change tape if currentTape is full
            if (tapes[currentTape]->full()) {

                std::cout << "go to another tape" << std::endl;
                ++currentTape;

            }

            if (currentTape == tapes.size() - 1)
                currentTape = 0;

            // update i
            i = -1;

        }

    }

    for (int i = 0; i < tapes.size(); ++i) {

        std::cout << i << " | size = " << tapes[i]->size() << ", capacity = " << tapes[i]->capacity() << std::endl;
        delete tapes[i];

    }

    for (int lev = 1; lev <= 4; ++lev) {

        for (int i = 0; i < tapes.size(); ++i)
            std::cout << a(lev, i + 1, tapes.size()) << " ";
        std::cout << std::endl;

    }

    return 0;

}
