#include "MonsterManager.hpp"

#include <Random/Random.hpp>

#include "Entities/Location.hpp"
#include "Entities/Monster.hpp"

namespace RefactoredProject {

    void MonsterManager_Impl::Create() {
        auto locations = m_DataBase.SearchInFile<Location>([](const Location& location) {
            return !location.IsFull();
        });

        if (locations.empty()) {
            std::cout << "No Location to add Monster! All Locations are full or there are no Location in file!";
            return;
        }

        MonsterProps props;

        for (const auto& location : locations)
            std::cout << *location << std::endl;

        std::cout << "Enter monster locationId:" << std::endl;
        std::cin >> props.LocationId;

        auto [location, monster] = m_DataBase.Create<Location, Monster>(props.LocationId);

        std::cout << "Enter monster name:" << std::endl;
        std::cin >> props.Name;

        std::cout << "Enter monster health:" << std::endl;
        std::cin >> props.Health;

        std::cout << "Enter monster damage:" << std::endl;
        std::cin >> props.Damage;

        std::cout << "Enter monster attack:" << std::endl;
        std::cin >> props.Attack;

        std::cout << "Enter monster attackType:" << std::endl;
        for (int i = 1; i < 5; ++i)
            std::cout << i << " - " << static_cast<Monster::AttackType>(i) << std::endl;
        std::cin >> reinterpret_cast<int&>(props.AttackType);

        monster.SetProps(std::move(props));

        location.IncreaseMonsters();

        m_DataBase.Update<Location>(location);

        std::cout << "Monster " << monster.GetName() << " was successfully created!" << std::endl;
    }

    void MonsterManager_Impl::CreateRandom() {
        int  n;
        auto locations = m_DataBase.SearchInFile<Location>([](const Location& location) {
            return !location.IsFull();
        });

        if (locations.empty()) {
            std::cout << "No Location to add Monster! All Locations are full or there are no Location in file!";
            return;
        }

        std::cout << "Enter amount of monsters:" << std::endl;
        std::cin >> n;

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
            std::cout << "No Monsters in memory!" << std::endl;
            return false;
        }

        for (const auto& monster : monsters)
            std::cout << *monster << std::endl;

        return true;
    }

    bool MonsterManager_Impl::PrintFromFile() {
        auto monsters = m_DataBase.GetFromFile<Monster>();

        if (monsters.empty()) {
            std::cout << "No Monsters in file!" << std::endl;
            return false;
        }

        for (const auto& monster : monsters)
            std::cout << *monster << std::endl;

        return true;
    }

    void MonsterManager_Impl::Load() {
        m_DataBase.Load<Monster>();
        std::cout << "Monsters was successfully loaded from file" << std::endl;
    }

    void MonsterManager_Impl::Save() {
        m_DataBase.Save<Monster>();
        std::cout << "Monsters was successfully saved to file" << std::endl;
    }

    void MonsterManager_Impl::DeleteFromMemory(CountType countType) {
        if (countType == CountType::Single) {
            if (!PrintFromMemory()) {
                std::cout << "No Monsters in memory!" << std::endl;
                return;
            }

            int monsterId;

            std::cout << "Enter Monster id:" << std::endl;
            std::cin >> monsterId;

            m_DataBase.DeleteFromMemory<Monster>(monsterId);

            std::cout << "Location " << monsterId << " was successfully deleted from memory!" << std::endl;
        } else {
            m_DataBase.DeleteFromMemory<Monster>();
            std::cout << "Monsters was successfully deleted from memory" << std::endl;
        }
    }

    void MonsterManager_Impl::DeleteFromFile(CountType countType) {
        if (countType == CountType::Single) {
            if (!PrintFromFile()) {
                std::cout << "No Monsters in file!" << std::endl;
                return;
            }

            int monsterId;

            std::cout << "Enter Monster id:" << std::endl;
            std::cin >> monsterId;

            m_DataBase.DeleteFromFile<Monster>(monsterId);

            std::cout << "Monster " << monsterId << " was successfully deleted from file!" << std::endl;
        } else {
            m_DataBase.DeleteFromFile<Monster>();

            std::cout << "Monsters was successfully deleted from file" << std::endl;
        }
    }

    void MonsterManager_Impl::Truncate() {
        m_DataBase.Truncate<Location>();
    }

    void MonsterManager_Impl::UpdateInMemory() {
        if (!PrintFromMemory())
            return;

        int monsterId;

        std::cout << "Enter Monster id:" << std::endl;
        std::cin >> monsterId;

        auto&        monster = m_DataBase.GetFromMemory<Monster>(monsterId);
        MonsterProps props;

        std::cout << "Enter monster name:" << std::endl;
        std::cin >> props.Name;

        std::cout << "Enter monster health:" << std::endl;
        std::cin >> props.Health;

        std::cout << "Enter monster damage:" << std::endl;
        std::cin >> props.Damage;

        std::cout << "Enter monster attack:" << std::endl;
        std::cin >> props.Attack;

        std::cout << "Enter monster attackType:" << std::endl;
        for (int i = 1; i < 5; ++i)
            std::cout << i << " - " << static_cast<Monster::AttackType>(i) << std::endl;
        std::cin >> reinterpret_cast<int&>(props.AttackType);

        auto locations = m_DataBase.SearchInFile<Location>([](const Location& location) {
            return !location.IsFull();
        });

        if (locations.empty()) {
            std::cout << "No Location to add Monster! All Locations are full";
        } else {
            for (const auto& location : locations)
                std::cout << *location << std::endl;

            std::cout << "Enter Location id:" << std::endl;
            std::cout << props.LocationId;

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

        std::cout << "Enter Monster id:" << std::endl;
        std::cin >> monsterId;

        auto         monster = m_DataBase.GetFromFile<Monster>(monsterId);
        MonsterProps props;

        std::cout << "Enter monster name:" << std::endl;
        std::cin >> props.Name;

        std::cout << "Enter monster health:" << std::endl;
        std::cin >> props.Health;

        std::cout << "Enter monster damage:" << std::endl;
        std::cin >> props.Damage;

        std::cout << "Enter monster attack:" << std::endl;
        std::cin >> props.Attack;

        std::cout << "Enter monster attackType:" << std::endl;
        for (int i = 1; i < 5; ++i)
            std::cout << i << " - " << static_cast<Monster::AttackType>(i) << std::endl;
        std::cin >> reinterpret_cast<int&>(props.AttackType);

        auto locations = m_DataBase.SearchInFile<Location>([](const Location& location) {
            return !location.IsFull();
        });

        if (locations.empty()) {
            std::cout << "No Location to add Monster! All Locations are full";
        } else {
            for (const auto& location : locations)
                std::cout << *location << std::endl;

            std::cout << "Enter Location id:" << std::endl;
            std::cout << props.LocationId;

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