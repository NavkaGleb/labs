#pragma once

#include <vector>
#include <memory>
#include <fstream>
#include <filesystem>
#include <memory>

#include "TypeInfo.hpp"
#include "IDataBaseEntity.hpp"

namespace RefactoredProject {

    class FileManager {
    public:
        FileManager() = default;
        virtual ~FileManager() = default;

        template <Entity T> T Get(uintmax_t pos) const;
        template <Entity T> std::vector<std::shared_ptr<T>> Get() const;

        template <Entity T> void Update(const T& entity, uintmax_t position) const;

        template <Entity T> std::vector<std::shared_ptr<T>> Search(SearchFunc<T> predicate);

    private:

    };

    template <Entity T>
    T FileManager::Get(uintmax_t pos) const {
        using TypeInfo::GetHash;
        using TypeInfo::GetBinaryDataPath;

        std::ifstream infile(GetBinaryDataPath(GetHash<T>()), std::fstream::in | std::fstream::binary);
        T             entity;

        infile.seekg(pos);
        entity.ReadFromBinary(infile);

        infile.close();

        return entity;
    }

    template <Entity T>
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

    template <Entity T>
    void FileManager::Update(const T& entity, uintmax_t position) const {
        using TypeInfo::GetHash;
        using TypeInfo::GetBinaryDataPath;

        std::ofstream outfile(GetBinaryDataPath(GetHash<T>()), std::fstream::out | std::fstream::binary);

        outfile.seekp(position);
        entity.WriteToBinary(outfile);

        outfile.close();
    }

    template <Entity T>
    std::vector<std::shared_ptr<T>> FileManager::Search(SearchFunc<T> predicate) {
        std::vector<std::shared_ptr<T>> result;

        std::string   path = TypeInfo::GetBinaryDataPath(TypeInfo::GetHash<T>());
        uintmax_t     size = std::filesystem::file_size(path);
        std::ifstream infile(path, std::fstream::in | std::fstream::binary);

        for (T entity; true;) {
            entity.ReadFromBinary(infile);

            if (predicate(entity))
                result.emplace_back(std::make_shared<T>(entity));

            if (infile.tellg() == size)
                break;
        }

        infile.close();

        return result;
    }

} // namespace RefactoredProject