#pragma once

#include <vector>
#include <memory>
#include <fstream>
#include <filesystem>
#include <memory>
#include <iostream>

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
        std::string   path = TypeInfo::GetBinaryDataPath(TypeInfo::GetHash<T>());
        std::ifstream infile(path, std::fstream::in | std::fstream::binary);

        if (!infile.is_open())
            return {};

        std::vector<std::shared_ptr<T>> result;
        uintmax_t                       size = std::filesystem::file_size(path);

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
        std::string       path = TypeInfo::GetBinaryDataPath(TypeInfo::GetHash<T>());
        uintmax_t         size = std::filesystem::file_size(path);
        std::ifstream     infile(path, std::fstream::in | std::fstream::out);
        std::vector<char> before;
        std::vector<char> after;

        before.resize(position);
        infile.read(before.data(), before.size());

        infile.seekg(static_cast<intmax_t>(infile.tellg()) + T::GetBytesCount());

        after.resize(size - infile.tellg());
        infile.read(after.data(), after.size());

        infile.close();

        std::ofstream outfile(path, std::fstream::out | std::fstream::binary);

        outfile.write(before.data(), before.size());
        entity.WriteToBinary(outfile);
        outfile.write(after.data(), after.size());

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