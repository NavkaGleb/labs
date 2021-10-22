#include "Location.hpp"

#include <iostream>
#include <fstream>

namespace RefactoredProject {

    Location::Location(int id)
        : IDataBaseEntity(id)
        , m_NameLength(30)
        , m_Name("DefaultLocation")
        , m_Area(0.0f)
        , m_MonstersCount(0)
        , m_MonstersMaxCount(0) {

        m_Name.resize(m_NameLength);
    }

    Location::~Location() noexcept {
        std::cout << "Location dtor" << std::endl;
    }

    int Location::GetBytesCount() {
        int bytesCount = 0;

        bytesCount += sizeof(int);   // m_Id;
        bytesCount += sizeof(int);   // m_NameLength
        bytesCount += 30;            // m_Name
        bytesCount += sizeof(float); // m_Area
        bytesCount += sizeof(int);   // m_MonstersCount
        bytesCount += sizeof(int);   // m_MonstersMaxCount

        return bytesCount;
    }

    void Location::SetProps(LocationProps&& props) {
        m_Name             = std::exchange(props.Name, std::string());
        m_Area             = std::exchange(props.Area, 0.0f);
        m_MonstersMaxCount = std::exchange(props.MonstersMaxCount, 0);

        m_Name.resize(m_NameLength);
    }

    void Location::SetName(const std::string& name) {
        m_Name = name;
        m_Name.resize(m_NameLength);
    }

    void Location::SetName(std::string&& name) {
        m_Name = std::move(name);
        m_Name.resize(m_NameLength);
    }

    void Location::SetArea(float area) {
        m_Area = area;
    }

    void Location::SetMonstersMaxCount(int monstersMaxCount) {
        m_MonstersMaxCount = monstersMaxCount;
    }

    bool Location::IsFull() const {
        return m_MonstersCount == m_MonstersMaxCount;
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

    void Location::ReadFromBinary(std::ifstream& infile) {
        infile.read((char*)&m_Id, sizeof(m_Id));
        infile.read((char*)&m_NameLength, sizeof(m_NameLength));
        infile.read((char*)m_Name.c_str(), m_NameLength);
        infile.read((char*)&m_Area, sizeof(m_Area));
        infile.read((char*)&m_MonstersCount, sizeof(m_MonstersCount));
        infile.read((char*)&m_MonstersMaxCount, sizeof(m_MonstersMaxCount));
    }

    void Location::WriteToBinary(std::ofstream& outfile) const {
        outfile.write((char*)&m_Id, sizeof(m_Id));
        outfile.write((char*)&m_NameLength, sizeof(m_NameLength));
        outfile.write((char*)m_Name.c_str(), m_NameLength);
        outfile.write((char*)&m_Area, sizeof(m_Area));
        outfile.write((char*)&m_MonstersCount, sizeof(m_MonstersCount));
        outfile.write((char*)&m_MonstersMaxCount, sizeof(m_MonstersMaxCount));
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