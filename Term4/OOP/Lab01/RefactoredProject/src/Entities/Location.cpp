#include "Location.hpp"

#include <iostream>

namespace RefactoredProject {

    Location::Location(int id)
        : m_Id(id)
        , m_Area(0.0f)
        , m_MonstersCount(0)
        , m_MonstersMaxCount(0) {}

    Location::~Location() noexcept {
        std::cout << "Location dtor" << std::endl;
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