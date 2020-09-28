#pragma once

#include <filesystem>

#include "DataStructures/Trees/GeneralTree.hpp"
#include "File.hpp"
#include "Directory.hpp"

namespace fs = std::filesystem;

namespace ng {

    typedef GeneralTree<FileSystemObject*>::Node Node;

    class FileSystem {
    public:
        // constructor / destructor
        FileSystem();
        FileSystem(const std::string& root);
        virtual ~FileSystem();

        // accessors
        const Node* root() const;
        const Node* current() const;

        // public methods
        void pushFile(const std::string& name);
        void pushDirectory(const std::string& name);

        void move(const std::string& path);

        void import(const std::string& path);

        // out
        void printCurrentPath() const;
        void printCurrentFiles() const;
        void printTree(const Node* node) const;

    private:
        // variables
        GeneralTree<FileSystemObject*>* _tree;
        Node* _currentNode;
        std::vector<std::pair<int, std::string>> _currentPath;

        // static private methods
        static std::time_t _toTimeT(const fs::file_time_type& fileTime);

        // private methods
        void _directoryTraversal(const fs::path& path, Node* node);

    }; // class FileSystem

} // namespace ng