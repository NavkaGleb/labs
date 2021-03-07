#pragma once

#include "IndexTable.hpp"
#include "RelationTable.hpp"
#include "MemoryManager.hpp"
#include "FileManager.hpp"
#include "Utility/Singleton.hpp"

namespace RefactoredProject {

    class DataBase_Impl {
    public:
        template <Entity T> void Init();

        template <Entity T, Entity U>
        void CreateRelation();

        template <Entity T, Entity U>
        void DeleteRelation();

        template <Entity T> T& Create();
        template <Entity T, Entity U> std::pair<T, U&> Create(int tId);

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

        template <Entity T> void Update(const T& entity);

        template <Entity T> std::vector<std::shared_ptr<T>> SearchInMemory(SearchFunc<T> predicate) const;
        template <Entity T> std::vector<std::shared_ptr<T>> SearchInFile(SearchFunc<T> predicate) const;

        void PrintRelationTable() const;
        void PrintIndexTable() const;

        friend class Singleton<DataBase_Impl>;
        friend class MemoryManager;
        friend class FileManager;

    private:
        DataBase_Impl();
        virtual ~DataBase_Impl() noexcept = default;

    private:
        IndexTable    m_IndexTable;
        RelationTable m_RelationTable;
        MemoryManager m_MemoryManager;
        FileManager   m_FileManager;

    }; // class DataBase_Impl

    using DataBase = Singleton<DataBase_Impl>;

    template <Entity T>
    void DataBase_Impl::Init() {
        return m_IndexTable.Init<T>();
    }

    template <Entity T, Entity U>
    void DataBase_Impl::CreateRelation() {
        return m_RelationTable.CreateRelation<T, U>();
    }

    template <Entity T, Entity U>
    void DataBase_Impl::DeleteRelation() {
        return m_RelationTable.DeleteRelation<T, U>();
    }

    template <Entity T>
    T& DataBase_Impl::Create() {
        if (m_RelationTable.IsMinor<T>()) {
            throw std::invalid_argument(
                "RefactoredProject::DataBase_Impl::Create: Given type is depended from another!"
            );
        }

        return m_MemoryManager.Create<T>(m_IndexTable.GetNewId<T>());
    }

    template <Entity T, Entity U>
    std::pair<T, U&> DataBase_Impl::Create(int tId) {
        if (!m_RelationTable.IsExists<T, U>()) {
            throw std::invalid_argument(
                "RefactoredProject::DataBase_Impl::Create: No relation from T to U!"
            );
        }

        if (!m_IndexTable.IsExists<T>(tId)) {
            throw std::invalid_argument(
                "RefactoredProject::DataBase_Impl::Create: No entity by id = " + std::to_string(tId)
            );
        }

        U& u = m_MemoryManager.Create<U>(m_IndexTable.GetNewId<U>());

        m_RelationTable.CreateConnection<T, U>(tId, u.GetId());

        return { GetFromFile<T>(tId), u };
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
        return m_MemoryManager.Save<T>();
    }

    template <Entity T>
    void DataBase_Impl::DeleteFromMemory(int id) {
//        if (m_MemoryManager.IsInFile<T>(id))
//            return m_MemoryManager.Delete<T>(id);
//
//        for (auto& [relation, connections] : m_RelationTable) {
//            if (relation.first == TypeInfo::GetHash<T>()) {
//                for (auto& [mainId, dependedIds] : connections) {
//                    for (const auto& dependedId : dependedIds) {
////                        auto hash = relation.second;
////                        m_MemoryManager.Delete<hash>(dependedId);
//                    }
//                }
//
//                m_MemoryManager.Delete<T>(id);
//                connections.clear();
//            }
//
//            if (relation.second == TypeInfo::GetHash<T>()) {
//                m_MemoryManager.Delete<T>(id);
//            }
//        }
    }

    template <Entity T>
    void DataBase_Impl::DeleteFromFile(int id) {
        if (m_RelationTable.IsMajor<T>()) {
            m_FileManager.Delete<T>(id);

            auto toDelete = m_RelationTable.GetMinorIds<T>(id);

            for (const auto& [typeInfo, minorIds] : toDelete) {
                m_RelationTable.DeleteMajor({ TypeInfo::Get<T>(), typeInfo }, id);

                for (const auto& minorId : minorIds) {
                    m_FileManager.Delete(typeInfo, minorId);
                    m_MemoryManager.Delete(typeInfo, minorId);
                }
            }
        }
    }

    template <Entity T>
    void DataBase_Impl::DeleteFromMemory() {

    }

    template <Entity T>
    void DataBase_Impl::DeleteFromFile() {

    }

    template <Entity T>
    void DataBase_Impl::Update(const T& entity) {
        if (m_MemoryManager.IsExists<T>(entity.GetId()))
            m_MemoryManager.Update(entity);

        auto position = m_IndexTable.GetPosition<T>(entity.GetId());

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