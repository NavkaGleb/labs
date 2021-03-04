#include "MemoryManager.hpp"

#include <iostream>

namespace RefactoredProject {

    MemoryManager::MemoryManager(std::unordered_map<std::size_t, std::vector<std::pair<int, uintmax_t>>>& indexTable)
        : m_IndexTable(indexTable) {}

    MemoryManager::~MemoryManager() noexcept {
        for (auto& [hash, entities] : m_Entities)
            for (auto& [id, entityData] : entities)
                entityData.Handle.reset();

        std::cout << "MemoryManager dtor" << std::endl;
    }

} // namespace RefactoredProject