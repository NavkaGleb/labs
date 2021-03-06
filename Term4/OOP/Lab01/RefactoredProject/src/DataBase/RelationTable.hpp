#pragma once

#include <map>
#include <vector>
#include <algorithm>
#include <filesystem>

#include "TypeInfo.hpp"
#include "IDataBaseEntity.hpp"

namespace RefactoredProject {

    class RelationTable {
    public:
        using Relation      = std::pair<TypeInfo, TypeInfo>;
        using Connection    = std::map<int, std::vector<int>>;
        using Table         = std::map<Relation, Connection>;
        using Iterator      = Table::iterator;
        using ConstIterator = Table::const_iterator;

        RelationTable() = default;
        virtual ~RelationTable() noexcept;

        template <Entity T, Entity U> void Init();

        template <Entity T, Entity U> [[nodiscard]] bool IsExists() const;
        template <Entity T> [[nodiscard]] bool IsMajor() const;
        template <Entity T> [[nodiscard]] bool IsMinor() const;

        template <Entity T, Entity U> void CreateRelation();
        template <Entity T, Entity U> void DeleteRelation();

        template <Entity T, Entity U> void CreateConnection(int tId, int uId);
        template <Entity T, Entity U> void DeleteConnection(int tId, int UId);

        template <Entity T> std::map<TypeInfo, std::vector<int>> GetMinorIds(int id);

        [[nodiscard]] inline Iterator begin() noexcept { return m_Table.begin(); }
        [[nodiscard]] inline Iterator end() noexcept { return m_Table.end(); }

        [[nodiscard]] inline ConstIterator begin() const noexcept { return m_Table.begin(); }
        [[nodiscard]] inline ConstIterator end() const noexcept { return m_Table.end(); }

        void Print() const;

    private:
       Table m_Table;

    }; // class RelationTable

    template <Entity T, Entity U>
    void RelationTable::Init() {
        Relation relation = { TypeInfo::Get<T>(), TypeInfo::Get<U>() };

        std::ifstream infile(
            GetRelationPath(relation.first.GetHash(), relation.second.GetHash()),
            std::fstream::out | std::fstream::binary
        );

        int         majorId       = 0;
        std::size_t minorIdsCount = 0;

        infile.read(reinterpret_cast<char*>(&majorId), sizeof(majorId));
        infile.read(reinterpret_cast<char*>(&minorIdsCount), sizeof(minorIdsCount));

        auto& minorIds = m_Table[relation][majorId];

        minorIds.resize(minorIdsCount);

        for (int i = 0; i < minorIdsCount; ++i)
            infile.read(reinterpret_cast<char*>(&minorIds[i]), sizeof(minorIds[i]));

        infile.close();
    }

    template <Entity T, Entity U>
    bool RelationTable::IsExists() const {
        return m_Table.contains({ TypeInfo::Get<T>(), TypeInfo::Get<U>() });
    }

    template <Entity T>
    bool RelationTable::IsMajor() const {
        return std::ranges::any_of(m_Table.begin(), m_Table.end(), [](const auto& pair) {
            return pair.first.first == TypeInfo::Get<T>();
        });
    }

    template <Entity T>
    bool RelationTable::IsMinor() const {
        return std::ranges::any_of(m_Table.begin(), m_Table.end(), [](const auto& pair) {
            return pair.first.second == TypeInfo::Get<T>();
        });
    }

    template <Entity T, Entity U>
    void RelationTable::CreateRelation() {
        if (std::filesystem::exists(GetRelationPath(TypeInfo::Get<T>().GetHash(), TypeInfo::Get<U>().GetHash())))
            return Init<T, U>();

        m_Table[{ TypeInfo::Get<T>(), TypeInfo::Get<U>() }];
    }

    template <Entity T, Entity U>
    void RelationTable::DeleteRelation() {
        m_Table.erase({ TypeInfo::Get<T>(), TypeInfo::Get<U>() });
    }

    template <Entity T, Entity U>
    void RelationTable::CreateConnection(int tId, int uId) {
        m_Table[{ TypeInfo::Get<T>(), TypeInfo::Get<U>() }][tId].emplace_back(uId);
    }

    template <Entity T, Entity U>
    void RelationTable::DeleteConnection(int tId, int uId) {
        auto& connections = m_Table[{ TypeInfo::Get<T>(), TypeInfo::Get<U>() }][tId];

        connections.erase(std::find(connections.begin(), connections.end(), uId));
    }

    template <Entity T>
    std::map<TypeInfo, std::vector<int>> RelationTable::GetMinorIds(int id) {
        std::map<TypeInfo, std::vector<int>> result;

        for (const auto& [relation, connections] : m_Table) {
            if (relation.first == TypeInfo::Get<T>()) {
                auto& resultIds = result[relation.second];

                resultIds.insert(resultIds.end(), connections.at(id).begin(), connections.at(id).end());
            }
        }

        return result;
    }

} // namespace RefactoredProject