#pragma once

#include <vector>
#include <array>
#include <memory>
#include <fstream>
#include <filesystem>
#include <memory>
#include <iostream>
#include <unordered_set>

#include "IndexTable.hpp"
#include "TypeInfo.hpp"
#include "IDataBaseEntity.hpp"

namespace RefactoredProject {

    class FileManager {
    public:
        explicit FileManager(IndexTable& indexTable);
        virtual ~FileManager() = default;

        template <Entity T> std::vector<std::shared_ptr<T>> Get() const;
        template <Entity T> T Get(uintmax_t pos) const;

        void Delete(const TypeInfo& typeInfo);
        void Delete(const TypeInfo& typeInfo, int id);
        void Truncate(const TypeInfo& typeInfo);

        template <Entity T> void Update(const T& entity, uintmax_t position) const;

        template <Entity T> std::vector<std::shared_ptr<T>> Search(SearchFunc<T> predicate) const;

    private:
        IndexTable& m_IndexTable;

    }; // class FileManager

    template <Entity T>
    std::vector<std::shared_ptr<T>> FileManager::Get() const {
        std::ifstream infile(GetBinaryDataPath<T>(), std::fstream::in | std::fstream::binary);

        if (!infile.is_open())
            return {};

        std::vector<std::shared_ptr<T>> result;

        for (T entity; const auto& entityData : m_IndexTable.GetData(TypeInfo::Get<T>())) {
            if (entityData.IsDeleted) {
                infile.seekg(entityData.Position + T::GetBytesCount());
                continue;
            }

            entity.ReadFromBinary(infile);
            result.emplace_back(std::make_shared<T>(entity));
        }

        infile.close();

        return result;
    }

    template <Entity T>
    T FileManager::Get(uintmax_t pos) const {
        std::ifstream infile(GetBinaryDataPath<T>(), std::fstream::in | std::fstream::binary);
        T             entity;

        infile.seekg(pos);
        entity.ReadFromBinary(infile);

        infile.close();

        return entity;
    }

    template <Entity T>
    void FileManager::Update(const T& entity, uintmax_t position) const {
        std::string       path = GetBinaryDataPath<T>();
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
    std::vector<std::shared_ptr<T>> FileManager::Search(SearchFunc<T> predicate) const {
        std::ifstream infile(GetBinaryDataPath<T>(), std::fstream::in | std::fstream::binary);

        if (!infile.is_open())
            return {};

        std::vector<std::shared_ptr<T>> result;

        for (T entity; const auto& entityData : m_IndexTable.GetData(TypeInfo::Get<T>())) {
            if (entityData.IsDeleted) {
                infile.seekg(entityData.Position + T::GetBytesCount());
                continue;
            }

            entity.ReadFromBinary(infile);

            if (predicate(entity))
                result.emplace_back(std::make_shared<T>(entity));
        }

        infile.close();

        return result;
    }

} // namespace RefactoredProject