#include <Tape.hpp>

namespace ng {

    // constructor / destructor
    Tape::Tape(const std::string& filename, std::ios_base::openmode mode) : _capacity(0), _size(0) {

        this->open(filename, mode);

    }

    Tape::~Tape() {

        this->close();

    }

    // accessors
    bool Tape::eof() const { return this->_file.eof(); }

    const int& Tape::capacity() const { return this->_capacity; }

    const int& Tape::size() const { return this->_size; }

    bool Tape::full() const { return this->_size == this->_capacity; }

    // modifiers
    void Tape::capacity(const int& capacity) { this->_capacity = capacity; }

    // public methods
    void Tape::incrementSize() { ++this->_size; }

    void Tape::open(const std::string& filename, std::ios_base::openmode mode) {

        this->_file.open(filename, mode);

    }

    void Tape::write(const std::vector<int>& data) {

        for (const auto& e : data)
            this->_file << e << " ";

    }

    void Tape::write(const int* data, const int& size) {

        for (int i = 0; i < size; ++i)
            this->_file << data[i] << " ";

    }

    void Tape::write(const std::string& str) {

        this->_file << str;

    }

    void Tape::close() { this->_file.close(); }

    // private methods

}