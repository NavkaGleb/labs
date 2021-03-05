#pragma once

#include <string>

#include "DataBase/IDataBaseEntity.hpp"

namespace RefactoredProject {

    class Monster : public IDataBaseEntity {
    public:
        enum class AttackType : int {
            None = 0,
            IncreaseDamage,
            RepeatAttack,
            SelfCure,
            EnemyParalyze
        };

        explicit Monster(int id = -1);
        ~Monster() noexcept override;

        [[nodiscard]] inline const std::string& GetName() const { return m_Name; }
        [[nodiscard]] inline int GetHealth() const { return m_Health; }
        [[nodiscard]] inline int GetDamage() const { return m_Damage; }
        [[nodiscard]] inline float GetAttack() const { return m_Attack; }
        [[nodiscard]] inline const AttackType& GetAttackType() const { return m_AttackType; }
        [[nodiscard]] inline int GetLocationId() const { return m_LocationId; }

        void SetName(const std::string& name);
        void SetHealth(int health);
        void SetDamage(int damage);
        void SetAttack(float attack);
        void SetAttackType(const AttackType& attackType);
        void SetLocationId(int locationId);

        void ReadFromBinary(std::ifstream& infile) override {}
        void WriteToBinary(std::ofstream& outfile) const override {}

        friend std::istream& operator >>(std::istream& istream, Monster& monster);
        friend std::ostream& operator <<(std::ostream& ostream, const Monster& monster);

    private:
        std::string m_Name;
        int         m_Health;
        int         m_Damage;
        float       m_Attack;
        AttackType  m_AttackType;
        int         m_LocationId;

    }; // class Monster

} // namespace RefactoredProject