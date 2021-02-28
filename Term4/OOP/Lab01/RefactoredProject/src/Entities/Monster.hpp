#pragma once

#include <string>

namespace RefactoredProject {

    class Monster {
    public:
        enum class AttackType : int {
            None = 0
        };

        explicit Monster(int id);
        virtual ~Monster() noexcept;

        [[nodiscard]] inline int GetId() const { return m_Id; }
        [[nodiscard]] inline const std::string& GetName() const { return m_Name; }
        [[nodiscard]] inline int GetHealth() const { return m_Health; }
        [[nodiscard]] inline int GetDamage() const { return m_Damage; }
        [[nodiscard]] inline float GetAttack() const { return m_Attack; }
        [[nodiscard]] inline const AttackType& GetAttackType() const { return m_AttackType; }
        [[nodiscard]] inline int GetLocationId() const { return m_LocationId; }

        friend std::istream& operator >>(std::istream& istream, Monster& monster);
        friend std::ostream& operator <<(std::ostream& ostream, const Monster& monster);

    private:
        int         m_Id;
        std::string m_Name;
        int         m_Health;
        int         m_Damage;
        float       m_Attack;
        AttackType  m_AttackType;
        int         m_LocationId;

    }; // class Monster

} // namespace RefactoredProject