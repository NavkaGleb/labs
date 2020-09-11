#include <Tape.hpp>

#include <iostream>

namespace ng {

    // constructor / destructor
    Tape::Tape(std::string filename, std::ios_base::openmode mode, int chunkSize)
        : _filename(std::move(filename)), _mode(mode), _capacity(0), _chunks(0), _dummy(0), _chunkSize(chunkSize * sizeof(int)) {

        this->_file.open(this->_filename, this->_mode);
        this->_file.seekp(0, std::ios_base::end);
        this->_end = this->_file.tellp();
        this->_file.seekp(0, std::ios_base::beg);

//        if (this->_chunks != 0)
//            std::cout << "CHUNK SIZE IN FILE = " << this->_end / this->_chunks << std::endl;

    }

    Tape::~Tape() {

        this->_file.close();

    }

    // accessors
    const std::string& Tape::filename() const { return this->_filename; }

    bool Tape::eof() const { return this->_file.eof(); }

    bool Tape::eoc() {

        std::cout << "\t-- eoc in " << this->_filename << std::endl;
        std::cout << "\tpos = " << this->_file.tellg() << ", chunkSize = " << this->_chunkSize << std::endl;
        return this->_file.tellg() == -1 || this->_file.tellg() == this->_end || this->_file.tellg() % (this->_chunkSize) == 0;

    }

    bool Tape::empty() const { return this->_chunks == 0; }

    const int& Tape::capacity() const { return this->_capacity; }

    const int& Tape::chunks() const { return this->_chunks; }

    const int& Tape::dummy() const { return this->_dummy; }

    bool Tape::full() const { return this->_chunks == this->_capacity; }

    int Tape::pos() { return this->_file.tellg(); }

    std::fstream::pos_type Tape::end() {

        this->_file.seekp(0, std::ios_base::end);
        auto end = this->_file.tellp();
        this->_file.seekp(0, std::ios_base::beg);

        return end;

    }

    // modifiers
    void Tape::capacity(const int& capacity) { this->_capacity = capacity; }

    void Tape::chunks(const int& chunks) { this->_chunks = chunks; }

    void Tape::chunkSize(const int& size) { this->_chunkSize = size * static_cast<int>(sizeof(int)); }

    // public methods
    void Tape::incrementChunks(const int& count) { this->_chunks += count; }

    void Tape::decrementChunks(const int& count) { this->_chunks -= count; }

    void Tape::decrementCapacity(const int& count) {

        if (this->_capacity == this->_chunks)
            this->_chunks -= count;

        this->_capacity -= count;

//        this->decrementDummy();

    }

    void Tape::decrementDummy() {

        if (this->_dummy > 0)
            --this->_dummy;
        else
            --this->_chunks;

    }

    void Tape::open(std::ios_base::openmode mode) {

        this->_mode = mode;
        this->_file.open(this->_filename, this->_mode);

        this->updateEnd();

    }

    void Tape::close() { this->_file.close(); }

    void Tape::read(int& data) {

        std::cout << "read from file" << this->_filename << std::endl;
        std::cout << "-- pos before read = " << this->_file.tellg() << ", end = " << this->_end << std::endl;

        this->_file.read(reinterpret_cast<char*>(&data), sizeof(data));

        std::cout << "-- pos after read = " << this->_file.tellg() << ", end = " << this->_end << std::endl;

    }

    void Tape::write(int& data) {

        this->_file.write(reinterpret_cast<char*>(&data), sizeof(data));

    }

    void Tape::write(std::vector<int>& data) {

        for (auto& e : data)
            this->_file.write(reinterpret_cast<char*>(&e), sizeof(e));

    }

    void Tape::initDummy() {

        this->_dummy = this->_capacity - this->_chunks;

    }

    void Tape::updateEnd() {

        this->_file.seekp(0, std::ios_base::end);
        this->_end = this->_file.tellp();
        this->_file.seekp(0, std::ios_base::beg);

        if (this->_chunks != 0) {

//            this->_chunkSize = this->_end / this->_chunks;
            std::cout << "-- update end in tape " << this->_filename << std::endl;
            std::cout << "chunkCapacity = " << this->_chunkSize << ", end = " << this->_end << ", chunks = " << this->_chunks << std::endl;

        }

    }

    // private methods

}