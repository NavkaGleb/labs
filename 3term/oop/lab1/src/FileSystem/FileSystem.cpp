#include "FileSystem/FileSystem.hpp"

#include <chrono>

namespace fs = std::filesystem;
namespace ch = std::chrono;

namespace ng {

    // constructor / destructor
    FileSystem::FileSystem() : _tree(new GeneralMapTree<std::string, FileSystemObject*>), _currentNode(nullptr) {}

    FileSystem::FileSystem(const std::string& root) {

        this->_tree = new GeneralMapTree<std::string, FileSystemObject*>(new Directory(root));
        this->_currentNode = this->_tree->root();

    }

    FileSystem::~FileSystem() { delete this->_tree; }

    // accessors
    const Node* FileSystem::root() const { return this->_tree->root(); }

    const Node* FileSystem::current() const { return this->_currentNode; }

    // public methods
    void FileSystem::pushFile(const std::string& name) {

        this->_tree->push(name, new File(name), this->_currentNode);

    }

    void FileSystem::pushDirectory(const std::string &name) {

        this->_tree->push(name, new Directory(name), this->_currentNode);

    }

    void FileSystem::pop(const std::string& name) {

        this->_tree->pop(name, this->_currentNode);

    }

    void FileSystem::move(const std::string& path) {

        if (path == "..") {

            this->_currentPath.pop_back();
            this->_currentNode = this->_currentNode->parent();
            return;

        }

        this->_currentPath.emplace_back(path);
        this->_currentNode = this->_currentNode->children().at(path);

    }

    void FileSystem::import(const std::string& path) {

        const fs::path workdir = fs::current_path().parent_path();

        if (!this->_tree->empty())
            delete this->_tree;

        std::time_t time = _toTimeT(fs::last_write_time(workdir));
        std::tm* fileTime = std::localtime(&time);

        this->_tree = new GeneralMapTree<std::string, FileSystemObject*>(new Directory(workdir.string(), Time(fileTime), Date(fileTime)));
        Node* root = this->_tree->root();

        this->_directoryTraversal(workdir, root);

    }

    void FileSystem::printCurrentPath() const {

        for (std::size_t i = 0; i < this->_currentPath.size() - 1; ++i)
            std::cout << this->_currentPath[i] << "/";

        std::cout << this->_currentPath.back() << "~" << std::endl;

    }

    void FileSystem::printCurrentFiles() const {

        for (const auto& [key, child] : this->_currentNode->children())
            std::cout << std::setw(10) << key;

        std::cout << std::endl;

    }

    void FileSystem::printTree(const Node* node) const {

        this->_tree->tprint(node);

    }

    // private static methods
    std::time_t FileSystem::_toTimeT(const fs::file_time_type& fileTime) {

        auto sctp = time_point_cast<ch::system_clock::duration>(
            fileTime - fs::file_time_type::clock::now() + ch::system_clock::now()
        );

        return ch::system_clock::to_time_t(sctp);

    }

    // private methods
    void FileSystem::_directoryTraversal(const fs::path& path, Node* parent) {

        for (const auto& elem : fs::directory_iterator(path)) {

            std::time_t time = _toTimeT(fs::last_write_time(elem.path()));
            std::tm* fileTime = std::localtime(&time);

            if (elem.is_directory()) {

                std::cout << "dir = " << elem.path().filename() << " -> " << Time(fileTime) << " " << Date(fileTime) << std::endl;

                Node* node = this->_tree->push(
                    elem.path().filename().string(),
                    new Directory(elem.path(), Time(fileTime), Date(fileTime)),
                    parent
                );
                this->_directoryTraversal(elem.path(), node);

            } else {

                std::cout << "file = " << elem.path().filename() << " -> " << Time(fileTime) << " " << Date(fileTime) << std::endl;

                this->_tree->push(
                    elem.path().filename().string(),
                    new File(elem.path(), Time(fileTime), Date(fileTime)),
                    parent
                );

            }

            std::cout << std::endl;

        }

    }

} // namespace ng