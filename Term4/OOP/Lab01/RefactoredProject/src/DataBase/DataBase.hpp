#pragma once

#include <unordered_map>
#include <vector>
#include <fstream>
#include <optional>
#include <iostream>
#include <memory>

#include "Utility/Singleton.hpp"

namespace RefactoredProject {

    class DataBase_Impl {
    public:
        template <typename T, typename... Args>
        T& Create(Args&&... args);

        template <typename T>
        T& Get(int id);

        template <typename T>
        const T& Get(int id) const;

        template <typename T>
        void SaveToFile() const;

        template <typename T>
        void Print() const;

        friend class Singleton<DataBase_Impl>;

    private:
        using EntitiesContainer = std::unordered_map<std::size_t, std::vector<std::shared_ptr<void>>>;

        template <typename T>
        class EntityIdManager {
        public:
            [[nodiscard]] int GetId() { return m_Ids[typeid(T).hash_code()]++; }

        private:
            std::unordered_map<std::size_t, int> m_Ids;

        }; // EntityIdManager

        DataBase_Impl() = default;
        virtual ~DataBase_Impl() noexcept;

    private:
        EntitiesContainer m_Entities;

    }; // class DataBase_Impl

    template <typename T, typename... Args>
    T& DataBase_Impl::Create(Args&&... args) {
        return *static_cast<T*>(
            m_Entities[typeid(T).hash_code()].emplace_back(
                std::make_shared<T>(std::forward<Args>(args)...)
            ).get()
        );
    }

    template <typename T>
    T& DataBase_Impl::Get(int id) {
        auto& entities = m_Entities[typeid(T).hash_code()];
        auto it = std::find_if(entities.begin(), entities.end(), [id](const auto& entity) {
            return static_cast<T*>(entity.get())->GetId() == id;
        });

        if (it != entities.end())
            return *static_cast<T*>((*it).get());

        throw std::out_of_range("RefactoredProject::DataBase_Impl::Get: No such record on database!");
    }

    template <typename T>
    const T& DataBase_Impl::Get(int id) const {
        const auto& entities = m_Entities.at(typeid(T).hash_code());
        auto it = std::find_if(entities.begin(), entities.end(), [id](const auto& entity) {
            return static_cast<T*>(entity.get())->GetId() == id;
        });

        if (it != entities.end())
            return *static_cast<T*>((*it).get());

        throw std::out_of_range("RefactoredProject::DataBase_Impl::Get: No such record on database!");
    }

    template <typename T>
    void DataBase_Impl::SaveToFile() const {
        std::string   filename = typeid(T).name();
        std::ofstream file("../media/DataBaseFiles/" + filename + ".txt");

        for (const auto& entity : m_Entities.at(typeid(T).hash_code()))
            file << *static_cast<T*>(entity) << std::endl;


        file.close();
    }

    template <typename T>
    void DataBase_Impl::Print() const {
        std::string   filename = typeid(T).name();
        std::ifstream file("../media/DataBaseFiles/" + filename + ".txt");
        T             entity;

        while (file >> entity)
            std::cout << entity << std::endl;

        file.close();
    }

    using DataBase = Singleton<DataBase_Impl>;

} // namespace RefactoredProject