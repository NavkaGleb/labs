#pragma once

#include <map>
#include <vector>

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

        [[nodiscard]] inline Iterator begin() noexcept { return m_Table.begin(); }
        [[nodiscard]] inline Iterator end() noexcept { return m_Table.end(); }

        [[nodiscard]] inline ConstIterator begin() const noexcept { return m_Table.begin(); }
        [[nodiscard]] inline ConstIterator end() const noexcept { return m_Table.end(); }

    private:
        std::map<Relation, std::map<int, std::vector<int>>> m_Table;

    }; // class RelationTable

} // namespace RefactoredProject