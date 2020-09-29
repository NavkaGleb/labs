#include "FileSystem/Directory.hpp"

#include <ctime>

namespace ng {

    // constructor / destructor
    Directory::Directory() {

        this->_creation = {};
        this->_type = FileSystemObject::Type::Directory;

    }

    Directory::Directory(const fs::path& path) {

        std::time_t t = std::time(nullptr);

        this->_path = path;
        this->_creation = DateTime(&t);
        this->_type = FileSystemObject::Type::Directory;

    }

    Directory::Directory(const fs::path& path, const DateTime& creation) {

        this->_path = path;
        this->_creation = creation;
        this->_type = FileSystemObject::Type::Directory;

    }

} // namespace ng