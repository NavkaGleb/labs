#pragma once

#include <fstream>
#include <vector>

namespace ng {

    class Tape {
    public:
        // constructor / destructor
        Tape() = default;
        Tape(const std::string& filename, std::ios_base::openmode mode);
        virtual ~Tape();

        // accessors
        [[nodiscard]] bool eof() const;
        [[nodiscard]] const int& capacity() const;
        [[nodiscard]] const int& size() const;
        [[nodiscard]] bool full() const;

        // modifiers
        void capacity(const int& capacity);

        // public methods
        void incrementSize();

        void open(const std::string& filename, std::ios_base::openmode mode);
        void write(const std::vector<int>& data);
        void write(const int* data, const int& size);
        void write(const std::string& str);
        void close();

    private:
        // variables
        std::fstream _file;
        int _capacity;
        int _size;

        // private methods

    }; // class Tape

} // namespace ng
