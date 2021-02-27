#pragma once

#include <unordered_map>
#include <vector>
#include <fstream>
#include <optional>

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

        friend class Singleton<DataBase_Impl>;

    private:
        using EntitiesContainer = std::unordered_map<std::size_t, std::vector<void*>>;

        template <typename T>
        class EntityIdManager {
        public:
            [[nodiscard]] int GetId() { return m_Ids[typeid(T).hash_code()]++; }

        private:
            std::unordered_map<std::size_t, int> m_Ids;

        }; // EntityIdManager

        DataBase_Impl() = default;
        virtual ~DataBase_Impl() = default;

    private:
        EntitiesContainer m_Entities;

    }; // class DataBase_Impl

    template <typename T, typename... Args>
    T& DataBase_Impl::Create(Args&&... args) {
        return *static_cast<T*>(m_Entities[typeid(T).hash_code()].emplace_back(new T(std::forward<Args>(args)...)));
    }

    template <typename T>
    T& DataBase_Impl::Get(int id) {
        auto& entities = m_Entities[typeid(T).hash_code()];
        auto it = std::find_if(entities.begin(), entities.end(), [id](const auto& entity) {
            return static_cast<T*>(entity)->GetId() == id;
        });

        if (it != entities.end())
            return *static_cast<T*>(*it);

        throw std::out_of_range("RefactoredProject::DataBase_Impl::Get: No such record on database!");
    }

    template <typename T>
    const T& DataBase_Impl::Get(int id) const {
        const auto& entities = m_Entities.at(typeid(T).hash_code());
        auto it = std::find_if(entities.begin(), entities.end(), [id](const auto& entity) {
            return static_cast<T*>(entity)->GetId() == id;
        });

        if (it != entities.end())
            return *static_cast<T*>(*it);

        throw std::out_of_range("RefactoredProject::DataBase_Impl::Get: No such record on database!");
    }

    template <typename T>
    void DataBase_Impl::SaveToFile() const {
        std::ofstream file("../media/DataBaseFiles/" + std::string(typeid(T).name()) + ".txt");



        file.close();
    }

    using DataBase = Singleton<DataBase_Impl>;

} // namespace RefactoredProject