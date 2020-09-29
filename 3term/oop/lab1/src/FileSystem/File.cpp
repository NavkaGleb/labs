#include "FileSystem/File.hpp"

#include <ctime>

namespace ng {

    // constructor / destructor
    File::File() {

        this->_creation = DateTime();
        this->_directory = false;
        this->_size = 0;
        this->_modification = DateTime();

    }

    File::File(const fs::path& path) {

        std::time_t t = std::time(nullptr);

        this->_path = path;
        this->_creation = DateTime(&t);
        this->_directory = false;
        this->_size = 0;
        this->_modification = this->_creation;

    }

    File::File(const fs::path& path, uintmax_t size, const DateTime& creation, const DateTime& modification) {

        this->_path = path;
        this->_size = size;
        this->_creation = creation;
        this->_directory = false;
        this->_size = 0;
        this->_modification = modification;

    }

    // accessors
    const uintmax_t& File::size() const { return this->_size; }

} // namespace ng
