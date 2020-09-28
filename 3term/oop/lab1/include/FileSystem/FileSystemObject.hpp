#pragma once

#include <string>
#include <filesystem>

#include "DateTime/Date.hpp"
#include "DateTime/Time.hpp"

namespace fs = std::filesystem;

namespace ng {

    class FileSystemObject {
    public:
        // constructor / destructor
        FileSystemObject() = default;
        virtual ~FileSystemObject() = default;

        // accessors
        [[nodiscard]] std::string path() const;
        [[nodiscard]] std::string filename() const;

        // public methods

        // operators
        friend std::ostream& operator<<(std::ostream& stream, const FileSystemObject& object);

    protected:
        // variables
        fs::path _path;
        uintmax_t _size;
        Time _creationTime;
        Date _creationDate;
        bool _directory;

    }; // class File

} // namespace ng