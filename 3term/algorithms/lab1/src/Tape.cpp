#include <Tape.hpp>

namespace ng {

    // constructor / destructor
    Tape::Tape(std::string filename, std::ios_base::openmode mode)
        : _filename(std::move(filename)), _capacity(0), _size(0), _position(0) {

        this->_file.open(this->_filename, mode);

    }

    Tape::~Tape() {

        this->close();

    }

    // accessors
    const std::string& Tape::filename() const { return this->_filename; }

    bool Tape::eof() const { return this->_file.eof(); }

    const int& Tape::capacity() const { return this->_capacity; }

    const int& Tape::size() const { return this->_size; }

    bool Tape::full() const { return this->_size == this->_capacity; }

    int Tape::dummy() const { return this->_capacity - this->_size; }

    // modifiers
    void Tape::capacity(const int& capacity) { this->_capacity = capacity; }

    // public methods
    void Tape::incrementSize() { ++this->_size; }

    void Tape::open(const std::string& filename, std::ios_base::openmode mode) {

        this->_filename = filename;
        this->_file.open(this->_filename, mode);

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

    void Tape::write(const int& data) {

        this->_file << data;
        ++this->_position;

    }

    void Tape::read(int& data) {

        this->_file >> data;

    }

    int Tape::read() {

        int data;
        this->_file >> data;

        return data;

    }

    void Tape::close() { this->_file.close(); }

    // private methods

}