#pragma once

#include "FileSystemObject.hpp"

namespace ng {

    class Directory : public FileSystemObject {
    public:
        // constructor / destructor
        Directory();
        Directory(const std::string& name);
        Directory(const std::string& name, const Time& creationTime, const Date& creationDate)

        // public methods

    }; // class File

} // namespace ng