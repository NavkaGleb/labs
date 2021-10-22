#pragma once

#include <string>
#include <typeinfo>
#include <functional>
#include <iostream>

namespace RefactoredProject {

    class TypeInfo {
    public:
        using Handle = std::reference_wrapper<const std::type_info>;

        struct Hasher {
            std::size_t operator() (const TypeInfo& typeInfo) {
                return typeInfo.GetHash();
            }
        };

        template <typename T>
        static TypeInfo Get();

        [[nodiscard]] inline std::size_t GetHash() const { return m_Handle.get().hash_code(); }

        bool operator <(const TypeInfo& other) const;
        bool operator ==(const TypeInfo& other) const;

    private:
        explicit TypeInfo(Handle&& handle);

    private:
        Handle m_Handle;

    }; // class TypeInfo

    template <typename T>
    TypeInfo TypeInfo::Get() {
        static TypeInfo typeInfo(typeid(T));
        return typeInfo;
    }

    template <typename T>
    inline std::string GetIndexTablePath() {
        return "../media/DataBaseFiles/" + std::to_string(TypeInfo::Get<T>().GetHash()) + "IndexTable.bin";
    }

    inline std::string GetIndexTablePath(std::size_t hash) {
        return "../media/DataBaseFiles/" + std::to_string(hash) + "IndexTable.bin";
    }

    template <typename T>
    inline std::string GetBinaryDataPath() {
        return "../media/DataBaseFiles/" + std::to_string(TypeInfo::Get<T>().GetHash()) + "Data.bin";
    }

    inline std::string GetBinaryDataPath(std::size_t hash) {
        return "../media/DataBaseFiles/" + std::to_string(hash) + "Data.bin";
    }

    template <typename T>
    inline std::string GetTextDataPath() {
        return "../media/DataBaseFiles/" + std::to_string(TypeInfo::Get<T>().GetHash()) + "Data.txt";
    }

    inline std::string GetTextDataPath(std::size_t hash) {
        return "../media/DataBaseFiles/" + std::to_string(hash) + "Data.txt";
    }

    inline std::string GetRelationPath(std::size_t hash1, std::size_t hash2) {
        return "../media/DataBaseFiles/" + std::to_string(hash1) + "_" + std::to_string(hash2) + "Relation.bin";
    }

} // namespace RefactoredProject::TypeInfo
