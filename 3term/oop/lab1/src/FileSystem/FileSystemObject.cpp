#include "FileSystem/FileSystemObject.hpp"

namespace ng {

    // accessors
    const std::string& FileSystemObject::name() const { return this->_name; }

    // operators
    std::ostream& operator<<(std::ostream& stream, const FileSystemObject& object) {

        return stream << object._name;

    }

}