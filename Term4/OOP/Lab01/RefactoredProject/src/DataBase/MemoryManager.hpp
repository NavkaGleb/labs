#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <memory>
#include <type_traits>
#include <fstream>
#include <filesystem>

#include "TypeInfo.hpp"

namespace RefactoredProject {

    class MemoryManager {
    public:
        explicit MemoryManager(std::unordered_map<std::size_t, std::vector<std::pair<int, uintmax_t>>>& indexTable);
        virtual ~MemoryManager() noexcept;

        [[nodiscard]] inline const auto& GetEntities() const { return m_Entities; }

        template <typename T> T& Create(int id);

        template <typename T> bool IsExists() const;
        template <typename T> bool IsExists(int id) const;
        template <typename T> bool IsInFile(int id) const;

        template <typename T> T& Get(int id);
        template <typename T> std::vector<std::shared_ptr<T>> Get();

        template <typename T> void Load();
        template <typename T> void Save();

        template <typename T> void Delete(int id);
        template <typename T> void Delete();

    private:
        struct EntityData {
            std::shared_ptr<void> Handle   = nullptr;
            bool                  IsInFile = false;

        }; // struct EntityData

    private:
        std::unordered_map<std::size_t, std::vector<std::pair<int, uintmax_t>>>& m_IndexTable;
        std::unordered_map<std::size_t, std::map<int, EntityData>>               m_Entities;

    }; // class MemoryManger

    template <typename T>
    T& MemoryManager::Create(int id) {
        return *static_cast<T*>((m_Entities[TypeInfo::GetHash<T>][id].Handle = std::make_shared<T>(id)).get());
    }

    template <typename T>
    bool MemoryManager::IsExists() const {
        return m_Entities.contains(TypeInfo::GetHash<T>());
    }

    template <typename T>
    bool MemoryManager::IsExists(int id) const {
        return m_Entities.contains(TypeInfo::GetHash<T>()) && m_Entities.at(TypeInfo::GetHash<T>()).contains(id);
    }

    template <typename T>
    bool MemoryManager::IsInFile(int id) const {
        return IsExists<T>(id) && m_Entities[TypeInfo::GetHash<T>()][id].IsInFile;
    }

    template <typename T>
    T& MemoryManager::Get(int id) {
        if (IsExists<T>(id))
            return *static_cast<T*>(m_Entities[GetHash<T>()][id].Handle.get());

        throw std::range_error("RefactoredProject::MemoryManager: Don't exists record with id = " + std::to_string(id));
    }

    template <typename T>
    std::vector<std::shared_ptr<T>> MemoryManager::Get() {
        if (!IsExists<T>()) {
            throw std::range_error(
                "RefactoredProject::MemoryManager: Don't exists such type" + std::string(typeid(T).name())
            );
        }

        std::vector<T*> result;

        for (auto& [id, entityData] : m_Entities[GetHash<T>()])
            result.emplace_back(static_cast<std::shared_ptr<T>>(entityData.Handle));

        return result;
    }

    template <typename T>
    void MemoryManager::Load() {
        auto          hash = TypeInfo::GetHash<T>();
        auto          path = TypeInfo::GetBinaryDataPath(hash);
        auto          size = std::filesystem::file_size(path);
        std::ifstream file(path, std::fstream::in | std::fstream::binary);

        for (T entity; true;) {
            entity.ReadFromBinary(file);

            if (!IsExists<T>(entity.GetId())) {
                auto& entityData    = m_Entities[hash][entity.GetId()];
                entityData.Handle   = std::make_shared<T>(std::move(entity));
                entityData.IsInFile = true;
            }

            if (size == file.tellg())
                break;
        }

        file.close();
    }

    template <typename T>
    void MemoryManager::Save() {
        using namespace TypeInfo;

        if (!IsExists<T>())
            return;

        auto          hash = GetHash<T>();
        std::ofstream textFile(GetTextDataPath(hash),     std::fstream::out | std::fstream::app                       );
        std::ofstream binaryFile(GetBinaryDataPath(hash), std::fstream::out | std::fstream::app | std::fstream::binary);

        for (auto& [id, entityData] : m_Entities[hash]) {
            if (entityData.IsInFile)
                continue;

            T& entity             = *static_cast<T*>(entityData.Handle.get());
            entityData.IsInFile   = true;

            m_IndexTable[hash].emplace_back(entity.GetId(), binaryFile.tellp());

            textFile << entity << std::endl;
            entity.WriteToBinary(binaryFile);
        }

        textFile.close();
        binaryFile.close();
    }

    template <typename T>
    void MemoryManager::Delete(int id) {
        if (!IsExists<T>(id))
            return;

        m_Entities[TypeInfo::GetHash<T>()].erase(id);
    }

    template <typename T>
    void MemoryManager::Delete() {
        if (!IsExists<T>())
            return;

        m_Entities[TypeInfo::GetHash<T>()].clear();
    }

} // namespace RefactoredProject