#include "FileSystem/File.hpp"

#include <ctime>

namespace ng {

    // constructor / destructor
    File::File() {

        this->_size = 0;
        this->_creationTime = Time();
        this->_creationDate = Date();
        this->_directory = false;
        this->_modificationTime = Time();
        this->_modificationDate = Date();

    }

    File::File(const fs::path& path) {

        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);

        this->_path = path;
        this->_size = 0;
        this->_creationTime = Time(Hour(now->tm_hour), Minute(now->tm_min), Second(now->tm_sec));
        this->_creationDate = Date(Day(now->tm_mday), Month(now->tm_mon + 1), Year(now->tm_year + 1900));
        this->_directory = false;
        this->_modificationTime = this->_creationTime;
        this->_modificationDate = this->_creationDate;

    }

    File::File(const fs::path& path, uintmax_t size, const Time& creationTime, const Date& creationDate) {

        this->_path = path;
        this->_size = size;
        this->_creationTime = creationTime;
        this->_creationDate = creationDate;
        this->_directory = false;
        this->_modificationTime = creationTime;
        this->_modificationDate = creationDate;

    }

} // namespace ng
