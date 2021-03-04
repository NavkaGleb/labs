#pragma once

#include <vector>
#include <memory>
#include <filesystem>

#include "TypeInfo.hpp"

namespace RefactoredProject {

    class FileManager {
    public:
        FileManager() = default;
        virtual ~FileManager() = default;

        template <typename T> T Get(uintmax_t pos) const;
        template <typename T> std::vector<std::shared_ptr<T>> Get() const;

    private:


    };

    template <typename T>
    T FileManager::Get(uintmax_t pos) const {
        using namespace TypeInfo;

        std::ifstream infile(GetBinaryDataPath(GetHash<T>()), std::fstream::in | std::fstream::binary);
        T             entity;

        infile.seekg(pos);
        entity.ReadFromBinary(infile);

        infile.close();

        return entity;
    }

    template <typename T>
    std::vector<std::shared_ptr<T>> FileManager::Get() const {
        std::vector<std::shared_ptr<T>> result;

        std::string   path = TypeInfo::GetBinaryDataPath(TypeInfo::GetHash<T>());
        uintmax_t     size = std::filesystem::file_size(path);
        std::ifstream infile(path, std::fstream::in | std::fstream::binary);

        for (T entity; true;) {
            entity.ReadFromBinary(infile);

            result.emplace_back(std::make_shared<T>(entity));

            if (infile.tellg() == size)
                break;
        }

        infile.close();

        return result;
    }

} // namespace RefactoredProject