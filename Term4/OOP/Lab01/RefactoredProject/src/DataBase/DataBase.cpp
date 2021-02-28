#include "DataBase.hpp"

#include <filesystem>

namespace RefactoredProject {

    DataBase_Impl::DataBase_Impl()
        : m_WorkingDirectory(std::filesystem::current_path()) {

        m_WorkingDirectory = m_WorkingDirectory.parent_path();
        m_WorkingDirectory /= "media/DataBaseFiles/";
    }

    DataBase_Impl::~DataBase_Impl() noexcept {
        for (auto& [hash, entities] : m_Entities)
            for (auto& [id, entityData] : entities)
                entityData.Handle.reset();

        std::cout << "DataBase_Impl dtor" << std::endl;
    }

} // namespace RefactoredProject