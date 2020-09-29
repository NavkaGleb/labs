#include "FileSystem/FileSystemObject.hpp"

namespace ng {

    // accessors
    fs::path FileSystemObject::path() const { return this->_path; }

    std::string FileSystemObject::filename() const { return this->_path.filename().string(); }

    const bool& FileSystemObject::directory() const { return this->_directory; }

    // operators
    std::ostream& operator<<(std::ostream& stream, const FileSystemObject& object) {

        stream << object._path.filename().string();

//        if (!object._directory)
//            stream << " (" << object._size << ")";

        return stream;

    }

    std::ostream& operator<<(std::ostream& stream, const FileSystemObject* object) {

//        stream << object->_path.filename() << " ";

        stream << object->_creation;

//        if (!object->_directory)
//            stream << " (" << object->_size << ")";

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