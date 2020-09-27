#pragma once

//#include <filesystem>

#include "DataStructures/Trees/GeneralTree.hpp"
#include "File.hpp"
#include "Directory.hpp"

namespace ng {

    class FileSystem {
    public:
        // constructor / destructor
        FileSystem();
        virtual ~FileSystem();

        // public methods
        void pushFile(const std::string& name);
        void pushDirectory(const std::string& name);

        void move(const std::string& directory);

        void print();

    private:
        // variables
        GeneralTree<FileSystemObject*>* _tree;
        GeneralTree<FileSystemObject*>::Node* _currentNode;
        std::vector<std::pair<int, std::string>> _currentPath;

        // private methods

    }; // class FileSystem

} // namespace ng