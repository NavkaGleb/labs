#pragma once

#include <functional>
#include <fstream>

namespace RefactoredProject {

    class IDataBaseEntity {
    public:
        explicit IDataBaseEntity(int id);
        virtual ~IDataBaseEntity() noexcept = default;

        static int GetBytesCount();

        [[nodiscard]] inline int GetId() const { return m_Id; }

        virtual void ReadFromBinary(std::ifstream& infile) = 0;
        virtual void WriteToBinary(std::ofstream& outfile) const = 0;

    protected:
        int m_Id;

    }; // class IDataBaseEntity

    template<typename T>
    concept Entity = std::is_base_of<IDataBaseEntity, T>::value;

    template <Entity T>
    using SearchFunc = std::function<bool(const T&)>;

} // namespace RefactoredProject
