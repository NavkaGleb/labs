#pragma once

#include <unordered_map>
#include <map>
#include <vector>
#include <fstream>
#include <optional>
#include <iostream>
#include <memory>
#include <filesystem>
#include <typeinfo>
#include <tuple>
#include <functional>
#include <type_traits>

#include "MemoryManager.hpp"
#include "FileManager.hpp"
#include "Utility/Singleton.hpp"

namespace RefactoredProject {

    class DataBaseEntity {
    public:
        explicit DataBaseEntity(int id)
            : m_Id(id) {}
        virtual ~DataBaseEntity() noexcept = default;

        [[nodiscard]] inline int GetId() const { return m_Id; }

        virtual void ReadFromBinary(std::ifstream& infile) = 0;
        virtual void WriteToBinary(std::ofstream& outfile) const = 0;

    protected:
        int m_Id = -1;

    }; // class DataBaseEntity

    template <typename T>
    concept Entity = std::is_base_of<DataBaseEntity, T>::value;

    class DataBase_Impl {
    public:
        using TypeId = std::size_t;

        template <typename T>
        using SearchFunc = std::function<bool(const T&)>;

        template <Entity T> void Init();

        template <Entity T> T& Create();

        template <Entity T> T& GetFromMemory(int id);
        template <Entity T> T GetFromFile(int id) const;
        template <Entity T> std::vector<std::shared_ptr<T>> GetFromMemory() const;
        template <Entity T> std::vector<std::shared_ptr<T>> GetFromFile() const;

        template <Entity T> void Load();
        template <Entity T> void Save();

        template <Entity T> void DeleteFromMemory(int id);
        template <Entity T> void DeleteFromFile(int id);
        template <Entity T> void DeleteFromMemory();
        template <Entity T> void DeleteFromFile();

        template <typename T>
        std::vector<int> GetIdsFromFile() const;

        template <Entity T, Entity U> void SetOneToMany();

        template <typename T>
        void Update(const T& entity);

        template <typename T>
        std::vector<T> SearchInMemory(SearchFunc<T> predicate) const;

        template <typename T>
        std::vector<T> SearchInFile(SearchFunc<T> predicate) const;

        friend class Singleton<DataBase_Impl>;
        friend class MemoryManager;
        friend class FileManager;

    private:
        using EntityPosition = std::pair<int, uintmax_t>;
        using IndexTable     = std::unordered_map<std::size_t, std::vector<EntityPosition>>;
        using RelationTable  = std::unordered_map<std::pair<std::size_t, std::size_t>, std::map<int, std::vector<int>>>;

        DataBase_Impl();
        virtual ~DataBase_Impl() noexcept;

    private:
        IndexTable    m_IndexTable;
        RelationTable m_RelationTable;

        MemoryManager m_MemoryManager;
        FileManager   m_FileManager;

        std::filesystem::path                                   m_WorkingDirectory;

    }; // class DataBase_Impl

    using DataBase = Singleton<DataBase_Impl>;

    template <Entity T>
    void DataBase_Impl::Init() {
        std::string   path = GetIndexTablePath(GetHash<T>());
        uintmax_t     size = std::filesystem::file_size(path);
        std::ifstream infile(path, std::fstream::in | std::fstream::binary);

        if (!infile.is_open())
            return;

        for (EntityPosition data; true;) {
            infile.read(reinterpret_cast<char*>(&data), sizeof(data));
            m_IndexTable[GetHash<T>()].emplace_back(data);

            if (infile.tellg() == size)
                break;
        }

        infile.close();
    }

    template <Entity T>
    T& DataBase_Impl::Create() {
        return m_MemoryManager.Create<T>(m_IndexTable[GetHash<T>()]++);
    }

    template <Entity T>
    T& DataBase_Impl::GetFromMemory(int id) {
        return m_MemoryManager.Get<T>(id);
    }

    template <Entity T>
    T DataBase_Impl::GetFromFile(int id) const {
        if (!m_IndexTable.contains(GetHash<T>())) {
            throw std::invalid_argument(
                "RefactoredProject::DataBase_Impl::GetFromFile: No such type in m_IndexTable!"
            );
        }

        auto* entityPosition = static_cast<EntityPosition*>(
            std::bsearch(
                &id,
                m_IndexTable[GetHash<T>()].data(),
                m_IndexTable[GetHash<T>()].size(),
                sizeof(EntityPosition),
                [](const void* lhs, const void* rhs) {
                    const auto& left  = *static_cast<const EntityPosition*>(lhs);
                    const auto& right = *static_cast<const EntityPosition*>(rhs);

                    if (left.first < right.first)
                        return -1;

                    if (left.first > right.first)
                        return 1;

                    return 0;
                }
            )
        );

        if (!entityPosition) {
            throw std::range_error(
                "RefactoredProject::DataBase_Impl::GetFromFile: No record by given id!"
            );
        }

        return m_FileManager.Get<T>(entityPosition->first);
    }

    template <Entity T>
    std::vector<std::shared_ptr<T>> DataBase_Impl::GetFromMemory() const {
        return m_MemoryManager.Get<T>();
    }

    template <Entity T>
    std::vector<std::shared_ptr<T>> DataBase_Impl::GetFromFile() const {
        return m_FileManager.Get<T>();
    }

    template <Entity T>
    void DataBase_Impl::Load() {
        return m_MemoryManager.Load<T>();
    }

    template <Entity T>
    void DataBase_Impl::Save() {
        return m_MemoryManager.Save<T>();
    }

    template <Entity T>
    void DataBase_Impl::DeleteFromMemory(int id) {
        if (m_MemoryManager.IsInFile<T>(id))
            return m_MemoryManager.Delete<T>(id);

        for (auto& [relation, ids] : m_RelationTable) {
            if (relation.first == TypeInfo::GetHash<T>()) {
                for (auto& [id, dependedId] : ids)
                    m_MemoryManager.Delete<T>(dependedId);
                    // TODO: Delete by hash!!!
            }
        }
    }

    template <Entity T>
    void DataBase_Impl::DeleteFromFile(int id) {

    }

    template <Entity T>
    void DataBase_Impl::DeleteFromMemory() {
        return m_MemoryManager.Delete<T>();
    }

    template <Entity T>
    void DataBase_Impl::DeleteFromFile() {
        for (auto& [relation, ids] : m_Relations) {
            if (relation.first == GetHash<T>()) {
                m_Entities[relation.first].clear();
                m_Entities[relation.second].clear();

                ClearFile(m_Types[relation.second].IdFilePath);
                ClearFile(m_Types[relation.second].TxtFilePath);
                ClearFile(m_Types[relation.second].BinFilePath);
            }

            if (relation.second == GetHash<T>()) {
                m_Entities[relation.second].clear();

                ClearFile(m_Types[relation.second].IdFilePath);
                ClearFile(m_Types[relation.second].TxtFilePath);
                ClearFile(m_Types[relation.second].BinFilePath);
            }
        }

        ClearFile(m_Types[GetHash<T>()].IdFilePath);
        ClearFile(m_Types[GetHash<T>()].TxtFilePath);
        ClearFile(m_Types[GetHash<T>()].BinFilePath);
    }

    template <typename T>
    std::vector<int> DataBase_Impl::GetIdsFromFile() const {
//        if (IsEmptyFile<T>())
//            return {};
//
//        std::vector<int> result;
//
//        std::ifstream file(m_WorkingDirectory / (GetFileName<T>() + ".txt"));
//        T             entity;
//
//        while (file >> entity)
//            result.emplace_back(entity.GetId());
//
//        file.close();
//
//        return result;


        return {};
    }

    template <Entity T, Entity U>
    void DataBase_Impl::SetOneToMany() {
        m_RelationTable[{ TypeInfo::GetHash<T>(), TypeInfo::GetHash<U>() }];
    }

    template <typename T>
    void DataBase_Impl::Update(const T& entity) {
//        if (IsExistsInMemory<T>(entity.GetId()))
//            Get<T>(entity.GetId()) = entity;
//
//        if (IsEmptyFile<T>())
//            return;
//
//        // Read from file
//        std::ifstream infile(m_WorkingDirectory / (GetFileName<T>() + ".txt"));
//        std::vector<T> temp;
//
//        for (T elem; infile >> elem;)
//            temp.emplace_back(elem.GetId() == entity.GetId() ? entity : elem);
//
//        infile.close();
//
//        // Write to file
//        std::ofstream outfile(m_WorkingDirectory / (GetFileName<T>() + ".txt"));
//
//        for (const auto& elem : temp)
//            outfile << elem << std::endl;
//
//        outfile.close();
    }

    template <typename T>
    std::vector<T> DataBase_Impl::SearchInMemory(SearchFunc<T> predicate) const {
//        TypeId hashCode = GetHashCode<T>();
//
//        if (!m_Entities.contains(hashCode))
//            return {};
//
//        std::vector<T> result;
//
//        for (const auto& [id, entityData] : m_Entities.at(hashCode)) {
//            auto entity = *static_cast<T*>(entityData.Handle.get());
//
//            if (predicate(entity))
//                result.emplace_back(std::move(entity));
//        }
//
//        return result;
    }

    template <typename T>
    std::vector<T> DataBase_Impl::SearchInFile(SearchFunc<T> predicate) const {
//        if (IsEmptyFile<T>())
//            return {};
//
//        std::vector<T> result;
//
//        std::ifstream file(m_WorkingDirectory / (GetFileName<T>() + ".txt"));
//
//        for (T entity; file >> entity;)
//            if (predicate(entity))
//                result.emplace_back(std::move(entity));
//
//        file.close();
//
//        return result;
        return {};
    }

} // namespace RefactoredProject