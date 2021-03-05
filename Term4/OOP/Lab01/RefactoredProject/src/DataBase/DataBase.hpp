#pragma once

#include "IndexTable.hpp"
#include "MemoryManager.hpp"
#include "FileManager.hpp"
#include "Utility/Singleton.hpp"

namespace RefactoredProject {

    class DataBase_Impl {
    public:
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

        template <Entity T, Entity U> void SetOneToMany();

        template <Entity T> void Update(const T& entity);

        template <Entity T> std::vector<std::shared_ptr<T>> SearchInMemory(SearchFunc<T> predicate) const;
        template <Entity T> std::vector<std::shared_ptr<T>> SearchInFile(SearchFunc<T> predicate) const;

        friend class Singleton<DataBase_Impl>;
        friend class MemoryManager;
        friend class FileManager;

    private:
        using RelationTable  = std::map<std::pair<std::size_t, std::size_t>, std::map<int, std::vector<int>>>;

        DataBase_Impl();
        virtual ~DataBase_Impl() noexcept = default;

    private:
        IndexTable    m_IndexTable;
        RelationTable m_RelationTable;

        MemoryManager m_MemoryManager;
        FileManager   m_FileManager;

        std::filesystem::path m_WorkingDirectory;

    }; // class DataBase_Impl

    using DataBase = Singleton<DataBase_Impl>;

    template <Entity T>
    void DataBase_Impl::Init() {
        m_IndexTable.Init<T>();
        m_IndexTable.Print();
    }

    template <Entity T>
    T& DataBase_Impl::Create() {
        return m_MemoryManager.Create<T>(m_IndexTable.GetNewId<T>());
    }

    template <Entity T>
    T& DataBase_Impl::GetFromMemory(int id) {
        return m_MemoryManager.Get<T>(id);
    }

    template <Entity T>
    T DataBase_Impl::GetFromFile(int id) const {
        if (!m_IndexTable.IsExists<T>()) {
            throw std::invalid_argument(
                "RefactoredProject::DataBase_Impl::GetFromFile: No such type in m_IndexTable!"
            );
        }

        m_IndexTable.Print();

        auto entityPosition = m_IndexTable.GetPosition<T>(id);

        if (!entityPosition.has_value()) {
            throw std::range_error(
                "RefactoredProject::DataBase_Impl::GetFromFile: No record by given id!"
            );
        }

        std::cout << "Entity pos int file!!" << *entityPosition << std::endl;

        return m_FileManager.Get<T>(*entityPosition);
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
        m_MemoryManager.Save<T>();
        m_IndexTable.Print();
    }

    template <Entity T>
    void DataBase_Impl::DeleteFromMemory(int id) {
        if (m_MemoryManager.IsInFile<T>(id))
            return m_MemoryManager.Delete<T>(id);

        for (auto& [relation, connections] : m_RelationTable) {
            if (relation.first == TypeInfo::GetHash<T>()) {
                for (auto& [mainId, dependedIds] : connections)
                    for (const auto& dependedId : dependedIds)
                        m_MemoryManager.Delete(dependedId, relation.second);

                m_MemoryManager.Delete<T>(id);
                connections.clear();
            }

            if (relation.second == TypeInfo::GetHash<T>()) {
                m_MemoryManager.Delete<T>(id);
            }
        }
    }

    template <Entity T>
    void DataBase_Impl::DeleteFromFile(int id) {

    }

    template <Entity T>
    void DataBase_Impl::DeleteFromMemory() {

    }

    template <Entity T>
    void DataBase_Impl::DeleteFromFile() {

    }

    template <Entity T, Entity U>
    void DataBase_Impl::SetOneToMany() {
        m_RelationTable[{ TypeInfo::GetHash<T>(), TypeInfo::GetHash<U>() }];
    }

    template <Entity T>
    void DataBase_Impl::Update(const T& entity) {
        if (m_MemoryManager.IsExists<T>(entity.GetId()))
            m_MemoryManager.Update(entity);

        auto position = m_IndexTable.GetPosition<T>(entity.GetId());

        std::cout << "position" << *position << std::endl;

        if (position.has_value())
            m_FileManager.Update(entity, *position);
    }

    template <Entity T>
    std::vector<std::shared_ptr<T>> DataBase_Impl::SearchInMemory(SearchFunc<T> predicate) const {
        return m_MemoryManager.Search<T>(predicate);
    }

    template <Entity T>
    std::vector<std::shared_ptr<T>> DataBase_Impl::SearchInFile(SearchFunc<T> predicate) const {
        return m_FileManager.Search<T>(predicate);
    }

} // namespace RefactoredProject