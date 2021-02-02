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
        [[nodiscard]] bool eoc();
        [[nodiscard]] const int& capacity() const;

        // modifiers
        void capacity(const int& capacity);

        // public methods
        void incrementCapacity(const int& count = 1);
        void decrementCapacity(const int& count = 1);
        void nextChunk();

        void open(std::ios_base::openmode mode);
        void close();
        void read(T& data);
        void write(const T& data);
        void write(const std::vector<T>& data);
        void addChunkPosition();

    private:
        // variables
        std::fstream _file;
        std::ios_base::openmode _mode;
        std::string _filename;
        int _capacity;
        int _end;
        int _currentPosition;
        int _currentChunk;
        std::vector<int> _chunksPosition;

    }; // class Tape

    // source
    // constructor / destructor
    template <typename T>
    Tape<T>::Tape(std::string filename, std::ios_base::openmode mode)
        : _filename(std::move(filename)), _mode(mode), _capacity(0),
        _end(0), _currentPosition(0), _currentChunk(0) {

        this->_file.open(this->_filename, this->_mode);

    }

    // accessors
    template <typename T>
    const std::string& Tape<T>::filename() const { return this->_filename; }

    template <typename T>
    bool Tape<T>::empty() const { return this->_capacity == 0; }

    template <typename T>
    bool Tape<T>::eoc() {

        return this->_chunksPosition[this->_currentChunk] == this->_currentPosition;

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

    }

    template <typename T>
    void Tape<T>::nextChunk() { ++this->_currentChunk; }

    template <typename T>
    void Tape<T>::open(std::ios_base::openmode mode) {

        this->_mode = mode;
        this->_file.open(this->_filename, this->_mode);

        if (this->_mode == (std::ios_base::out | std::ios_base::binary)) {

            this->_end = 0;
            this->_currentChunk = 0;
            this->_currentPosition = 0;
            this->_chunksPosition.clear();

        }

    }

    template <typename T>
    void Tape<T>::close() { this->_file.close(); }

    template <typename T>
    void Tape<T>::read(T& data) {

        this->_file.read(reinterpret_cast<char*>(&data), sizeof(data));
        this->_currentPosition += sizeof(T);

    }

    template <typename T>
    void Tape<T>::write(const T& data) {

        this->_file.write(reinterpret_cast<const char*>(&data), sizeof(data));
        this->_end += sizeof(data);

    }

    template <typename T>
    void Tape<T>::write(const std::vector<T>& data) {

        for (const auto& e : data)
            this->_file.write(reinterpret_cast<const char*>(&e), sizeof(e));



        this->_end += static_cast<int>(sizeof(T) * data.size());
        this->_chunksPosition.emplace_back(this->_end);

    }

    template <typename T>
    void Tape<T>::addChunkPosition() {

        this->_chunksPosition.emplace_back(this->_file.tellp());

    }

} // namespace ng
