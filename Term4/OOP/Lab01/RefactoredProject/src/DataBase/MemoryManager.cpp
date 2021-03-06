#include "MemoryManager.hpp"

#include <iostream>

namespace RefactoredProject {

    MemoryManager::MemoryManager(IndexTable& indexTable)
        : m_IndexTable(indexTable) {}

    MemoryManager::~MemoryManager() noexcept {
        for (auto& [hash, entities] : m_Entities)
            for (auto& [id, entityData] : entities)
                entityData.Handle.reset();

        std::cout << "MemoryManager dtor" << std::endl;
    }

    bool MemoryManager::IsExists(const TypeInfo& typeInfo) const {
        return m_Entities.contains(typeInfo);
    }

    bool MemoryManager::IsExists(const TypeInfo& typeInfo, int id) const {
        return IsExists(typeInfo) && m_Entities.at(typeInfo).contains(id);
    }

    void MemoryManager::Delete(const TypeInfo& typeInfo) {
        if (!IsExists(typeInfo))
            return;

        m_Entities[typeInfo].clear();
    }

    void MemoryManager::Delete(const TypeInfo& typeInfo, int id) {
        if (!IsExists(typeInfo, id))
            return;

        m_Entities[typeInfo].erase(id);
    }

} // namespace RefactoredProject