#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <functional>
#include <chrono>

#include <Tape.hpp>

namespace ng {

    // header
    template <typename T>
    class PolyphaseMergeSort {
    public:
        // constructor / destructor
        PolyphaseMergeSort(std::string inpath, std::string outpath, int filesCount, int chunkSize);
        virtual ~PolyphaseMergeSort();

        // public methods
        void run();

    private:
        // inner structs
        struct Peak {
            // variables
            T value;
            int tape;

            // constructor / destructor
            explicit Peak(int tape = -1) : value(0), tape(tape) {}
            virtual ~Peak() = default;

        };

        // variables
        std::string _inpath;
        std::string _outpath;
        std::vector<Tape<T>*> _tapes;
        std::vector<T> _chunk;
        int _result;
        std::chrono::time_point<std::chrono::high_resolution_clock> _start;

        // private methods
        void _readChunk(std::fstream& file, const int& size);
        void _initTapesCapacity(int min);
        void _initTapes();
        void _merge(const int& empty);
        void _sort();
        void _renameTape();
        void _deleteTapes();

    }; // class PolyphaseMergeSort

    // source

    // constructor / destructor
    template <typename T>
    PolyphaseMergeSort<T>::PolyphaseMergeSort(std::string inpath, std::string outpath, int filesCount, int chunkSize)
        : _inpath(std::move(inpath)), _outpath(std::move(outpath)), _result(-1) {

        this->_tapes.resize(filesCount, nullptr);
        this->_chunk.reserve(chunkSize);

        this->_start = std::chrono::high_resolution_clock::now();

    }

    template <typename T>
    PolyphaseMergeSort<T>::~PolyphaseMergeSort() {

        for (auto& tape : this->_tapes)
            delete tape;

    }

    // public methods
    template <typename T>
    void PolyphaseMergeSort<T>::run() {

        using namespace std::chrono;

        auto start = time_point_cast<milliseconds>(this->_start).time_since_epoch().count();

        this->_initTapes();
        time_point<high_resolution_clock> endTimePoint = high_resolution_clock::now();
        auto end = time_point_cast<milliseconds>(endTimePoint).time_since_epoch().count();
        std::cout << "end init tapes " << static_cast<double>(end - start) * 0.001 << std::endl;

        this->_sort();
        endTimePoint = high_resolution_clock::now();
        end = time_point_cast<milliseconds>(endTimePoint).time_since_epoch().count();
        std::cout << "end sort " << static_cast<double>(end - start) * 0.001 << std::endl;

        this->_renameTape();
        endTimePoint = high_resolution_clock::now();
        end = time_point_cast<milliseconds>(endTimePoint).time_since_epoch().count();
        std::cout << "rename tape " << static_cast<double>(end - start) * 0.001 << std::endl;

        this->_deleteTapes();
        endTimePoint = high_resolution_clock::now();
        end = time_point_cast<milliseconds>(endTimePoint).time_since_epoch().count();
        std::cout << "delete tapes " << static_cast<double>(end - start) * 0.001 << std::endl;

    }

    // private methods
    template <typename T>
    void PolyphaseMergeSort<T>::_readChunk(std::fstream& file, const int& size) {

        this->_chunk.clear();
        this->_chunk.resize(size);

        for (int i = 0; i < this->_chunk.size(); ++i)
            file.read(reinterpret_cast<char*>(&this->_chunk[i]), sizeof(this->_chunk[i]));

    }

    template <typename T>
    void PolyphaseMergeSort<T>::_initTapesCapacity(int min) {

        int sum = 1;
        int maxIndex = 0;
        int max;

        this->_tapes[0]->capacity(1);

        while (sum <= min) {

            for (int i = 0; i < this->_tapes.size(); ++i)
                if (this->_tapes[maxIndex]->capacity() < this->_tapes[i]->capacity())
                    maxIndex = i;

            max = this->_tapes[maxIndex]->capacity();

            for (int i = 0; i < this->_tapes.size(); ++i)
                i == maxIndex ? this->_tapes[i]->capacity(0) : this->_tapes[i]->incrementCapacity(max);

            sum += max * (this->_tapes.size() - 2);
            maxIndex = 0;

        }

        for (int i = 0; i < this->_tapes.size(); ++i)
            std::cout << i << " -> " << this->_tapes[i]->capacity() << std::endl;
        std::cout << std::endl;

    }

    template <typename T>
    void PolyphaseMergeSort<T>::_initTapes() {

        for (int i = 0; i < this->_tapes.size(); ++i)
            this->_tapes[i] = new Tape<T>(
                "../files/" + std::to_string(i) + ".bin",
                std::ios_base::out | std::ios_base::binary
            );

        std::fstream infile(this->_inpath, std::ios_base::in | std::ios_base::binary);

        if (!infile.is_open())
            throw std::invalid_argument("failed to open the file | template <typename T>PolyphaseMergeSort<T>::run");

        int numbers;
        int chunks = 0;
        int chunkSize;
        int oversizeChunks;

        infile.seekg(0, std::ios_base::end);
        numbers = infile.tellg() / sizeof(T);
        infile.seekg(0, std::ios_base::beg);

        this->_initTapesCapacity((numbers + this->_chunk.capacity() - 1) / this->_chunk.capacity());

        for (const auto& tape : this->_tapes)
            chunks += tape->capacity();

        chunkSize = numbers / chunks;
        oversizeChunks = numbers % chunks;

        for (auto& tape : this->_tapes) {

            for (int i = 0; i < tape->capacity(); ++i) {

                if (oversizeChunks != 0) {

                    this->_readChunk(infile, chunkSize + 1);
                    std::sort(this->_chunk.begin(), this->_chunk.end());
                    tape->write(this->_chunk);
                    --oversizeChunks;

                } else {

                    this->_readChunk(infile, chunkSize);
                    std::sort(this->_chunk.begin(), this->_chunk.end());
                    tape->write(this->_chunk);

                }

            }

            tape->close();
            tape->open(std::ios_base::in | std::ios_base::binary);

        }

    }

    template <typename T>
    void PolyphaseMergeSort<T>::_merge(const int& empty) {

        std::cout << "merge " << empty << std::endl;

        std::vector<Peak> peaks(this->_tapes.size());

        for (int i = 0; i < peaks.size(); ++i) {

            if (i != empty) {

                if (this->_tapes[i]->eof()) {

                    std::cout << "end of file " << i << std::endl;
                    peaks[i] = Peak();

                } else {
                    peaks[i] = Peak(i);
                    this->_tapes[i]->read(peaks[i].value);
                }

            }

        }

        int min = 0;

        while (min != -1) {

            min = -1;

            for (int i = 0; i < this->_tapes.size(); ++i) {

                if (i == empty)
                    continue;

                if (peaks[i].tape == -1)
                    continue;

                if (min == -1 || peaks[min].value > peaks[i].value)
                    min = i;

            }

            if (min != -1) {

                this->_tapes[empty]->write(peaks[min].value);

                if (!this->_tapes[min]->eoc(peaks[min].value))
                    this->_tapes[min]->read(peaks[min].value);
                else
                    peaks[min].tape = -1;

            }

        }

    }

    template <typename T>
    void PolyphaseMergeSort<T>::_sort() {

        std::cout << "sort" << std::endl;

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

                for (int i = 0; i < this->_tapes[min]->capacity(); ++i)
                    this->_merge(empty);

                this->_tapes[empty]->close();
                this->_tapes[empty]->open(std::ios_base::in | std::ios_base::binary);
                this->_tapes[empty]->capacity(this->_tapes[min]->capacity());

                for (int i = 0; i < this->_tapes.size(); ++i)
                    if (i != empty)
                        this->_tapes[i]->decrementCapacity(this->_tapes[empty]->capacity());

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