#pragma once

#include <fstream>
#include <vector>

namespace ng {

    class Tape {
    public:
        // constructor / destructor
        Tape() = default;
        Tape(std::string filename, std::ios_base::openmode mode, int chunkSize);
        virtual ~Tape();

        // accessors
        [[nodiscard]] const std::string& filename() const;
        [[nodiscard]] bool eof() const;
        [[nodiscard]] bool eoc();
        [[nodiscard]] bool empty() const;
        [[nodiscard]] const int& capacity() const;
        [[nodiscard]] const int& chunks() const;
        [[nodiscard]] const int& dummy() const;
        [[nodiscard]] bool full() const;
        int pos();
        std::fstream::pos_type end();

        // modifiers
        void capacity(const int& capacity);
        void chunks(const int& chunks);
        void chunkSize(const int& size);

        // public methods
        void incrementChunks(const int& count = 1);
        void decrementChunks(const int& count = 1);
        void decrementCapacity(const int& count = 1);
        void decrementDummy();

        void open(std::ios_base::openmode mode);
        void close();
        void read(int& data);
        void write(int& data);
        void write(std::vector<int>& data);

        void initDummy();
        void updateEnd();
    private:
        // variables
        std::fstream _file;
        std::ios_base::openmode _mode;
        std::string _filename;

        int _capacity;
        int _chunks;
        int _dummy;
        int _chunkSize;
        int _end;

        // private methods

    }; // class Tape

} // namespace ng
