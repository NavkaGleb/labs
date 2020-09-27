#include "FileSystem/File.hpp"

#include <ctime>

namespace ng {

    // constructor / destructor
    File::File(const std::string& name) {

        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);

        this->_name = name;
        this->_size = 0;
        this->_creationTime = Time(now->tm_sec, now->tm_min, now->tm_hour);
        this->_creationDate = Date(Day(now->tm_mday), Month(now->tm_mon + 1), Year(now->tm_year + 1900));
        this->_directory = false;
        this->_modificationTime = this->_creationTime;
        this->_modificationDate = this->_creationDate;

    }

} // namespace ng
