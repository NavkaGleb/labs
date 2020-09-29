#pragma once

#include <filesystem>

#include "DataStructures/Trees/GeneralMapTree.hpp"
#include "File.hpp"
#include "Directory.hpp"

namespace fs = std::filesystem;

namespace ng {

    typedef GeneralMapTree<std::string, FileSystemObject*>::Node Node;

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
        void pop(const std::string& name);
        void move(const std::string& path);

        void search();
        void import(const std::string& path);

        void printCurrentPath() const;
        void printCurrentFiles() const;
        void printTree(const Node* node) const;

    private:
        // variables
        GeneralMapTree<std::string, FileSystemObject*>* _tree;
        Node* _currentNode;
        std::vector<std::string> _currentPath;

        // static private methods
        static std::time_t _toTimeT(const fs::file_time_type& fileTime);

        // private methods
        void _directoryTraversal(const fs::path& path, Node* node);

    }; // class FileSystem

} // namespace ng