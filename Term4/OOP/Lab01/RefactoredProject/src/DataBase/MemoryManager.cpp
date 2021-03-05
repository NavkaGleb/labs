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

    bool MemoryManager::IsExists(std::size_t hash) const {
        return m_Entities.contains(hash);
    }

    bool MemoryManager::IsExists(int id, std::size_t hash) const {
        return IsExists(hash) && m_Entities.at(hash).contains(id);
    }

    void MemoryManager::Delete(std::size_t hash) {
        if (!IsExists(hash))
            return;

        m_Entities[hash].clear();
    }

    void MemoryManager::Delete(int id, std::size_t hash) {
        if (!IsExists(id, hash))
            return;

        m_Entities[hash].erase(id);
    }

} // namespace RefactoredProject