#include "TypeInfo.hpp"

namespace RefactoredProject {

    bool TypeInfo::operator <(const TypeInfo& other) const {
        return m_Handle.get().hash_code() < other.m_Handle.get().hash_code();
    }

    bool TypeInfo::operator ==(const TypeInfo& other) const {
        return m_Handle.get() == other.m_Handle.get();
    }

    TypeInfo::TypeInfo(Handle&& handle)
        : m_Handle(handle) {}

} // namespace RefactoredProject