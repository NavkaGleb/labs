#include "FileSystem/FileSystemObject.hpp"

namespace ng {

    // accessors
    std::string FileSystemObject::path() const { return this->_path.string(); }

    std::string FileSystemObject::filename() const { return this->_path.filename().string(); }

    // operators
    std::ostream& operator<<(std::ostream& stream, const FileSystemObject& object) {

        stream << object._path.filename().string();

        if (!object._directory)
            stream << " (" << object._size << ")";

        return stream;

    }

}