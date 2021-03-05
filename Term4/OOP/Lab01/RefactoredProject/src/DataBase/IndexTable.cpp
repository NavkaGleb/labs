#include "IndexTable.hpp"

namespace RefactoredProject {

    IndexTable::~IndexTable() {
        for (const auto& [hash, entitiesData] : m_Table) {
            std::ofstream outfile(TypeInfo::GetIndexTablePath(hash), std::fstream::out | std::fstream::binary);

            for (const auto& entityData : entitiesData)
                outfile.write(reinterpret_cast<const char*>(&entityData), sizeof(entityData));

            outfile.close();
        }
    }

} // namespace RefactoredProject