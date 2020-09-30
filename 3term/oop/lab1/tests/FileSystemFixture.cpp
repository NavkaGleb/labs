#include <gtest/gtest.h>

#include "FileSystem/FileSystem.hpp"

class FileSystemFixture : public ::testing::Test {
protected:
    // variables
    ng::FileSystem* _fs;

    // protected methods
    void TearDown() override { delete this->_fs; }

    void CreateFileSystem() { this->_fs = new ng::FileSystem; }

};

TEST_F(FileSystemFixture, Init) {

    std::vector<std::string> expectedSearch = { "first_dir", "3dir", "first_file.txt", "second_dir" };
    std::function<bool(const ng::FileSystemObject&)> searchFunc = [](const ng::FileSystemObject& obj) {

        return obj.filename().find(".txt") != std::string::npos || obj.filename().ends_with('r');

    };

    this->CreateFileSystem();
    this->_fs->pushDirectory("root");
    this->_fs->pushFile("first_file.txt");
    this->_fs->pushDirectory("first_dir");
    this->_fs->pushDirectory("second_dir");
    this->_fs->move("first_dir");
    this->_fs->pushFile("second.file");
    this->_fs->pushDirectory("3dir");
    this->_fs->move("3dir");
    this->_fs->pushFile("end.bin");

    std::vector<ng::FileSystemObject> actualSearch = this->_fs->search(".", searchFunc);

    for (std::size_t i = 0; i < expectedSearch.size(); ++i)
        EXPECT_EQ(expectedSearch[i], actualSearch[i].filename());

}

TEST_F(FileSystemFixture, Import) {

    std::vector<std::string> expectedSearch = {
        "ExpressionTree.hpp",
        "GeneralMapTree.hpp",
        "GeneralTree.hpp",
        "Date.hpp",
        "DateTime.hpp",
        "Time.hpp",
        "Directory.hpp",
        "File.hpp",
        "FileSystem.hpp",
        "FileSystemObject.hpp",
        "Random.hpp",
        "ExpressionTree.cpp",
        "Directory.cpp",
        "File.cpp",
        "FileSystem.cpp",
        "FileSystemObject.cpp",
        "ExpressionTreeFixture.cpp",
        "FileSystemFixture.cpp",
        "TestFunc.hpp"
    };
    std::function<bool(const ng::FileSystemObject&)> searchFunc = [](const ng::FileSystemObject& obj) {

        return (obj.filename().find(".hpp") != std::string::npos || obj.filename().ends_with(".cpp")) &&
                obj.creation() >= ng::DateTime(
                    ng::Time(ng::Hour(10), ng::Minute(10), ng::Second(0)),
                    ng::Date(ng::Day(27), ng::Month(9), ng::Year(2020))
                ) &&
                obj.filename() != "CMakeCXXCompilerId.cpp";

    };

    this->CreateFileSystem();
    this->_fs->import(".");

//    this->_fs->printTree(this->_fs->root());

    std::vector<ng::FileSystemObject> actualSearch = this->_fs->search(".", searchFunc);

    for (std::size_t i = 0; i < expectedSearch.size(); ++i)
        EXPECT_EQ(expectedSearch[i], actualSearch[i].filename());

}