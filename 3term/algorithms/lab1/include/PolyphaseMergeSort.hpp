#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <functional>

#include <Tape.hpp>

namespace ng {

    template <typename T>
    class PolyphaseMergeSort {
    public:
        // constructor / destructor
        PolyphaseMergeSort(std::string inpath, std::string outpath, int filesCount, int chunkSize);
        virtual ~PolyphaseMergeSort();

        // public methods
        void run();

    private:
        // variables
        std::string _inpath;
        std::string _outpath;
        std::vector<Tape<T>*> _tapes;
        int _ctape;
        std::vector<T> _chunk;
        int _level;
        int _result;

        // private methods
        void _updateTapesCapacity();
        void _nullifyTape();
        void _nextTape();
        void _writeChunk();
        void _initTapes();
        void _merge(const int& empty);
        void _merge();
        void _renameTape();
        void _deleteTapes();

    }; // class PolyphaseMergeSort

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
    template <typename T>
    PolyphaseMergeSort<T>::PolyphaseMergeSort(std::string inpath, std::string outpath, int filesCount, int chunkSize)
            : _inpath(std::move(inpath)), _outpath(std::move(outpath)), _ctape(0), _level(1), _result(-1) {

        this->_tapes.resize(filesCount, nullptr);
        this->_chunk.reserve(chunkSize);

    }

    template <typename T>
    PolyphaseMergeSort<T>::~PolyphaseMergeSort() {

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
    template <typename T>
    void PolyphaseMergeSort<T>::run() {

        this->_initTapes();
        this->_merge();
        this->_renameTape();
        this->_deleteTapes();

    }

    // private methods
    template <typename T>
    void PolyphaseMergeSort<T>::_updateTapesCapacity() {

        for (int i = 0; i < this->_tapes.size(); ++i)
            this->_tapes[i]->capacity(a(this->_level, i + 1, this->_tapes.size()));

        ++this->_level;

    }

    template <typename T>
    void PolyphaseMergeSort<T>::_nullifyTape() {

        this->_ctape = 0;

    }

    template <typename T>
    void PolyphaseMergeSort<T>::_nextTape() {

        ++this->_ctape;

        if (this->_ctape == this->_tapes.size() - 1)
            this->_nullifyTape();

    }

    template <typename T>
    void PolyphaseMergeSort<T>::_writeChunk() {

        if (this->_tapes[this->_ctape]->full()) {

            this->_updateTapesCapacity();
            this->_nullifyTape();

        }

        std::sort(this->_chunk.begin(), this->_chunk.end());

        this->_tapes[this->_ctape]->write(this->_chunk);
        this->_tapes[this->_ctape]->incrementChunks();

        this->_chunk.clear();

    }

    template <typename T>
    void PolyphaseMergeSort<T>::_initTapes() {

        for (int i = 0; i < this->_tapes.size(); ++i)
            this->_tapes[i] = new Tape<T>(
                    "../files/" + std::to_string(i) + ".bin",
                    std::ios_base::out | std::ios_base::binary
            );

        std::ifstream infile(this->_inpath, std::ios_base::binary);

        if (!infile.is_open())
            throw std::invalid_argument("failed to open the file | template <typename T>PolyphaseMergeSort<T>::run");

        T number;
        int eof;

        infile.seekg(0, std::ios_base::end);
        eof = infile.tellg();
        infile.seekg(0, std::ios_base::beg);

        for (int i = 0; infile.tellg() != eof; ++i) {

            infile.read(reinterpret_cast<char*>(&number), sizeof(number));
            this->_chunk.emplace_back(number);

            if (i == this->_chunk.capacity() - 1) {

                this->_writeChunk();

                if (this->_tapes[this->_ctape]->full())
                    this->_nextTape();

                i = -1;

            }

        }

        if (!this->_chunk.empty())
            this->_writeChunk();

        for (auto& tape : this->_tapes) {

            tape->close();
            tape->open(std::ios_base::in | std::ios_base::binary);

        }

    }

    template <typename T>
    void PolyphaseMergeSort<T>::_merge(const int& empty) {

        struct Peak {

            T value;
            int tape;

            explicit Peak(int tape = -1) : value(0), tape(tape) {}
            virtual ~Peak() = default;

        };

        static std::function<bool(const Peak&, const Peak&)> sortFunction = [](const Peak& peak1, const Peak& peak2) {

            return peak1.value > peak2.value;

        };

        std::vector<Peak> peaks;

        for (int i = 0; i < this->_tapes.size(); ++i) {

            if (empty != i && !this->_tapes[i]->eof()) {

                peaks.emplace_back(Peak(i));
                this->_tapes[i]->read(peaks.back().value);

            }

        }

        while (!peaks.empty()) {

            if (peaks.size() == 1) {

                this->_tapes[empty]->write(peaks.back().value);

                while (!this->_tapes[peaks.back().tape]->eoc(peaks.back().value)) {

                    this->_tapes[peaks.back().tape]->read(peaks.back().value);
                    this->_tapes[empty]->write(peaks.back().value);

                }

                return;

            }

            std::sort(peaks.begin(), peaks.end(), sortFunction);

            this->_tapes[empty]->write(peaks.back().value);

            if (!this->_tapes[peaks.back().tape]->eoc(peaks.back().value))
                this->_tapes[peaks.back().tape]->read(peaks.back().value);
            else
                peaks.pop_back();

        }

    }

    template <typename T>
    void PolyphaseMergeSort<T>::_merge() {

        bool sorted = false;
        int empty = -1;
        int min = -1;

        while (!sorted) {

            for (int i = 0; i < this->_tapes.size(); ++i) {

                if (this->_tapes[i]->empty())
                    empty == -1 ? empty = i : sorted = true;
                else if (min == -1 || this->_tapes[min]->capacity() > this->_tapes[i]->capacity())
                    min = i;

            }

            if (!sorted) {

                this->_tapes[empty]->close();
                this->_tapes[empty]->open(std::ios_base::out | std::ios_base::binary);

                this->_tapes[empty]->capacity(this->_tapes[min]->capacity());
                this->_tapes[empty]->chunks(this->_tapes[min]->capacity());

                for (int i = 0; i < this->_tapes[empty]->capacity(); ++i)
                    this->_merge(empty);

                for (int j = 0; j < this->_tapes.size(); ++j)
                    if (j != empty)
                        this->_tapes[j]->decrementCapacity(this->_tapes[empty]->capacity());

                this->_tapes[empty]->close();
                this->_tapes[empty]->open(std::ios_base::in | std::ios_base::binary);

                empty = -1;
                min = -1;

            }

        }

        for (auto& tape : this->_tapes)
            tape->close();

        this->_result = min;

    }

    template <typename T>
    void PolyphaseMergeSort<T>::_renameTape() {

        if (std::fstream(this->_outpath))
            std::remove(this->_outpath.c_str());

        std::rename(this->_tapes[this->_result]->filename().c_str(), this->_outpath.c_str());

    }

    template <typename T>
    void PolyphaseMergeSort<T>::_deleteTapes() {

        for (int i = 0; i < this->_tapes.size(); ++i)
            if (i != this->_result && std::remove(this->_tapes[i]->filename().c_str()) != 0)
                std::cerr << "failed to delete " << this->_tapes[i]->filename() << std::endl;

    }

} // namespace ng