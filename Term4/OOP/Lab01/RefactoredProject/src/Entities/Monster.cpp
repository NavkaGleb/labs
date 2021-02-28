#include "Monster.hpp"

#include <iostream>

namespace RefactoredProject {

    Monster::Monster(int id)
        : m_Id(id)
        , m_Health(0)
        , m_Damage(0)
        , m_Attack(0.0f)
        , m_AttackType(AttackType::None)
        , m_LocationId(-1) {}

    Monster::~Monster() noexcept {
        std::cout << "Monster dtor" << std::endl;
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