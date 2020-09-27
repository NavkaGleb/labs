#pragma once

#include <string>

#include "DateTime/Date.hpp"
#include "DateTime/Time.hpp"

namespace ng {

    class FileSystemObject {
    public:
        // constructor / destructor
        FileSystemObject() = default;
        virtual ~FileSystemObject() = default;

        // accessors
        [[nodiscard]] const std::string& name() const;

        // public methods

        // operators
        friend std::ostream& operator<<(std::ostream& stream, const FileSystemObject& object);

    protected:
        // variables
        std::string _name;
        long long _size;
        Time _creationTime;
        Date _creationDate;
        bool _directory;

    }; // class File

} // namespace ng