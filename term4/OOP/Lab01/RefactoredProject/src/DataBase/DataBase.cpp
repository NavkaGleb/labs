#include "DataBase.hpp"

namespace RefactoredProject {

    DataBase_Impl::DataBase_Impl()
        : m_MemoryManager(m_IndexTable)
        , m_FileManager(m_IndexTable) {}

    void DataBase_Impl::PrintRelationTable() const {
        return m_RelationTable.Print();
    }

    void DataBase_Impl::PrintIndexTable() const {
        return m_IndexTable.Print();
    }

} // namespace RefactoredProject