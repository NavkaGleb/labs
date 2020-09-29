#pragma once

#include <string>
#include <filesystem>

#include "DateTime/DateTime.hpp"

namespace fs = std::filesystem;

namespace ng {

    class FileSystemObject {
    public:
        // enums
        enum Type { Unknown = -1, File = 0, Directory };

        // constructor / destructor
        FileSystemObject() = default;
        virtual ~FileSystemObject() = default;

        // accessors
        [[nodiscard]] fs::path path() const;
        [[nodiscard]] std::string filename() const;
        [[nodiscard]] const short& type() const;

        // public methods

        // operators
        friend std::ostream& operator<<(std::ostream& stream, const FileSystemObject& object);
        friend std::ostream& operator<<(std::ostream& stream, const FileSystemObject* object);
        friend bool operator<(const FileSystemObject& object1, const FileSystemObject& object2);

    protected:
        // variables
        fs::path _path;
        DateTime _creation;
        short _type;

    }; // class File

} // namespace ng