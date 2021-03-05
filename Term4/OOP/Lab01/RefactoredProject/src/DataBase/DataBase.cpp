#include "DataBase.hpp"

#include <filesystem>

namespace RefactoredProject {

    DataBase_Impl::DataBase_Impl()
        : m_MemoryManager(m_IndexTable)
        , m_WorkingDirectory(std::filesystem::current_path()) {

        m_WorkingDirectory = m_WorkingDirectory.parent_path();
        m_WorkingDirectory /= "media/DataBaseFiles/";
    }

} // namespace RefactoredProject