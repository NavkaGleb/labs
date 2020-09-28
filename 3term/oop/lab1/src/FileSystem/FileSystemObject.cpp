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

    std::ostream& operator<<(std::ostream& stream, const FileSystemObject* object) {

//        stream << object->_path.filename().string();

        stream << object->_creationTime << " " << object->_creationDate;

        if (!object->_directory)
            stream << " (" << object->_size << ")";

        return stream;

    }

    bool operator<(const FileSystemObject& object1, const FileSystemObject& object2) {

        if (object1._directory && !object2._directory)
            return false;

        if (!object1._directory && object2._directory)
            return true;

        return object1.filename() < object2.filename();

    }

}