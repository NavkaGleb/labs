#include "DataBase.hpp"

namespace RefactoredProject {

    DataBase_Impl::DataBase_Impl()
        : m_MemoryManager(m_IndexTable) {}

} // namespace RefactoredProject