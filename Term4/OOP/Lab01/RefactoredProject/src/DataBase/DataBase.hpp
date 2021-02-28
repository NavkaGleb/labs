#pragma once

#include <unordered_map>
#include <vector>
#include <fstream>
#include <optional>
#include <iostream>
#include <memory>
#include <map>
#include <filesystem>

#include "Utility/Singleton.hpp"

namespace RefactoredProject {

    class DataBase_Impl {
    public:
        template <typename T>
        static std::string GetFileName();

        template <typename T>
        static std::size_t GetHashCode();

        template <typename T>
        bool IsEmptyFile() const;

        template <typename T>
        T& Create();

        template <typename T>
        T& Get(int id);

        template <typename T>
        const T& Get(int id) const;

        template <typename T>
        bool IsExists(int id) const;

        template <typename T>
        void SaveToFile();

        template <typename T>
        void LoadFromFile();

        template <typename T>
        bool PrintFromFile() const;

        template <typename T>
        bool PrintFromMemory() const;

        template <typename T>
        void DeleteFromFile() const;

        template <typename T>
        void DeleteFromMemory();

        template <typename T>
        std::vector<int> GetIdsFromFile() const;

        friend class Singleton<DataBase_Impl>;

    private:
        template <typename T>
        using EntityContainer = std::unordered_map<std::size_t, std::map<int, T>>;

        DataBase_Impl();
        virtual ~DataBase_Impl() noexcept;

    private:
        EntityContainer<std::shared_ptr<void>> m_Entities;
        EntityContainer<bool>                  m_FileManager;
        std::filesystem::path                  m_WorkingDirectory;


    }; // class DataBase_Impl

    template <typename T>
    std::string DataBase_Impl::GetFileName() {
        return typeid(T).name();
    }

    template <typename T>
    std::size_t DataBase_Impl::GetHashCode() {
        return typeid(T).hash_code();
    }

    template <typename T>
    bool DataBase_Impl::IsEmptyFile() const {
        return std::filesystem::file_size(m_WorkingDirectory / (GetFileName<T>() + ".txt")) == 0;
    }

    template <typename T>
    T& DataBase_Impl::Create() {
        std::size_t hashCode = GetHashCode<T>();
        int         entityId = m_Entities[hashCode].size();

        auto& entity = m_Entities[hashCode][entityId] = std::make_shared<T>(entityId);

        return *static_cast<T*>(entity.get());
    }

    template <typename T>
    T& DataBase_Impl::Get(int id) {
        std::size_t hashCode = GetHashCode<T>();

        if (m_Entities.contains(hashCode) && m_Entities[hashCode].contains(id))
            return *static_cast<T*>(m_Entities[hashCode][id].get());

        throw std::out_of_range("RefactoredProject::DataBase_Impl::Get: No such record on database!");
    }

    template <typename T>
    const T& DataBase_Impl::Get(int id) const {
        std::size_t hashCode = GetHashCode<T>();

        if (m_Entities.contains(hashCode) && m_Entities.at(hashCode).contains(id))
            return *static_cast<T*>(m_Entities.at(hashCode).at(id).get());

        throw std::out_of_range("RefactoredProject::DataBase_Impl::Get: No such record on database!");
    }

    template <typename T>
    bool DataBase_Impl::IsExists(int id) const {
        return m_Entities.contains(GetHashCode<T>()) && m_Entities.at(GetHashCode<T>()).contains(id);
    }

    template <typename T>
    void DataBase_Impl::SaveToFile() {
        std::size_t hashCode = GetHashCode<T>();
        std::ofstream file(m_WorkingDirectory / (GetFileName<T>() + ".txt"), std::ios_base::app);

        for (const auto& [id, entity] : m_Entities.at(hashCode)) {
            if (!(m_FileManager.contains(hashCode) && m_FileManager.at(hashCode).contains(id))) {
                m_FileManager[hashCode][id] = true;
                file << *static_cast<T*>(entity.get()) << std::endl;
            }
        }

        file.close();
    }

    template <typename T>
    void DataBase_Impl::LoadFromFile() {
        if (IsEmptyFile<T>())
            return;

        std::ifstream file(m_WorkingDirectory / (GetFileName<T>() + ".txt"));
        std::size_t   hashCode = GetHashCode<T>();
        T             entity;

        while (file >> entity) {
            if (!(m_Entities.contains(hashCode) && m_Entities[hashCode].contains(entity.GetId())))
                m_Entities[hashCode][entity.GetId()] = std::make_shared<T>(entity);
        }

        file.close();
    }

    template <typename T>
    bool DataBase_Impl::PrintFromFile() const {
        if (IsEmptyFile<T>())
            return false;

        std::ifstream file(m_WorkingDirectory / (GetFileName<T>() + ".txt"));
        T             entity;

        while (file >> entity)
            std::cout << entity << std::endl;

        file.close();

        return true;
    }

    template <typename T>
    bool DataBase_Impl::PrintFromMemory() const {
        if (!m_Entities.contains(GetHashCode<T>()))
            return false;

        for (const auto& [id, entity] : m_Entities.at(GetHashCode<T>()))
            std::cout << *static_cast<T*>(entity.get()) << std::endl;

        return true;
    }

    template <typename T>
    void DataBase_Impl::DeleteFromFile() const {
        std::ofstream file(m_WorkingDirectory / (GetFileName<T>() + ".txt"), std::ios_base::trunc);
        file.close();
    }

    template <typename T>
    void DataBase_Impl::DeleteFromMemory() {
        m_Entities.erase(GetHashCode<T>());
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

    using DataBase = Singleton<DataBase_Impl>;

} // namespace RefactoredProject