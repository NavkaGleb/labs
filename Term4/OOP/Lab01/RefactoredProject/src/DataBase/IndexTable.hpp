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
        struct EntityData {
            int       Id        = -1;
            uintmax_t Position  = 0;
            bool      IsDeleted = false;

        }; // struct EntityData

        using EntityDataContainer = std::vector<EntityData>;

        IndexTable() = default;
        virtual ~IndexTable();

        template <Entity T> void Init();

        [[nodiscard]] bool IsExists(const TypeInfo& typeInfo) const;
        [[nodiscard]] bool IsExists(const TypeInfo& typeInfo, int id) const;

        [[nodiscard]] int GetNewId(const TypeInfo& typeInfo);
        [[nodiscard]] std::optional<uintmax_t> GetPosition(const TypeInfo& typeInfo, int id) const;

        [[nodiscard]] inline int GetBytesCount(const TypeInfo& typeInfo) const { return m_BytesCount.at(typeInfo); }

        [[nodiscard]] EntityDataContainer& GetData(const TypeInfo& typeInfo) { return m_Table[typeInfo]; }
        [[nodiscard]] const EntityDataContainer& GetData(const TypeInfo& typeInfo) const { return m_Table.at(typeInfo); }

        void SetIsDeleted(const TypeInfo& typeInfo, int id, bool isDeleted);

        void Push(const TypeInfo& typeInfo, int id, uintmax_t position);

        void Print() const;

    private:
        [[nodiscard]] EntityData* GetEntityData(const TypeInfo& typeInfo, int id) const;

    private:
        std::map<TypeInfo, EntityDataContainer> m_Table;
        std::map<TypeInfo, int>                 m_Ids;
        std::map<TypeInfo, int>                 m_BytesCount;

    }; // namespace IndexTable

    template <Entity T>
    void IndexTable::Init() {
        std::string   path = GetIndexTablePath<T>();
        std::ifstream infile(path, std::fstream::in | std::fstream::binary | std::fstream::app);

        if (!infile.is_open())
            throw std::runtime_error("RefactoredProject::IndexTable::Init: Can not open the file!");

        uintmax_t size = std::filesystem::file_size(path);

        if (size == 0) {
            m_Ids[TypeInfo::Get<T>()] = -1;
            return;
        }

        for (EntityData entityData; true;) {
            infile.read(reinterpret_cast<char*>(&entityData), sizeof(entityData));

            m_Table[TypeInfo::Get<T>()].emplace_back(entityData);

            if (infile.tellg() == size)
                break;
        }

        m_Ids[TypeInfo::Get<T>()]        = m_Table[TypeInfo::Get<T>()].back().Id;
        m_BytesCount[TypeInfo::Get<T>()] = T::GetBytesCount();

        infile.close();
    }

} // namespace RefactoredProject