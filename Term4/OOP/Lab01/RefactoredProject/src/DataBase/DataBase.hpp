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

#include "Utility/Singleton.hpp"

namespace RefactoredProject {

    class DataBaseEntity {
    public:
        explicit DataBaseEntity(int id)
            : m_Id(id) {}
        virtual ~DataBaseEntity() noexcept = default;

        [[nodiscard]] inline int GetId() const { return m_Id; }

    protected:
        int m_Id = -1;

    }; // class DataBaseEntity

    class DataBase_Impl {
    public:
        using TypeId = std::size_t;

        template <typename T>
        using SearchFunc = std::function<bool(const T&)>;

        template <typename T>
        static std::string GetFileName();

        template <typename T>
        static TypeId GetHashCode();

        template <typename T>
        void Init();

        template <typename T>
        bool IsEmptyFile() const;

        template <typename T>
        T& Create();

        template <typename T, typename U>
        std::pair<T, U&> Create(int idT);

        template <typename T>
        T& Get(int id);

        template <typename T>
        std::optional<T> GetFromFile(int id) const;

        template <typename T>
        bool IsExistsInMemory(int id) const;

        template <typename T>
        bool IsExistsInFile(int id) const;

        template <typename T>
        void LoadFromFile();

        template <typename T>
        void SaveToFile();

        template <typename T>
        void DeleteFromMemory(int id);

        template <typename T>
        void DeleteFromFile(int id);

        template <typename T>
        void DeleteFromMemory();

        template <typename T>
        void DeleteFromFile();

        template <typename T>
        std::vector<T> GetFromMemory() const;

        template <typename T>
        std::vector<T> GetFromFile() const;

        template <typename T>
        std::vector<int> GetIdsFromFile() const;

        template <typename T, typename U>
        void SetOneToMany();

        template <typename T>
        void Update(const T& entity);

        template <typename T>
        std::vector<T> SearchInMemory(SearchFunc<T> predicate) const;

        template <typename T>
        std::vector<T> SearchInFile(SearchFunc<T> predicate) const;

        friend class Singleton<DataBase_Impl>;

    private:
        struct EntityData {
            std::shared_ptr<void> Handle     = nullptr;
            bool                  IsInFile   = false;
            bool                  IsDepended = false;
        };

        template <typename T>
        using EntityContainer = std::unordered_map<TypeId, std::map<int, T>>;

        DataBase_Impl();
        virtual ~DataBase_Impl() noexcept;

    private:
        EntityContainer<EntityData>       m_Entities;
        std::unordered_map<TypeId, int>   m_IdManager;
        std::filesystem::path m_WorkingDirectory;
        std::map<std::pair<std::string, std::string>, std::map<int, int>> m_Relations;

    }; // class DataBase_Impl

    using DataBase = Singleton<DataBase_Impl>;
    template <typename T>
    std::string DataBase_Impl::GetFileName() {
        return typeid(T).name();
    }

    template <typename T>
    std::size_t DataBase_Impl::GetHashCode() {
        return typeid(T).hash_code();
    }

    template <typename T>
    void DataBase_Impl::Init() {

        std::ifstream infile(m_WorkingDirectory / (GetFileName<T>() + ".txt"));
        T             entity;

        while (infile >> entity) {}

        m_IdManager[GetHashCode<T>()] = entity.GetId() + 1;

        infile.close();
    }

    template <typename T>
    bool DataBase_Impl::IsEmptyFile() const {
        return std::filesystem::file_size(m_WorkingDirectory / (GetFileName<T>() + ".txt")) == 0;
    }

    template <typename T>
    T& DataBase_Impl::Create() {
        TypeId hashCode = GetHashCode<T>();
        int    id       = m_IdManager[hashCode]++;

        auto& entity = m_Entities[hashCode][id].Handle = std::make_shared<T>(id);

        return *static_cast<T*>(entity.get());
    }

    template <typename T, typename U>
    std::pair<T, U&> DataBase_Impl::Create(int idT) {
        TypeId hashCodeT = GetHashCode<T>();
        TypeId hashCodeU = GetHashCode<U>();

        if (!m_Relations.contains({ GetFileName<T>(), GetFileName<U>() }))
            throw std::invalid_argument("RefactoredProject::Database_Impl::Create<T, U>: No such relation!");

        auto t = GetFromFile<T>(idT);

        if (!t.has_value()) {
            throw std::invalid_argument(
                "RefactoredProject::DataBase_Impl::Create<T, U>: No such record by id = " + std::to_string(idT)
            );
        }

        int   idU    = m_IdManager[hashCodeU]++;
        auto& entity = m_Entities[hashCodeU][idU].Handle = std::make_shared<U>(idU);

        m_Entities[hashCodeU][idU].IsDepended = true;
        m_Relations[{ GetFileName<T>(), GetFileName<U>() }][idT] = idU;

        return { *t, *static_cast<U*>(entity.get()) };
    }

    template <typename T>
    T& DataBase_Impl::Get(int id) {
        TypeId hashCode = GetHashCode<T>();

        if (m_Entities.contains(hashCode) && m_Entities[hashCode].contains(id))
            return *static_cast<T*>(m_Entities[hashCode][id].Handle.get());

        throw std::out_of_range("RefactoredProject::DataBase_Impl::Get: No such record on database!");
    }

    template <typename T>
    std::optional<T> DataBase_Impl::GetFromFile(int id) const {
        if (IsEmptyFile<T>())
            return std::nullopt;

        std::ifstream file(m_WorkingDirectory / (GetFileName<T>() + ".txt"));

        for (T entity; file >> entity;)
            if (entity.GetId() == id)
                return entity;

        file.close();

        return std::nullopt;
    }

    template <typename T>
    bool DataBase_Impl::IsExistsInMemory(int id) const {
        return m_Entities.contains(GetHashCode<T>()) && m_Entities.at(GetHashCode<T>()).contains(id);
    }

    template <typename T>
    bool DataBase_Impl::IsExistsInFile(int id) const {
        if (IsEmptyFile<T>())
            return false;

        std::ifstream file(m_WorkingDirectory / (GetFileName<T>() + ".txt"));

        for (T entity; file >> entity;)
            if (entity.GetId() == id)
                return true;

        file.close();

        return false;
    }

    template <typename T>
    void DataBase_Impl::LoadFromFile() {
        if (IsEmptyFile<T>())
            return;

        std::ifstream file(m_WorkingDirectory / (GetFileName<T>() + ".txt"));
        TypeId        hashCode = GetHashCode<T>();

        for (T entity; file >> entity;) {
            if (!(m_Entities.contains(hashCode) && m_Entities[hashCode].contains(entity.GetId())))
                m_Entities[hashCode][entity.GetId()].Handle = std::make_shared<T>(entity);
        }

        file.close();
    }

    template <typename T>
    void DataBase_Impl::SaveToFile() {
        if (!m_Entities.contains(GetHashCode<T>()))
            return;

        TypeId        hashCode = GetHashCode<T>();
        std::ofstream file(m_WorkingDirectory / (GetFileName<T>() + ".txt"), std::ios_base::app);

        for (auto& [id, entityData] : m_Entities[hashCode]) {
            entityData.IsInFile = true;
            file << *static_cast<T*>(entityData.Handle.get()) << std::endl;
        }

        file.close();
    }

    template <typename T>
    void DataBase_Impl::DeleteFromMemory(int id) {
        m_Entities.erase(GetHashCode<T>());
    }

    template <typename T>
    void DataBase_Impl::DeleteFromFile(int id) {
        std::vector<std::string> dependedTypes(1, GetFileName<T>());

        for (auto& [relation, ids] : m_Relations) {
            // Delete the relation
            if (relation.first == GetFileName<T>()) {
                dependedTypes.emplace_back(relation.second);
                m_Relations.erase(relation);
            }

            // Clear the ids but save the relation
            if (relation.second == GetFileName<T>())
                ids.clear();
        }

        for (const auto& filename : dependedTypes) {
            std::ofstream file(m_WorkingDirectory / (filename + ".txt"), std::ios_base::trunc);
            file.close();
        }
    }

    template <typename T>
    void DataBase_Impl::DeleteFromMemory() {
//        std::vector<TypeId> dependedTypes;
//
//        for (auto& [relation, ids] : m_Relations) {
//            // Delete the relation
//            if (relation.first == GetFileName<T>()) {
//                dependedTypes.emplace_back(relation.second);
//                m_Relations.erase(relation);
//            }
//
//            // Clear the ids but save the relation
//            if (relation.second == GetFileName<T>())
//                ids.clear();
//        }
//
//        for (const auto& filename : dependedTypes) {
//            std::ofstream file(m_WorkingDirectory / (filename + ".txt"), std::ios_base::trunc);
//            file.close();
//        }
    }

    template <typename T>
    void DataBase_Impl::DeleteFromFile() {
        std::vector<std::string> dependedTypes(1, GetFileName<T>());
        std::vector<std::pair<std::string, std::string>> relationsToDelete;

        std::cout << "Size: " << m_Relations.size() << std::endl;

        for (auto& [relation, ids] : m_Relations) {
            // Delete the relation
            std::cout << "First: " << relation.first << ", Second: " << relation.second << std::endl;

            if (relation.first == GetFileName<T>())
                dependedTypes.emplace_back(relation.second);

            // Clear the ids but save the relation
            if (relation.second == GetFileName<T>())
                ids.clear();
        }

        for (const auto& filename : dependedTypes) {
            std::ofstream file(m_WorkingDirectory / (filename + ".txt"), std::ios_base::trunc);
            file.close();
        }
    }

    template <typename T>
    std::vector<T> DataBase_Impl::GetFromMemory() const {
        if (!m_Entities.contains(GetHashCode<T>()))
            return {};

        std::vector<T> result;

        for (const auto& [id, entityData] : m_Entities.at(GetHashCode<T>()))
            result.emplace_back(*static_cast<T*>(entityData.Handle.get()));

        return result;
    }

    template <typename T>
    std::vector<T> DataBase_Impl::GetFromFile() const {
        if (IsEmptyFile<T>())
            return {};

        std::ifstream  file(m_WorkingDirectory / (GetFileName<T>() + ".txt"));
        std::vector<T> result;

        for (T entity; file >> entity;)
            result.emplace_back(entity);

        file.close();

        return result;
    }

    template <typename T>
    std::vector<int> DataBase_Impl::GetIdsFromFile() const {
        if (IsEmptyFile<T>())
            return {};

        std::vector<int> result;

        std::ifstream file(m_WorkingDirectory / (GetFileName<T>() + ".txt"));
        T             entity;

        while (file >> entity)
            result.emplace_back(entity.GetId());

        file.close();

        return result;
    }

    template <typename T, typename U>
    void DataBase_Impl::SetOneToMany() {
        m_Relations[{ GetFileName<T>(), GetFileName<U>() }];
    }

    template <typename T>
    void DataBase_Impl::Update(const T& entity) {
        if (IsExistsInMemory<T>(entity.GetId()))
            Get<T>(entity.GetId()) = entity;

        if (IsEmptyFile<T>())
            return;

        // Read from file
        std::ifstream infile(m_WorkingDirectory / (GetFileName<T>() + ".txt"));
        std::vector<T> temp;

        for (T elem; infile >> elem;)
            temp.emplace_back(elem.GetId() == entity.GetId() ? entity : elem);

        infile.close();

        // Write to file
        std::ofstream outfile(m_WorkingDirectory / (GetFileName<T>() + ".txt"));

        for (const auto& elem : temp)
            outfile << elem << std::endl;

        outfile.close();
    }

    template <typename T>
    std::vector<T> DataBase_Impl::SearchInMemory(SearchFunc<T> predicate) const {
        TypeId hashCode = GetHashCode<T>();

        if (!m_Entities.contains(hashCode))
            return {};

        std::vector<T> result;

        for (const auto& [id, entityData] : m_Entities.at(hashCode)) {
            auto entity = *static_cast<T*>(entityData.Handle.get());

            if (predicate(entity))
                result.emplace_back(std::move(entity));
        }

        return result;
    }

    template <typename T>
    std::vector<T> DataBase_Impl::SearchInFile(SearchFunc<T> predicate) const {
        if (IsEmptyFile<T>())
            return {};

        std::vector<T> result;

        std::ifstream file(m_WorkingDirectory / (GetFileName<T>() + ".txt"));

        for (T entity; file >> entity;)
            if (predicate(entity))
                result.emplace_back(std::move(entity));

        file.close();

        return result;
    }

} // namespace RefactoredProject