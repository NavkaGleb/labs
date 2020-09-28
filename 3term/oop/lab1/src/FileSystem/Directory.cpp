#include "FileSystem/Directory.hpp"

#include <ctime>

namespace ng {

    // constructor / destructor
    Directory::Directory() {

        this->_size = 0;
        this->_creationTime = { 0, 0, 0 };
        this->_creationDate = { Day(0), Month(0), Year(0) };
        this->_directory = true;

    }

    Directory::Directory(const std::string& name) {

        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);

        this->_name = name;
        this->_size = 0;
        this->_creationTime = Time(now->tm_sec, now->tm_min, now->tm_hour);
        this->_creationDate = Date(Day(now->tm_mday), Month(now->tm_mon + 1), Year(now->tm_year + 1900));
        this->_directory = true;

    }

} // namespace ng