#include "FileManager.hpp"

namespace RefactoredProject {

    FileManager::FileManager(IndexTable& indexTable)
        : m_IndexTable(indexTable) {}

    void FileManager::Delete(const TypeInfo& typeInfo) {

    }

    void FileManager::Delete(const TypeInfo& typeInfo, int id) {

    }

    void FileManager::Truncate(const TypeInfo& typeInfo) {
        std::ifstream     infile(GetBinaryDataPath(typeInfo.GetHash()), std::fstream::in | std::fstream::binary);
        std::vector<char> mainBuffer;
        std::vector<char> subBuffer(m_IndexTable.GetBytesCount(typeInfo));
        auto&             data = m_IndexTable.GetData(typeInfo);

        for (auto it = data.begin(); it != data.end(); ++it) {
            if (it->IsDeleted) {
                data.erase(it--);
                continue;
            }

            infile.read(subBuffer.data(), subBuffer.size());
            mainBuffer.insert(mainBuffer.end(), subBuffer.begin(), subBuffer.end());
        }

        infile.close();

        std::ofstream outfile(GetBinaryDataPath(typeInfo.GetHash()), std::fstream::out | std::fstream::binary);

        outfile.write(mainBuffer.data(), mainBuffer.size());
        outfile.close();
    }

} // namespace RefactoredProject