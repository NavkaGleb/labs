#include "IDataBaseEntity.hpp"

#include <stdexcept>

namespace RefactoredProject {

    IDataBaseEntity::IDataBaseEntity(int id)
        : m_Id(id) {}

    int IDataBaseEntity::GetBytesCount() {
        throw std::runtime_error("RefactoredProject::IDataBaseEntity::GetBytesCount: Not implemented method!");
    }

} // namespace RefactoredProject