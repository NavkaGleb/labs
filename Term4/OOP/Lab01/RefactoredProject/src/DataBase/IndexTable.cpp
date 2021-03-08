#include "IndexTable.hpp"

namespace RefactoredProject {

    IndexTable::~IndexTable() {
        for (const auto& [typeInfo, entitiesData] : m_Table) {
            std::ofstream outfile(GetIndexTablePath(typeInfo.GetHash()), std::fstream::out | std::fstream::binary);

            for (const auto& entityData : entitiesData)
                outfile.write(reinterpret_cast<const char*>(&entityData), sizeof(entityData));

            outfile.close();
        }
    }

    bool IndexTable::IsExists(const TypeInfo& typeInfo) const {
        return m_Table.contains(typeInfo);
    }

    bool IndexTable::IsExists(const TypeInfo& typeInfo, int id) const {
        return GetPosition(typeInfo, id).has_value();
    }

    int IndexTable::GetNewId(const TypeInfo& typeInfo) {
        return ++m_Ids[typeInfo];
    }


    std::optional<uintmax_t> IndexTable::GetPosition(const TypeInfo& typeInfo, int id) const {
        auto* entityData = GetEntityData(typeInfo, id);

        return !entityData || entityData->IsDeleted ? std::nullopt : std::optional(entityData->Position);
    }

    void IndexTable::Print() const {
        std::cout << "Size: " << m_Table.size() << std::endl;
        for (const auto& [hash, pairs] : m_Table) {
            std::cout << "Hash: " << hash.GetHash() << std::endl;
            std::cout << "Pairs: " << pairs.size() << std::endl;
            for (const auto& pair : pairs)
                std::cout << pair.Id << " -> " << pair.Position << ": " << pair.IsDeleted << std::endl;

            std::cout << std::endl;
        }
    }

    IndexTable::EntityData* IndexTable::GetEntityData(const TypeInfo& typeInfo, int id) const {
        return static_cast<EntityData*>(
            std::bsearch(
                &id,
                m_Table.at(typeInfo).data(),
                m_Table.at(typeInfo).size(),
                sizeof(EntityData),
                [](const void* lhs, const void* rhs) {
                    const auto& left  = *static_cast<const EntityData*>(lhs);
                    const auto& right = *static_cast<const EntityData*>(rhs);

                    if (left.Id < right.Id)
                        return -1;

                    if (left.Id > right.Id)
                        return 1;

                    return 0;
                }
            )
        );
    }

} // namespace RefactoredProject