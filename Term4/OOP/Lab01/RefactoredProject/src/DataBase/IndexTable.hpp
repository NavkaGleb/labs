#pragma once

#include <map>
#include <vector>
#include <cstdint>
#include <optional>
#include <fstream>
#include <filesystem>
#include <iostream>

#include "TypeInfo.hpp"
#include "IDataBaseEntity.hpp"

namespace RefactoredProject {

    class IndexTable {
    public:
        using EntityData = std::pair<int, uintmax_t>;

        IndexTable() = default;
        virtual ~IndexTable();

        template <Entity T> void Init();

        template <Entity T> [[nodiscard]] bool IsExists() const;

        template <Entity T> [[nodiscard]] int GetNewId();
        template <Entity T> [[nodiscard]] std::optional<uintmax_t> GetPosition(int id) const;

        template <Entity T> void Push(const EntityData& entityData);

        void Print() const;

    private:
        std::map<std::size_t, std::vector<EntityData>> m_Table;
        std::map<std::size_t, int>                     m_Ids;

    }; // namespace IndexTable

    template <Entity T>
    void IndexTable::Init() {
        std::string   path = TypeInfo::GetIndexTablePath(TypeInfo::GetHash<T>());
        std::ifstream infile(path, std::fstream::in | std::fstream::binary | std::fstream::app);

        if (!infile.is_open())
            throw std::runtime_error("RefactoredProject::IndexTable::Init: Can not open the file!");

        uintmax_t size = std::filesystem::file_size(path);

        if (size == 0) {
            m_Ids[TypeInfo::GetHash<T>()] = -1;
            return;
        }

        for (EntityData entityData; true;) {
            infile.read(reinterpret_cast<char*>(&entityData), sizeof(entityData));
            m_Table[TypeInfo::GetHash<T>()].emplace_back(entityData);

            if (infile.tellg() == size)
                break;
        }

        m_Ids[TypeInfo::GetHash<T>()] = m_Table[TypeInfo::GetHash<T>()].back().first;

        infile.close();
    }

    template <Entity T>
    bool IndexTable::IsExists() const {
        return m_Table.contains(TypeInfo::GetHash<T>());
    }

    template <Entity T>
    int IndexTable::GetNewId() {
        return ++m_Ids[TypeInfo::GetHash<T>()];
    }

    template <Entity T>
    std::optional<uintmax_t> IndexTable::GetPosition(int id) const {
        auto* entityData = static_cast<EntityData*>(
            std::bsearch(
                &id,
                m_Table.at(TypeInfo::GetHash<T>()).data(),
                m_Table.at(TypeInfo::GetHash<T>()).size(),
                sizeof(EntityData),
                [](const void* lhs, const void* rhs) {
                    const auto& left  = *static_cast<const EntityData*>(lhs);
                    const auto& right = *static_cast<const EntityData*>(rhs);

                    if (left.first < right.first)
                        return -1;

                    if (left.first > right.first)
                        return 1;

                    return 0;
                }
            )
        );

        return !entityData ? std::nullopt : std::optional(entityData->second);
    }

    template <Entity T>
    void IndexTable::Push(const EntityData& entityData) {
        m_Table[TypeInfo::GetHash<T>()].emplace_back(entityData);
    }

} // namespace RefactoredProject