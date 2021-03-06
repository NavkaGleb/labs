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
        return 0;
    }

    void Monster::SetName(const std::string& name) {
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

} // namespace RefactoredProject