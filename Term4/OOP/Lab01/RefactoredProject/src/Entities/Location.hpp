#pragma once

#include <string>

#include "DataBase/DataBase.hpp"

namespace RefactoredProject {

    class Location : public DataBaseEntity {
    public:
        explicit Location(int id = -1);
        ~Location() noexcept override;

        [[nodiscard]] inline const std::string& GetName() const { return m_Name; }
        [[nodiscard]] inline float GetArea() const { return m_Area; }
        [[nodiscard]] inline int GetMonstersCount() const { return m_MonstersCount; }
        [[nodiscard]] inline int GetMonstersMaxCount() const { return m_MonstersMaxCount; }

        void SetName(const std::string& string);
        void SetArea(float area);
        void SetMonstersMaxCount(int monstersMaxCount);

        [[nodiscard]] bool IsFull() const;

        bool IncreaseMonsters(int count = 1);
        void DecreaseMonsters(int count = 1);

        friend std::istream& operator >>(std::istream& istream, Location& location);
        friend std::ostream& operator <<(std::ostream& ostream, const Location& location);

    private:
        std::string m_Name;
        float       m_Area;
        int         m_MonstersCount;
        int         m_MonstersMaxCount;

    }; // class Location

} // namespace RefactoredProject