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
        [[nodiscard]] fs::path path() const;
        [[nodiscard]] std::string filename() const;
        [[nodiscard]] const uintmax_t& size() const;
        [[nodiscard]] const bool& directory() const;

        // public methods

        // operators
        friend std::ostream& operator<<(std::ostream& stream, const FileSystemObject& object);
        friend std::ostream& operator<<(std::ostream& stream, const FileSystemObject* object);
        friend bool operator<(const FileSystemObject& object1, const FileSystemObject& object2);

    protected:
        // variables
        fs::path _path;
        uintmax_t _size;
        Time _creationTime;
        Date _creationDate;
        bool _directory;

    }; // class File

} // namespace ng