#include "FileSystem/Directory.hpp"

#include <ctime>

namespace ng {

    // constructor / destructor
    Directory::Directory() {

        this->_creation = {};
        this->_directory = true;

    }

    Directory::Directory(const fs::path& path) {

        std::time_t t = std::time(nullptr);

        this->_path = path;
        this->_creation = DateTime(&t);
        this->_directory = true;

    }

    Directory::Directory(const fs::path& path, const DateTime& creation) {

        this->_path = path;
        this->_creation = creation;
        this->_directory = true;

    }

} // namespace ng