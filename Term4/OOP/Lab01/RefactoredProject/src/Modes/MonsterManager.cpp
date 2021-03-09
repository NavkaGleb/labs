#include "MonsterManager.hpp"

#include <Random/Random.hpp>

#include "Entities/Location.hpp"
#include "Entities/Monster.hpp"

namespace RefactoredProject {

    MonsterManager_Impl::MonsterManager_Impl(std::istream& istream, std::ostream& ostream)
        : IEntityManager(istream, ostream) {}
    
    void MonsterManager_Impl::Create() {
        auto locations = m_DataBase.SearchInFile<Location>([](const Location& location) {
            return !location.IsFull();
        });

        if (locations.empty()) {
            m_OutputStream << "No Location to add Monster! All Locations are full or there are no Location in file!";
            return;
        }

        MonsterProps props;

        for (const auto& location : locations)
            m_OutputStream << *location << std::endl;

        m_OutputStream << "Enter monster locationId:" << std::endl;
        m_InputStream >> props.LocationId;

        auto [location, monster] = m_DataBase.Create<Location, Monster>(props.LocationId);

        m_OutputStream << "Enter monster name:" << std::endl;
        m_InputStream >> props.Name;

        m_OutputStream << "Enter monster health:" << std::endl;
        m_InputStream >> props.Health;

        m_OutputStream << "Enter monster damage:" << std::endl;
        m_InputStream >> props.Damage;

        m_OutputStream << "Enter monster attack:" << std::endl;
        m_InputStream >> props.Attack;

        m_OutputStream << "Enter monster attackType:" << std::endl;
        for (int i = 1; i < 5; ++i)
            m_OutputStream << i << " - " << static_cast<Monster::AttackType>(i) << std::endl;
        m_InputStream >> reinterpret_cast<int&>(props.AttackType);

        monster.SetProps(std::move(props));

        location.IncreaseMonsters();

        m_DataBase.Update<Location>(location);

        m_OutputStream << "Monster " << monster.GetName() << " was successfully created!" << std::endl;
    }

    void MonsterManager_Impl::CreateRandom() {
        int  n;
        auto locations = m_DataBase.SearchInFile<Location>([](const Location& location) {
            return !location.IsFull();
        });

        if (locations.empty()) {
            m_OutputStream << "No Location to add Monster! All Locations are full or there are no Location in file!";
            return;
        }

        m_OutputStream << "Enter amount of monsters:" << std::endl;
        m_InputStream >> n;

        for (int i = 0; i < n; ++i) {
            auto& monster = m_DataBase.Create<Monster>();
            auto location = Ng::Random::Get(locations);

            monster.SetName(Ng::Random::GetString(Ng::Random::Get(3, 7)));
            monster.SetHealth(Ng::Random::Get(10, 100));
            monster.SetDamage(Ng::Random::Get(15, 40));
            monster.SetAttack(Ng::Random::Get(0.0f, 1.0f));
            monster.SetAttackType(static_cast<Monster::AttackType>(Ng::Random::Get(1, 4)));
            monster.SetLocationId(location->GetId());

            location->IncreaseMonsters();

            m_DataBase.Update<Location>(*location);
        }
    }

    bool MonsterManager_Impl::PrintFromMemory() {
        auto monsters = m_DataBase.GetFromMemory<Monster>();

        if (monsters.empty()) {
            m_OutputStream << "No Monsters in memory!" << std::endl;
            return false;
        }

        for (const auto& monster : monsters)
            m_OutputStream << *monster << std::endl;

        return true;
    }

    bool MonsterManager_Impl::PrintFromFile() {
        auto monsters = m_DataBase.GetFromFile<Monster>();

        if (monsters.empty()) {
            m_OutputStream << "No Monsters in file!" << std::endl;
            return false;
        }

        for (const auto& monster : monsters)
            m_OutputStream << *monster << std::endl;

        return true;
    }

    void MonsterManager_Impl::Load() {
        m_DataBase.Load<Monster>();
        m_OutputStream << "Monsters was successfully loaded from file" << std::endl;
    }

    void MonsterManager_Impl::Save() {
        m_DataBase.Save<Monster>();
        m_OutputStream << "Monsters was successfully saved to file" << std::endl;
    }

    void MonsterManager_Impl::DeleteFromMemory(CountType countType) {
        if (countType == CountType::Single) {
            if (!PrintFromMemory()) {
                m_OutputStream << "No Monsters in memory!" << std::endl;
                return;
            }

            int monsterId;

            m_OutputStream << "Enter Monster id:" << std::endl;
            m_InputStream >> monsterId;

            auto& monster  = m_DataBase.GetFromMemory<Monster>(monsterId);
            auto  location = m_DataBase.GetFromFile<Location>(monster.GetLocationId());

            if (!m_DataBase.DeleteFromMemory<Monster>(monsterId)) {
                location.DecreaseMonsters();
                m_DataBase.Update<Location>(location);
            }

            m_OutputStream << "Location " << monsterId << " was successfully deleted from memory!" << std::endl;
        } else {
            auto monstersInMemory = m_DataBase.DeleteFromMemory<Monster>();

            for (const auto& monster : monstersInMemory) {
                auto location = m_DataBase.GetFromFile<Location>(monster.GetLocationId());
                location.DecreaseMonsters();
                m_DataBase.Update<Location>(location);
            }

            m_OutputStream << "Monsters was successfully deleted from memory" << std::endl;
        }
    }

    void MonsterManager_Impl::DeleteFromFile(CountType countType) {
        if (countType == CountType::Single) {
            if (!PrintFromFile()) {
                m_OutputStream << "No Monsters in file!" << std::endl;
                return;
            }

            int monsterId;

            m_OutputStream << "Enter Monster id:" << std::endl;
            m_InputStream >> monsterId;

            auto monster  = m_DataBase.GetFromFile<Monster>(monsterId);
            auto location = m_DataBase.GetFromFile<Location>(monster.GetLocationId());

            location.DecreaseMonsters();

            m_DataBase.DeleteFromFile<Monster>(monsterId);
            m_DataBase.Update<Location>(location);

            m_OutputStream << "Monster " << monsterId << " was successfully deleted from file!" << std::endl;
        } else {
            m_DataBase.DeleteFromFile<Monster>();

            auto locations = m_DataBase.GetFromFile<Location>();

            for (auto& location : locations) {
                location->DecreaseMonsters(location->GetMonstersCount());
                m_DataBase.Update<Location>(*location);
            }

            m_OutputStream << "Monsters was successfully deleted from file" << std::endl;
        }
    }

    void MonsterManager_Impl::Truncate() {
        m_DataBase.Truncate<Location>();
    }

    void MonsterManager_Impl::UpdateInMemory() {
        if (!PrintFromMemory())
            return;

        int monsterId;

        m_OutputStream << "Enter Monster id:" << std::endl;
        m_InputStream >> monsterId;

        auto&        monster = m_DataBase.GetFromMemory<Monster>(monsterId);
        MonsterProps props;

        m_OutputStream << "Enter monster name:" << std::endl;
        m_InputStream >> props.Name;

        m_OutputStream << "Enter monster health:" << std::endl;
        m_InputStream >> props.Health;

        m_OutputStream << "Enter monster damage:" << std::endl;
        m_InputStream >> props.Damage;

        m_OutputStream << "Enter monster attack:" << std::endl;
        m_InputStream >> props.Attack;

        m_OutputStream << "Enter monster attackType:" << std::endl;
        for (int i = 1; i < 5; ++i)
            m_OutputStream << i << " - " << static_cast<Monster::AttackType>(i) << std::endl;
        m_InputStream >> reinterpret_cast<int&>(props.AttackType);

        auto locations = m_DataBase.SearchInFile<Location>([](const Location& location) {
            return !location.IsFull();
        });

        if (locations.empty()) {
            m_OutputStream << "No Location to add Monster! All Locations are full";
        } else {
            for (const auto& location : locations)
                m_OutputStream << *location << std::endl;

            m_OutputStream << "Enter Location id:" << std::endl;
            m_OutputStream << props.LocationId;

            if (props.LocationId != monster.GetLocationId()) {
                auto oldLocation = m_DataBase.GetFromFile<Location>(monster.GetLocationId());
                auto newLocation = m_DataBase.GetFromFile<Location>(props.LocationId);

                oldLocation.DecreaseMonsters();
                newLocation.IncreaseMonsters();

                m_DataBase.DeleteConnection<Location, Monster>(oldLocation.GetId(), monster.GetId());
                m_DataBase.CreateConnection<Location, Monster>(newLocation.GetId(), monster.GetId());

                m_DataBase.Update<Location>(oldLocation);
                m_DataBase.Update<Location>(newLocation);
            }
        }

        monster.SetProps(std::move(props));

        m_DataBase.Update<Monster>(monster);
    }

    void MonsterManager_Impl::UpdateInFile() {
        if (!PrintFromFile())
            return;

        int monsterId;

        m_OutputStream << "Enter Monster id:" << std::endl;
        m_InputStream >> monsterId;

        auto         monster = m_DataBase.GetFromFile<Monster>(monsterId);
        MonsterProps props;

        m_OutputStream << "Enter monster name:" << std::endl;
        m_InputStream >> props.Name;

        m_OutputStream << "Enter monster health:" << std::endl;
        m_InputStream >> props.Health;

        m_OutputStream << "Enter monster damage:" << std::endl;
        m_InputStream >> props.Damage;

        m_OutputStream << "Enter monster attack:" << std::endl;
        m_InputStream >> props.Attack;

        m_OutputStream << "Enter monster attackType:" << std::endl;
        for (int i = 1; i < 5; ++i)
            m_OutputStream << i << " - " << static_cast<Monster::AttackType>(i) << std::endl;
        m_InputStream >> reinterpret_cast<int&>(props.AttackType);

        auto locations = m_DataBase.SearchInFile<Location>([](const Location& location) {
            return !location.IsFull();
        });

        if (locations.empty()) {
            m_OutputStream << "No Location to add Monster! All Locations are full";
        } else {
            for (const auto& location : locations)
                m_OutputStream << *location << std::endl;

            m_OutputStream << "Enter Location id:" << std::endl;
            m_OutputStream << props.LocationId;

            if (props.LocationId != monster.GetLocationId()) {
                auto oldLocation = m_DataBase.GetFromFile<Location>(monster.GetLocationId());
                auto newLocation = m_DataBase.GetFromFile<Location>(props.LocationId);

                oldLocation.DecreaseMonsters();
                newLocation.IncreaseMonsters();

                m_DataBase.DeleteConnection<Location, Monster>(oldLocation.GetId(), monster.GetId());
                m_DataBase.CreateConnection<Location, Monster>(newLocation.GetId(), monster.GetId());

                m_DataBase.Update<Location>(oldLocation);
                m_DataBase.Update<Location>(newLocation);
            }
        }

        monster.SetProps(std::move(props));

        m_DataBase.Update<Monster>(monster);
    }

    void MonsterManager_Impl::SearchInMemory() {

    }

    void MonsterManager_Impl::SearchInFile() {

    }

} // namespace RefactoredProject