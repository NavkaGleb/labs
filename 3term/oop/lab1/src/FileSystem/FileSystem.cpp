#include "FileSystem/FileSystem.hpp"

#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;
namespace ch = std::chrono;

namespace ng {

    // constructor / destructor
    FileSystem::FileSystem() : _tree(new GeneralTree<FileSystemObject*>), _currentNode(nullptr) {}

    FileSystem::FileSystem(const std::string& root) {

        this->_tree = new GeneralTree<FileSystemObject*>(new Directory(root));
        this->_currentNode = this->_tree->root();

    }

    FileSystem::~FileSystem() { delete this->_tree; }

    // public methods
    void FileSystem::pushFile(const std::string& name) {

        std::vector<int> path;

        for (const auto& [index, filename] : this->_currentPath)
            path.emplace_back(index);

        this->_tree->push(new File(name), path);

    }

    void FileSystem::pushDirectory(const std::string &name) {

        std::vector<int> path;

        for (const auto& [index, filename] : this->_currentPath)
            path.emplace_back(index);

        this->_tree->push(new Directory(name), path);

    }

    void FileSystem::move(const std::string& path) {

        if (path == "..") {

            this->_currentPath.pop_back();
            return;

        }

        for (int i = 0; i < this->_currentNode->children().size(); ++i) {

            if (this->_currentNode->children().at(i)->value()->name() == path) {

                this->_currentPath.emplace_back(i, path);
                break;

            }

        }

    }

    void FileSystem::import(const std::string& root) {

        const fs::path workdir = fs::current_path().parent_path() / "src" / "FileSystem";
        const fs::path a = workdir.filename();

        std::cout << "workdir = " << workdir << std::endl;

        if (!this->_tree->empty())
            delete this->_tree;

        this->_tree = new GeneralTree<FileSystemObject*>(new Directory(workdir.string()));

        for (const auto& elem : std::filesystem::directory_iterator(workdir)) {

            if (elem.is_directory()) {

                Node* node = this->_tree->push(new Directory())

            }

            if (fs::is_directory(e.path()))
                std::cout << "directory - " << e.path().filename() << " -> " << std::endl;
            else {

                std::cout << "file - " << e.path().filename() << ", size = " << fs::file_size(e.path()) << std::endl;

                std::time_t tt = _toTimeT(fs::last_write_time(e.path()));
                std::tm* now = std::localtime(&tt);

                std::cout << Time(now) << " " << Date(now) << std::endl;

            }

            std::cout << std::endl;

        }

    }

    void FileSystem::print() {

        this->_tree->tprint();

    }

    // private static methods
    std::time_t FileSystem::_toTimeT(const fs::file_time_type& fileTime) {

        auto sctp = time_point_cast<ch::system_clock::duration>(
            fileTime - fs::file_time_type::clock::now() + ch::system_clock::now()
        );

        return ch::system_clock::to_time_t(sctp);

    }

    // private methods
    void FileSystem::_directoryTraversal(const fs::path& path) {

        for (const auto& child : fs::directory_iterator(path))
            int a;

    }

} // namespace ng