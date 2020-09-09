#include "PolyphaseMergeSort.hpp"

#include <algorithm>

namespace ng {

    // constructor / destructor
    PolyphaseMergeSort::PolyphaseMergeSort(std::string dataPath, int filesCount, int chunkSize)
        : _dataPath(std::move(dataPath)), _currentTape(nullptr), _currentTapeIndex(0), _level(1) {

        this->_tapes.resize(filesCount, nullptr);
        this->_chunk.reserve(chunkSize);

    }

    PolyphaseMergeSort::~PolyphaseMergeSort() {

//        for (auto& tape : this->_tapes)
//            delete tape;

        for (int i = 0; i < this->_tapes.size(); ++i) {

            std::cout << i << " | size = " << this->_tapes[i]->size() << ", capacity = " << this->_tapes[i]->capacity() << std::endl;
            delete this->_tapes[i];

        }

    }

    // public methods
    void PolyphaseMergeSort::run() {

        std::ifstream infile(this->_dataPath);

        if (!infile.is_open())
            throw std::invalid_argument("failed to open the file | PolyphaseMergeSort::run");

        this->_initTapes();

        int number;

        for (int i = 0; !infile.eof(); ++i) {

            infile >> number;
            this->_chunk.emplace_back(number);

            std::cout << "number = " << number << std::endl;

            if (i + 1 == this->_chunk.capacity()) {

                // update capacity of tapes
                if (this->_currentTape->full()) {

                    this->_updateTapesCapacity();
                    this->_nullifyTape();

                }

                std::sort(this->_chunk.begin(), this->_chunk.end());

                // write data to tape
                this->_currentTape->write(this->_chunk);
                this->_currentTape->write("\n");
                this->_currentTape->incrementSize();

                // clear chunk
                this->_chunk.clear();

                // change tape if currentTape is full
                if (this->_currentTape->full())
                    this->_nextTape();

                // update i
                i = -1;

            }

        }

    }

    // private methods
    void PolyphaseMergeSort::_initTapes() {

        for (int i = 0; i < this->_tapes.size(); ++i)
            this->_tapes[i] = new Tape("../Files/" + std::to_string(i + 1) + ".txt", std::ios_base::out);

        this->_currentTape = this->_tapes.front();

    }

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

    void PolyphaseMergeSort::_updateTapesCapacity() {

        std::cout << "---update tape capacity" << std::endl;
        for (int i = 0; i < this->_tapes.size(); ++i)
            this->_tapes[i]->capacity(a(this->_level, i + 1, this->_tapes.size()));

        ++this->_level;

    }

    void PolyphaseMergeSort::_nullifyTape() {

        this->_currentTapeIndex = 0;
        this->_currentTape = this->_tapes[this->_currentTapeIndex];

    }

    void PolyphaseMergeSort::_nextTape() {

        ++this->_currentTapeIndex;
        this->_currentTape = this->_tapes[this->_currentTapeIndex];

        if (this->_currentTapeIndex == this->_tapes.size() - 1)
            this->_nullifyTape();

    }

} // namespace ng