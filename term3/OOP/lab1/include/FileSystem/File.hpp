#pragma once

#include "FileSystemObject.hpp"

namespace ng {

    class File : public FileSystemObject {
    public:
        // constructor / destructor
        File();
        File(const fs::path& path);
        File(const fs::path& path, uintmax_t size, const DateTime& creation, const DateTime& modification);

        // accessors
        const uintmax_t& size() const;

        // public methods

    private:
        // variables
        uintmax_t _size;
        DateTime _modification;

        // private methods

    }; // class File

} // namespace ng