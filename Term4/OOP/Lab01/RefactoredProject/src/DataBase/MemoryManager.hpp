#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <memory>
#include <type_traits>
#include <fstream>
#include <filesystem>
#include <iostream>

#include "IndexTable.hpp"
#include "TypeInfo.hpp"
#include "IDataBaseEntity.hpp"

namespace RefactoredProject {

    class MemoryManager {
    public:
        explicit MemoryManager(IndexTable& indexTable);
        virtual ~MemoryManager() noexcept;

        [[nodiscard]] inline const auto& GetEntities() const { return m_Entities; }

        template <Entity T> T& Create(int id);

        [[nodiscard]] bool IsExists(const TypeInfo& typeInfo) const;
        [[nodiscard]] bool IsExists(const TypeInfo& typeInfo, int id) const;
        template <Entity T> [[nodiscard]] bool IsExists() const;
        template <Entity T> [[nodiscard]] bool IsExists(int id) const;
        template <Entity T> [[nodiscard]] bool IsInFile(int id) const;

        template <Entity T> std::vector<std::shared_ptr<T>> Get() const;
        template <Entity T> T& Get(int id);

        template <Entity T> void Load();
        template <Entity T> void Save();

        template <Entity T> void Delete();
        template <Entity T> void Delete(int id);

        void Delete(const TypeInfo& typeInfo);
        void Delete(const TypeInfo& typeInfo, int id);

        template <Entity T> void Update(const T& entity);

        template <Entity T> std::vector<std::shared_ptr<T>> Search(SearchFunc<T> predicate);

    private:
        struct EntityData {
            std::shared_ptr<void> Handle   = nullptr;
            bool                  IsInFile = false;

        }; // struct EntityData

        using EntityContainer = std::map<TypeInfo, std::map<int, EntityData>>;

    private:
        IndexTable&     m_IndexTable;
        EntityContainer m_Entities;

    }; // class MemoryManger

    template <Entity T>
    T& MemoryManager::Create(int id) {
        return *static_cast<T*>((m_Entities[TypeInfo::Get<T>()][id].Handle = std::make_shared<T>(id)).get());
    }

    template <Entity T>
    bool MemoryManager::IsExists() const {
        return m_Entities.contains(TypeInfo::Get<T>());
    }

    template <Entity T>
    bool MemoryManager::IsExists(int id) const {
        return m_Entities.contains(TypeInfo::Get<T>()) && m_Entities.at(TypeInfo::Get<T>()).contains(id);
    }

    template <Entity T>
    bool MemoryManager::IsInFile(int id) const {
        return IsExists<T>(id) && m_Entities.at(TypeInfo::Get<T>()).at(id).IsInFile;
    }

    template <Entity T>
    std::vector<std::shared_ptr<T>> MemoryManager::Get() const {
        if (!IsExists<T>())
            return {};

        std::vector<std::shared_ptr<T>> result;

        for (const auto& [id, entityData] : m_Entities.at(TypeInfo::Get<T>()))
            result.emplace_back(std::static_pointer_cast<T>(entityData.Handle));

        return result;
    }

    template <Entity T>
    T& MemoryManager::Get(int id) {
        if (IsExists<T>(id))
            return *static_cast<T*>(m_Entities[TypeInfo::Get<T>()][id].Handle.get());

        throw std::range_error("RefactoredProject::MemoryManager: Don't exists record with id = " + std::to_string(id));
    }

    template <Entity T>
    void MemoryManager::Load() {
        auto          path = GetBinaryDataPath<T>();
        auto          size = std::filesystem::file_size(path);
        std::ifstream file(path, std::fstream::in | std::fstream::binary);

        for (T entity; true;) {
            entity.ReadFromBinary(file);

            if (!IsExists<T>(entity.GetId())) {
                auto& entityData    = m_Entities[TypeInfo::Get<T>()][entity.GetId()];
                entityData.Handle   = std::make_shared<T>(std::move(entity));
                entityData.IsInFile = true;
            }

            if (size == file.tellg())
                break;
        }

        file.close();
    }

    template <Entity T>
    void MemoryManager::Save() {
        if (!IsExists<T>())
            return;

        std::ofstream textFile(GetTextDataPath<T>(),     std::fstream::out | std::fstream::app                       );
        std::ofstream binaryFile(GetBinaryDataPath<T>(), std::fstream::out | std::fstream::app | std::fstream::binary);

        binaryFile.seekp(std::filesystem::file_size(GetBinaryDataPath<T>()));

        for (auto& [id, entityData] : m_Entities[TypeInfo::Get<T>()]) {
            if (entityData.IsInFile)
                continue;

            T& entity             = *static_cast<T*>(entityData.Handle.get());
            entityData.IsInFile   = true;

            m_IndexTable.Push<T>({ entity.GetId(), binaryFile.tellp() });

            textFile << entity << std::endl;
            entity.WriteToBinary(binaryFile);
        }

        m_IndexTable.Print();

        textFile.close();
        binaryFile.close();
    }

    template <Entity T>
    void MemoryManager::Delete() {
        if (!IsExists<T>())
            return;

        m_Entities[TypeInfo::Get<T>()].clear();
    }

    template <Entity T>
    void MemoryManager::Delete(int id) {
        if (!IsExists<T>(id))
            return;

        m_Entities[TypeInfo::Get<T>()].erase(id);
    }

    template <Entity T>
    void MemoryManager::Update(const T& entity) {
        m_Entities[TypeInfo::Get<T>()][entity.GetId()].Handle = std::make_shared<T>(entity);
    }

    template <Entity T>
    std::vector<std::shared_ptr<T>> MemoryManager::Search(SearchFunc<T> predicate) {
        if (!IsExists<T>())
            return {};

        std::vector<std::shared_ptr<T>> result;

        for (auto& [id, entityData] : m_Entities[TypeInfo::Get<T>()]) {
            auto& entity = static_cast<std::shared_ptr<T>>(entityData.Handle);

            if (predicate(entity))
                result.emplace_back(entity);
        }

        return result;
    }

} // namespace RefactoredProject