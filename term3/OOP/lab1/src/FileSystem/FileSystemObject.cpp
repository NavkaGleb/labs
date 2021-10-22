#include "FileSystem/FileSystemObject.hpp"

namespace ng {

    // accessors
    fs::path FileSystemObject::path() const { return this->_path; }

    std::string FileSystemObject::filename() const { return this->_path.filename().string(); }

    const DateTime& FileSystemObject::creation() const { return this->_creation; }

    const short& FileSystemObject::type() const { return this->_type; }

    // operators
    std::ostream& operator<<(std::ostream& stream, const FileSystemObject& object) {

        stream << object._path.filename().string();

        return stream;

    }

    std::ostream& operator<<(std::ostream& stream, const FileSystemObject* object) {

        stream << object->_creation;

        return stream;

    }

}