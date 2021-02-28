#pragma once

#include <string>

namespace RefactoredProject {

    class Location {
    public:
        explicit Location(int id);
        virtual ~Location() noexcept;

        [[nodiscard]] inline int GetId() const { return m_Id; }
        [[nodiscard]] inline const std::string& GetName() const { return m_Name; }
        [[nodiscard]] inline float GetArea() const { return m_Area; }
        [[nodiscard]] inline int GetMonstersCount() const { return m_MonstersCount; }
        [[nodiscard]] inline int GetMonstersMaxCount() const { return m_MonstersMaxCount; }

        friend std::istream& operator >>(std::istream& istream, Location& location);
        friend std::ostream& operator <<(std::ostream& ostream, const Location& location);

    private:
        int         m_Id;
        std::string m_Name;
        float       m_Area;
        int         m_MonstersCount;
        int         m_MonstersMaxCount;

    }; // class Location

} // namespace RefactoredProject