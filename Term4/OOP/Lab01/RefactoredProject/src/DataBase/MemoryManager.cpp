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

} // namespace RefactoredProject