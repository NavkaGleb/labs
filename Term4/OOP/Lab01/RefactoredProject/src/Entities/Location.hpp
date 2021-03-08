#pragma once

#include <string>

#include "DataBase/IDataBaseEntity.hpp"

namespace RefactoredProject {

    struct LocationProps {
        std::string Name;
        float       Area             = 0.0f;
        int         MonstersMaxCount = 0;

    }; // struct LocationProps

    class Location : public IDataBaseEntity {
    public:
        explicit Location(int id = -1);
        ~Location() noexcept override;

        static int GetBytesCount();

        [[nodiscard]] inline const std::string& GetName() const { return m_Name; }
        [[nodiscard]] inline float GetArea() const { return m_Area; }
        [[nodiscard]] inline int GetMonstersCount() const { return m_MonstersCount; }
        [[nodiscard]] inline int GetMonstersMaxCount() const { return m_MonstersMaxCount; }

        void SetProps(LocationProps&& props);
        void SetName(const std::string& name);
        void SetName(std::string&& name);
        void SetArea(float area);
        void SetMonstersMaxCount(int monstersMaxCount);

        [[nodiscard]] bool IsFull() const;

        bool IncreaseMonsters(int count = 1);
        void DecreaseMonsters(int count = 1);

        void ReadFromBinary(std::ifstream& infile) override;
        void WriteToBinary(std::ofstream& outfile) const override;

        friend std::istream& operator >>(std::istream& istream, Location& location);
        friend std::ostream& operator <<(std::ostream& ostream, const Location& location);

    private:
        const int   m_NameLength;
        std::string m_Name;
        float       m_Area;
        int         m_MonstersCount;
        int         m_MonstersMaxCount;

    }; // class Location

} // namespace RefactoredProject