#pragma once

#include <fstream>
#include <vector>

namespace ng {

    // header
    template <typename T>
    class Tape {
    public:
        // constructor / destructor
        Tape(std::string filename, std::ios_base::openmode mode);
        virtual ~Tape() = default;

        // accessors
        [[nodiscard]] const std::string& filename() const;
        [[nodiscard]] bool empty() const;
        [[nodiscard]] bool eof();
        [[nodiscard]] bool eoc(const T& current);
        [[nodiscard]] const int& capacity() const;

        // modifiers
        void capacity(const int& capacity);

        // public methods
        void incrementCapacity(const int& count = 1);
        void decrementCapacity(const int& count = 1);

        void open(std::ios_base::openmode mode);
        void close();
        void read(T& data);
        void write(const T& data);
        void write(const std::vector<T>& data);

    private:
        // variables
        std::fstream _file;
        std::ios_base::openmode _mode;
        std::string _filename;
        int _capacity;
        int _chunks;
        int _end;

    }; // class Tape

    // source
    // constructor / destructor
    template <typename T>
    Tape<T>::Tape(std::string filename, std::ios_base::openmode mode)
        : _filename(std::move(filename)), _mode(mode), _capacity(0), _chunks(0), _end(0) {

        this->_file.open(this->_filename, this->_mode);

    }

    // accessors
    template <typename T>
    const std::string& Tape<T>::filename() const { return this->_filename; }

    template <typename T>
    bool Tape<T>::empty() const { return this->_capacity == 0; }

    template <typename T>
    bool Tape<T>::eof() { return this->_file.tellg() == this->_end || this->_file.tellg() == -1; }

    template <typename T>
    bool Tape<T>::eoc(const T& current) {

        std::fstream::pos_type cur = this->_file.tellg();
        T next;
        this->_file.read(reinterpret_cast<char*>(&next), sizeof(next));

        if (this->_file.tellg() == -1)
            return true;

        this->_file.seekg(cur);
        return this->_file.tellg() == this->_end || current > next;

    }

    template <typename T>
    const int& Tape<T>::capacity() const { return this->_capacity; }

    // modifiers
    template <typename T>
    void Tape<T>::capacity(const int& capacity) { this->_capacity = capacity; }

    // public methods
    template <typename T>
    void Tape<T>::incrementCapacity(const int& count) { this->_capacity += count; }

    template <typename T>
    void Tape<T>::decrementCapacity(const int& count) {

        this->_capacity -= count;

        if (this->_capacity < this->_chunks)
            this->_chunks = this->_capacity;

    }

    template <typename T>
    void Tape<T>::open(std::ios_base::openmode mode) {

        this->_mode = mode;
        this->_file.open(this->_filename, this->_mode);

        if (this->_mode == (std::ios_base::out | std::ios_base::binary))
            this->_end = 0;

    }

    template <typename T>
    void Tape<T>::close() { this->_file.close(); }

    template <typename T>
    void Tape<T>::read(T& data) { this->_file.read(reinterpret_cast<char*>(&data), sizeof(data)); }

    template <typename T>
    void Tape<T>::write(const T& data) {

        this->_file.write(reinterpret_cast<const char*>(&data), sizeof(data));
        this->_end += sizeof(data);

    }

    template <typename T>
    void Tape<T>::write(const std::vector<T>& data) {

        for (const auto& e : data)
            this->_file.write(reinterpret_cast<const char*>(&e), sizeof(e));

        this->_end += sizeof(T) * data.size();

    }

} // namespace ng
