#include "Location.hpp"

#include <iostream>

namespace RefactoredProject {

    Location::Location(int id)
        : m_Id(id)
        , m_Name("LocationDefault")
        , m_Area(0.0f)
        , m_MonstersCount(0)
        , m_MonstersMaxCount(0) {}

    Location::~Location() noexcept {
        std::cout << "Location dtor" << std::endl;
    }

    void Location::SetName(const std::string& name) {
        m_Name = name;
    }

    void Location::SetArea(float area) {
        m_Area = area;
    }

    void Location::SetMonstersMaxCount(int monstersMaxCount) {
        m_MonstersMaxCount = monstersMaxCount;
    }

    bool Location::IncreaseMonsters(int count) {
        if (m_MonstersCount + count > m_MonstersMaxCount)
            return false;

        m_MonstersCount += count;

        return true;
    }

    void Location::DecreaseMonsters(int count) {
        if (m_MonstersCount - count < 0)
            return;

        m_MonstersCount -= count;
    }

    std::istream& operator >>(std::istream& istream, Location& location) {
        istream >> location.m_Id;
        istream >> location.m_Name;
        istream >> location.m_Area;
        istream >> location.m_MonstersCount;
        istream >> location.m_MonstersMaxCount;

        return istream;
    }

    std::ostream& operator <<(std::ostream& ostream, const Location& location) {
        ostream << location.m_Id << " ";
        ostream << location.m_Name << " ";
        ostream << location.m_Area << " ";
        ostream << location.m_MonstersCount << " ";
        ostream << location.m_MonstersMaxCount;

        return ostream;
    }

} // namespace RefactoredProject