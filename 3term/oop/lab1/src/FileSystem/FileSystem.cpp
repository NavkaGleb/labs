#include "FileSystem/FileSystem.hpp"

//#include <filesystem>

namespace ng {

    // constructor / destructor
    FileSystem::FileSystem() : _tree(new GeneralTree<FileSystemObject*>) {}

    FileSystem::~FileSystem() { delete this->_tree; }

    // public methods
    void FileSystem::pushFile(const std::string& name) {

        std::vector<int> path;

        for (const auto& [index, filename] : this->_currentPath)
            path.emplace_back(index);

        this->_tree->push(new File(name), path);

    }

    void FileSystem

    void FileSystem::move(const std::string& directory) {

        for (int i = 0; i < this->_currentNode->children().size(); ++i) {

            if (this->_currentNode->children().at(i)->value()->name() == directory) {

                this->_currentPath.emplace_back(i, directory);
                break;

            }

        }

    }

    void FileSystem::print() {

        this->_tree->tprint();

    }

    // private methods

} // namespace ng