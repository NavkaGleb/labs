#include "Monster.hpp"

#include <iostream>

namespace RefactoredProject {

    Monster::Monster(int id)
        : IDataBaseEntity(id)
        , m_NameLength(30)
        , m_Name("MonsterDefault")
        , m_Health(0)
        , m_Damage(0)
        , m_Attack(0.0f)
        , m_AttackType(AttackType::None)
        , m_LocationId(-1) {

        m_Name.resize(m_NameLength);
    }

    Monster::~Monster() noexcept {
        std::cout << "Monster dtor" << std::endl;
    }

    int Monster::GetBytesCount() {
        int bytesCount = 0;

        bytesCount += sizeof(int);        // m_Id;
        bytesCount += sizeof(int);        // m_NameLength
        bytesCount += 30;                 // m_Name
        bytesCount += sizeof(int);        // m_Health
        bytesCount += sizeof(int);        // m_Damage
        bytesCount += sizeof(float);      // m_Attack
        bytesCount += sizeof(AttackType); // m_AttackType
        bytesCount += sizeof(int);        // m_LocationId

        return bytesCount;
    }

    void Monster::SetProps(MonsterProps&& props) {
        m_Name       = std::exchange(props.Name, {});
        m_Health     = std::exchange(props.Health, 0);
        m_Damage     = std::exchange(props.Damage, 0);
        m_Attack     = std::exchange(props.Damage, 0.0f);
        m_AttackType = std::exchange(props.AttackType, AttackType::None);
        m_LocationId = std::exchange(props.LocationId, -1);

        m_Name.resize(m_NameLength);
    }

    void Monster::SetName(const std::string& name) {
        m_Name = name;
        m_Name.resize(m_NameLength);
    }

    void Monster::SetName(std::string&& name) {
        m_Name = name;
        m_Name.resize(m_NameLength);
    }

    void Monster::SetHealth(int health) {
        m_Health = health;
    }

    void Monster::SetDamage(int damage) {
        m_Damage = damage;
    }

    void Monster::SetAttack(float attack) {
        m_Attack = attack;
    }

    void Monster::SetAttackType(const AttackType& attackType) {
        m_AttackType = attackType;
    }

    void Monster::SetLocationId(int locationId) {
        m_LocationId = locationId;
    }

    void Monster::ReadFromBinary(std::ifstream& infile) {
        infile.read((char*)&m_Id, sizeof(m_Id));
        infile.read((char*)&m_NameLength, sizeof(m_NameLength));
        infile.read((char*)m_Name.c_str(), m_NameLength);
        infile.read((char*)&m_Health, sizeof(m_Health));
        infile.read((char*)&m_Damage, sizeof(m_Damage));
        infile.read((char*)&m_Attack, sizeof(m_Attack));
        infile.read((char*)&m_AttackType, sizeof(m_AttackType));
        infile.read((char*)&m_LocationId, sizeof(m_LocationId));
    }

    void Monster::WriteToBinary(std::ofstream& outfile) const {
        outfile.write((const char*)&m_Id, sizeof(m_Id));
        outfile.write((const char*)&m_NameLength, sizeof(m_NameLength));
        outfile.write((const char*)m_Name.c_str(), m_NameLength);
        outfile.write((const char*)&m_Health, sizeof(m_Health));
        outfile.write((const char*)&m_Damage, sizeof(m_Damage));
        outfile.write((const char*)&m_Attack, sizeof(m_Attack));
        outfile.write((const char*)&m_AttackType, sizeof(m_AttackType));
        outfile.write((const char*)&m_LocationId, sizeof(m_LocationId));
    }

    std::istream& operator >>(std::istream& istream, Monster& monster) {
        istream >> monster.m_Id;
        istream >> monster.m_Name;
        istream >> monster.m_Health;
        istream >> monster.m_Damage;
        istream >> monster.m_Attack;
        istream >> reinterpret_cast<int&>(monster.m_AttackType);
        istream >> monster.m_LocationId;

        return istream;
    }

    std::ostream& operator <<(std::ostream& ostream, const Monster& monster) {
        ostream << monster.m_Id << " ";
        ostream << monster.m_Name << " ";
        ostream << monster.m_Health << " ";
        ostream << monster.m_Damage << " ";
        ostream << monster.m_Attack << " ";
        ostream << static_cast<int>(monster.m_AttackType) << " ";
        ostream << monster.m_LocationId;

        return ostream;
    }

    std::ostream& operator <<(std::ostream& ostream, const Monster::AttackType& attackType) {
        switch (attackType) {
            case Monster::AttackType::IncreaseDamage: ostream << "IncreaseDamage"; break;
            case Monster::AttackType::RepeatAttack:   ostream << "RepeatAttack";   break;
            case Monster::AttackType::SelfCure:       ostream << "SelfCure";       break;
            case Monster::AttackType::EnemyParalyze:  ostream << "EnemyParalyze";  break;
            default:                                  ostream << "None";           break;
        }

        return ostream;
    }

} // namespace RefactoredProject