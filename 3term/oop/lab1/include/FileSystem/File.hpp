#pragma once

#include "FileSystemObject.hpp"

namespace ng {

    class File : public FileSystemObject {
    public:
        // constructor / destructor
        File(const std::string& name);

        // public methods

    private:
        // variables
        Time _modificationTime;
        Date _modificationDate;

        // private methods

    }; // class File

} // namespace ng