#pragma once

#include <string>
#include <typeinfo>

namespace RefactoredProject::TypeInfo {

    template <typename T>
    inline std::size_t GetHash() {
        return typeid(T).hash_code();
    }

    inline std::string GetIndexTablePath(std::size_t hash) {
        return "../media/DataBaseFiles/" + std::to_string(hash) + "IndexTable.bin";
    }

    inline std::string GetBinaryDataPath(std::size_t hash) {
        return "../media/DataBaseFiles/" + std::to_string(hash) + "Data.bin";
    }

    inline std::string GetTextDataPath(std::size_t hash) {
        return "../media/DataBaseFiles/" + std::to_string(hash) + "Data.txt";
    }

} // namespace RefactoredProject::TypeInfo