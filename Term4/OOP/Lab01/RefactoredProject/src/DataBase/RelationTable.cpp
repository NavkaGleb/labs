#include "RelationTable.hpp"

namespace RefactoredProject {

    RelationTable::~RelationTable() noexcept {
        for (const auto& [relation, connections] : m_Table) {
            std::ofstream outfile(
                GetRelationPath(relation.first.GetHash(), relation.second.GetHash()),
                std::fstream::out | std::fstream::binary
            );

            for (const auto& [majorId, minorIds] : connections) {
                auto minorIdsCount = minorIds.size();

                outfile.write(reinterpret_cast<const char*>(&majorId), sizeof(majorId));
                outfile.write(reinterpret_cast<const char*>(&minorIdsCount), sizeof(minorIdsCount));

                for (const auto& minorId : minorIds)
                    outfile.write(reinterpret_cast<const char*>(&minorId), sizeof(minorId));
            }

            outfile.close();
        }
    }

    void RelationTable::Print() const {
        for (const auto& [relation, connections] : m_Table) {
            std::cout << "Relation: " << relation.first.GetHash() << " -> " << relation.second.GetHash() << std::endl;

            for (const auto& [majorId, minorIds] : connections) {
                std::cout << majorId << ": ";

                for (const auto& minorId : minorIds)
                    std::cout << minorId << " ";
                std::cout << std::endl;
            }

            std::cout << std::endl;
        }
    }

} // namespace RefactoredProject