#pragma once

#include "FileSystemObject.hpp"

namespace ng {

    class File : public FileSystemObject {
    public:
        // constructor / destructor
        File();
        File(const fs::path& path);
        File(const fs::path& path, uintmax_t size, const Time& creationTime, const Date& creationDate);

        // public methods

    private:
        // variables
        Time _modificationTime;
        Date _modificationDate;

        // private methods

    }; // class File

} // namespace ng