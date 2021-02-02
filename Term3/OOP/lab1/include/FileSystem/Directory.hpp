#pragma once

#include "FileSystemObject.hpp"

namespace ng {

    class Directory : public FileSystemObject {
    public:
        // constructor / destructor
        Directory();
        Directory(const fs::path& path);
        Directory(const fs::path& path, const DateTime& creation);

        // public methods

    }; // class File

} // namespace ng