#pragma once

#include <map>
#include <vector>

#include "TypeInfo.hpp"
#include "IDataBaseEntity.hpp"

namespace RefactoredProject {

    class RelationTable {
    public:
        using Relation      = std::pair<std::size_t, std::size_t>;
        using Accordance    = std::map<int, std::vector<int>>;
        using Table         = std::map<Relation, Accordance>;
        using Iterator      = Table::iterator;
        using ConstIterator = Table::const_iterator;

        RelationTable() = default;
        virtual ~RelationTable() = default;

        template <Entity T, Entity U>
        [[nodiscard]] bool IsExists() const;

        template <Entity T, Entity U> void CreateRelation();
        template <Entity T, Entity U> void DeleteRelation();

        template <Entity T, Entity U> void AddAccordance(int tId, int uId);

        [[nodiscard]] inline Iterator begin() noexcept { return m_Table.begin(); }
        [[nodiscard]] inline Iterator end() noexcept { return m_Table.end(); }

        [[nodiscard]] inline ConstIterator begin() const noexcept { return m_Table.begin(); }
        [[nodiscard]] inline ConstIterator end() const noexcept { return m_Table.end(); }

    private:
        std::map<Relation, std::map<int, std::vector<int>>> m_Table;

    }; // class RelationTable

    template <Entity T, Entity U>
    bool RelationTable::IsExists() const {
        return m_Table.contains(TypeInfo::GetHash<T>(), TypeInfo::GetHash<U>());
    }

    template <Entity T, Entity U>
    void RelationTable::CreateRelation() {
        m_Table[{ TypeInfo::GetHash<T>(), TypeInfo::GetHash<U>() }];
    }

    template <Entity T, Entity U>
    void RelationTable::DeleteRelation() {
        m_Table.erase({ TypeInfo::GetHash<T>(), TypeInfo::GetHash<U>() });
    }

    template <Entity T, Entity U>
    void RelationTable::AddAccordance(int tId, int uId) {
        m_Table[{ TypeInfo::GetHash<T>(), TypeInfo::GetHash<U>() }][tId].emplace_back(uId);
    }

} // namespace RefactoredProject