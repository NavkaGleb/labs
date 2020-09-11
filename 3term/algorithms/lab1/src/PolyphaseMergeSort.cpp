#include "PolyphaseMergeSort.hpp"

#include <algorithm>
#include <map>
#include <functional>

namespace ng {

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

    // constructor / destructor
    PolyphaseMergeSort::PolyphaseMergeSort(std::string dataPath, int filesCount, int chunkSize)
        : _dataPath(std::move(dataPath)), _ctape(0), _level(1) {

        this->_tapes.resize(filesCount, nullptr);
        this->_chunk.reserve(chunkSize);

    }

    PolyphaseMergeSort::~PolyphaseMergeSort() {

//        for (auto& tape : this->_tapes)
//            delete tape;

        for (int i = 0; i < this->_tapes.size(); ++i) {

            std::cout << i << " | chunks = " << this->_tapes[i]->chunks() << ", capacity = " << this->_tapes[i]->capacity() << std::endl;
            delete this->_tapes[i];

        }

        std::cout << "---" << std::endl;
        for (int level = 1; level <= 4; ++level) {

            for (int i = 0; i < this->_tapes.size(); ++i)
                std::cout << a(level, i + 1, this->_tapes.size()) << " ";
            std::cout << std::endl;

        }

    }

    // public methods
    void PolyphaseMergeSort::run() {

        std::ifstream infile(this->_dataPath, std::ios_base::binary);

        if (!infile.is_open())
            throw std::invalid_argument("failed to open the file | PolyphaseMergeSort::run");

        int number;
        int eof;

        // init eof
        infile.seekg(0, std::ios_base::end);
        eof = infile.tellg();
        infile.seekg(0, std::ios_base::beg);

        this->_initTapes();

        for (int i = 0; infile.tellg() != eof; ++i) {

            infile.read(reinterpret_cast<char*>(&number), sizeof(number));
            this->_chunk.emplace_back(number);

            if (i + 1 == this->_chunk.capacity()) {

                // update capacity of tapes
                if (this->_tapes[this->_ctape]->full()) {

                    this->_updateTapesCapacity();
                    this->_nullifyTape();

                }

                // sort data in memory
                std::sort(this->_chunk.begin(), this->_chunk.end());

                // write data to tape
                this->_tapes[this->_ctape]->write(this->_chunk);
                this->_tapes[this->_ctape]->incrementChunks();

                // clear chunk
                this->_chunk.clear();

                // change tape if currentTape is full
                if (this->_tapes[this->_ctape]->full())
                    this->_nextTape();

                // update i
                i = -1;

            }

        }

        if (!this->_chunk.empty()) {

            if (this->_tapes[this->_ctape]->full())
                this->_nextTape();

            if (this->_tapes[this->_ctape]->full()) {

                this->_updateTapesCapacity();
                this->_nullifyTape();

            }

            // sort data in memory
            std::sort(this->_chunk.begin(), this->_chunk.end());

            // write data to tape
            this->_tapes[this->_ctape]->write(this->_chunk);
            this->_tapes[this->_ctape]->incrementChunks();

        }

        for (auto& tape : this->_tapes) {

            tape->close();
            tape->open(std::ios_base::in | std::ios_base::binary);
            tape->initDummy();

        }

        this->_merge();

    }

    // private methods
    void PolyphaseMergeSort::_initTapes() {

        for (int i = 0; i < this->_tapes.size(); ++i)
            this->_tapes[i] = new Tape(
                "../files/" + std::to_string(i) + ".bin",
                std::ios_base::out | std::ios_base::binary,
                this->_chunk.capacity()
            );

    }

    void PolyphaseMergeSort::_updateTapesCapacity() {

        for (int i = 0; i < this->_tapes.size(); ++i)
            this->_tapes[i]->capacity(a(this->_level, i + 1, this->_tapes.size()));

        ++this->_level;

    }

    void PolyphaseMergeSort::_nullifyTape() {

        this->_ctape = 0;

    }

    void PolyphaseMergeSort::_nextTape() {

        ++this->_ctape;

        if (this->_ctape == this->_tapes.size() - 1)
            this->_nullifyTape();

    }

    void PolyphaseMergeSort::_merge(const int& empty, const bool& initChunkSize) {

        std::cout << "--merge chunks " << empty << std::endl;

        struct Peak {

            int value;
            int tape;

            explicit Peak(int tape = -1) : value(0), tape(tape) {}
            virtual ~Peak() = default;

        };

        static std::function<bool(const Peak&, const Peak&)> sortFunction = [](const Peak& peak1, const Peak& peak2) {
            return peak1.value > peak2.value;
        };

        std::vector<Peak> peaks;
        std::vector<int> infile;

        std::cout << "INIT peaks !" << std::endl;

        for (int i = 0; i < this->_tapes.size(); ++i) {

//            std::cout << this->_tapes[i]->filename() << " -> " this->_tapes[i]->eoc() <<

            if (empty != i && !this->_tapes[i]->eof()) {

                peaks.emplace_back(Peak(i));
                this->_tapes[i]->read(peaks.back().value);
                std::cout << "\ttape " << i << " -> " << peaks.back().value << "(pos: " << this->_tapes[i]->pos() << ")" << std::endl;

            }

        }

        std::cout << std::endl;

        while (!peaks.empty()) {

            // sort peaks
            std::sort(peaks.begin(), peaks.end(), sortFunction);

            std::cout << "peaks AFTER SORTING !!!!" << std::endl;
            for (const auto& peak : peaks)
                std::cout << "\ttape " << peak.tape << " -> " << peak.value << "(pos: " << this->_tapes[peak.tape]->pos() << ")" << std::endl;
            std::cout << std::endl;

            // write to file
            std::cout << "write to file << " << peaks.back().value << "(" << peaks.back().tape << ")" << std::endl;
            this->_tapes[empty]->write(peaks.back().value);
            infile.emplace_back(peaks.back().value);

            // get new number
            if (!peaks.empty() && !this->_tapes[peaks.back().tape]->eoc(peaks.back().value)) {

                this->_tapes[peaks.back().tape]->read(peaks.back().value);
                std::cout << "\tpush new tape " << peaks.back().tape << " -> " << peaks.back().value << "(pos: " << this->_tapes[peaks.back().tape]->pos() << ")" << std::endl;

            } else {

                std::cout << "\t peak " << peaks.back().tape << " = -1" << std::endl;
                peaks.back().tape = -1;

            }

            for (int i = static_cast<int>(peaks.size()) - 1; i >= 0; --i) {

                if (peaks[i].tape == -1)
                    peaks.pop_back();
                else
                    break;

            }

        }

        std::cout << "-- number in empty file\t\t";
        for (int i : infile)
            std::cout << i << " ";
        std::cout << std::endl;

        if (initChunkSize)
            this->_tapes[empty]->chunkSize(infile.size());

    }

    void PolyphaseMergeSort::_merge() {

        std::cout << "--merge" << std::endl;

        bool sorted = false;
        int empty = -1;
        int min = -1;
        int level = 1;

        while (!sorted) {

            ++level;

            std::cout << "MERGE LEVEL = " << level << std::endl;

            std::cout << "\ttapes in begin of loop" << std::endl;
            for (int i = 0; i < this->_tapes.size(); ++i)
                std::cout << i << " | chunks = " << this->_tapes[i]->chunks() << ", capacity = " << this->_tapes[i]->capacity() << std::endl;

            // search min chunks and empty file
            for (int i = 0; i < this->_tapes.size(); ++i) {

                if (this->_tapes[i]->empty()) {

                    if (empty == -1)
                        empty = i;
                    else
                        sorted = true;

                } else if (min == -1 || this->_tapes[min]->capacity() > this->_tapes[i]->capacity()) {

                    min = i;

                }

                if (sorted)
                    std::cout << "full file -> " << min << "!!!!!!!!!!!!!!!!!!!" << std::endl;

            }

            if (!sorted) {

                this->_tapes[empty]->close();
                this->_tapes[empty]->open(std::ios_base::out | std::ios_base::trunc);
                this->_tapes[empty]->close();

                this->_tapes[empty]->open(std::ios_base::out | std::ios_base::binary);
                this->_tapes[empty]->capacity(this->_tapes[min]->capacity());
                this->_tapes[empty]->chunks(this->_tapes[min]->capacity());

//                int sum = 0;
//                for (int i = 0; i < this->_tapes.size(); ++i)
//                    if (i != empty)
//                        sum += this->_tapes[i]->chunkSize();
//
//                std::cout << "\t\t\t\t\t\tSUM == " << sum << std::endl;
//
//                this->_tapes[empty]->chunkSize(sum / 4);

                std::cout << "empty file = " << empty << std::endl;
                std::cout << "min capacity = " << this->_tapes[min]->capacity() << std::endl;

                int minCapacity = this->_tapes[min]->capacity();

                for (int i = 0; i < minCapacity; ++i) {

                    this->_merge(empty, i == 0);

                    for (int j = 0; j < this->_tapes.size(); ++j)
                        if (j != empty)
                            this->_tapes[j]->decrementCapacity();

                    std::cout << "-- chunks after MERGE ! " << std::endl;
                    for (int j = 0; j < this->_tapes.size(); ++j)
                        std::cout << j << " | chunks = " << this->_tapes[j]->chunks() << ", capacity = " << this->_tapes[j]->capacity() << std::endl;
                    std::cout << std::endl;

                }

                // open input file and init chunkCapacity and size and capacity of tape
                this->_tapes[empty]->close();
                this->_tapes[empty]->open(std::ios_base::in | std::ios_base::binary);

                std::cout << "-- chunks after ALL" << std::endl;
                for (int i = 0; i < this->_tapes.size(); ++i)
                    std::cout << i << " | chunks = " << this->_tapes[i]->chunks() << ", capacity = " << this->_tapes[i]->capacity() << std::endl;

            }

            empty = -1;
            min = -1;

        }

    }

} // namespace ng