#include "DataBase.hpp"

#include <filesystem>

namespace RefactoredProject {

    DataBase_Impl::DataBase_Impl()
        : m_MemoryManager(m_IndexTable)
        , m_WorkingDirectory(std::filesystem::current_path()) {

        m_WorkingDirectory = m_WorkingDirectory.parent_path();
        m_WorkingDirectory /= "media/DataBaseFiles/";
    }

    DataBase_Impl::~DataBase_Impl() noexcept {
        for (const auto& [hash, entityPositions] : m_IndexTable) {
            std::ofstream outfile(TypeInfo::GetIndexTablePath(hash), std::fstream::out | std::fstream::binary);

            for (const auto& entityPosition : entityPositions)
                outfile.write(reinterpret_cast<const char*>(&entityPosition), sizeof(entityPosition));

            outfile.close();
        }
    }

} // namespace RefactoredProject