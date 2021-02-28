#include "InteractiveMode.hpp"

#include <iostream>

#include <Random/Random.hpp>

#include "DataBase/DataBase.hpp"
#include "Entities/Location.hpp"
#include "Entities/Monster.hpp"

namespace RefactoredProject {

    namespace Internal {

        void CreateLocation() {
            auto&       location = DataBase::Get().Create<Location>();
            std::string name;
            float       area;
            int         monstersMaxCount;

            std::cout << "Enter location name:" << std::endl;
            std::cin >> name;

            std::cout << "Enter location area:" << std::endl;
            std::cin >> area;

            std::cout << "Enter location monstersMaxCount:" << std::endl;
            std::cin >> monstersMaxCount;

            location.SetName(name);
            location.SetArea(area);
            location.SetMonstersMaxCount(monstersMaxCount);

            std::cout << "Location " << location.GetName() << " was successfully created!" << std::endl;
        }

        void CreateNRandomLocations() {
            int n;

            std::cout << "Enter amount of locations:" << std::endl;
            std::cin >> n;

            for (int i = 0; i < n; ++i) {
                auto& location = DataBase::Get().Create<Location>();

                location.SetName(Ng::Random::GetString(Ng::Random::Get(2, 7)));
                location.SetArea(Ng::Random::Get(10.0f, 500.0f));
                location.SetMonstersMaxCount(Ng::Random::Get(5, 100));
            }
        }

        void IsLocationExists() {
            int id;

            std::cout << "Enter Location id:" << std::endl;
            std::cin >> id;

            if (DataBase::Get().IsExists<Location>(id))
                std::cout << "Location is exists!" << std::endl;
            else
                std::cout << "Location isn't exists!" << std::endl;
        }

        void PrintLocationsFromMemory() {
            bool response = DataBase::Get().PrintFromMemory<Location>();

            if (!response)
                std::cout << "No Locations on memory!" << std::endl;
        }

        void PrintLocationsFromFile() {
            bool response = DataBase::Get().PrintFromFile<Location>();

            if (!response)
                std::cout << "No Locations on file!" << std::endl;
        }

        void SaveLocation() {
            DataBase::Get().SaveToFile<Location>();
        }

        void LoadLocations() {
            DataBase::Get().LoadFromFile<Location>();
        }

        void DeleteLocationsFromMemory() {
            DataBase::Get().DeleteFromMemory<Location>();
        }

        void DeleteLocationsFromFile() {
            DataBase::Get().DeleteFromFile<Location>();
        }

        void CreateMonster() {
            using AttackType = Monster::AttackType;

            if (DataBase::Get().IsEmptyFile<Location>()) {
                std::cout << "No Locations in file" << std::endl;
                return;
            }

            auto&       monster = DataBase::Get().Create<Monster>();
            std::string name;
            int         health;
            int         damage;
            float       attack;
            AttackType  attackType;
            int         locationId;

            std::cout << "Enter monster name:" << std::endl;
            std::cin >> name;

            std::cout << "Enter monster health:" << std::endl;
            std::cin >> health;

            std::cout << "Enter monster damage:" << std::endl;
            std::cin >> damage;

            std::cout << "Enter monster attack:" << std::endl;
            std::cin >> attack;

            std::cout << "Enter monster attackType:" << std::endl;
            std::cin >> reinterpret_cast<int&>(attackType);

            while (true) {
                std::cout << "Enter monster locationId:" << std::endl;
                std::cin >> locationId;

                if (!DataBase::Get().IsExists<Location>(locationId)) {
                    std::cout << "No Location by id = " << locationId << std::endl;
                    continue;
                }

                auto& location = DataBase::Get().Get<Location>(locationId);
                bool response  = location.IncreaseMonsters();

                if (response)
                    break;
                else
                    std::cout << "Location is full!" << std::endl;
            }



            monster.SetName(name);
            monster.SetHealth(health);
            monster.SetDamage(damage);
            monster.SetAttack(attack);
            monster.SetAttackType(attackType);
            monster.SetLocationId(locationId);

            std::cout << "Monster " << monster.GetName() << " was successfully created!" << std::endl;
        }

        void CreateNRandomMonsters() {
            int n;
            std::vector<int> locationIds = DataBase::Get().GetIdsFromFile<Location>();

            if (locationIds.empty()) {
                std::cout << "No Locations in file!" << std::endl;
                return;
            }

            std::cout << "Enter amount of monsters:" << std::endl;
            std::cin >> n;

            for (int i = 0; i < n; ++i) {
                auto& monster = DataBase::Get().Create<Monster>();

                monster.SetName(Ng::Random::GetString(Ng::Random::Get(3, 7)));
                monster.SetHealth(Ng::Random::Get(10, 100));
                monster.SetDamage(Ng::Random::Get(15, 40));
                monster.SetAttack(Ng::Random::Get(0.0f, 1.0f));
                monster.SetAttackType(static_cast<Monster::AttackType>(Ng::Random::Get(1, 4)));
                monster.SetLocationId(Ng::Random::Get(locationIds));
            }
        }

        void PrintMonstersFromMemory() {
            bool response = DataBase::Get().PrintFromMemory<Monster>();

            if (!response)
                std::cout << "No Monsters in memory" << std::endl;
        }

        void PrintMonstersFromFile() {
            bool response = DataBase::Get().PrintFromFile<Monster>();

            if (!response)
                std::cout << "No Monsters in file" << std::endl;
        }

    }

    void InteractiveMode_Impl::Run() {
        char response = 'y';

        while (response == 'y') {
            UpdateCommand();
            ParseCommand();

            std::cout << "\ndo you want to continue? press 'y' or 'n'" << '\n';
            std::cin >> response;

            while (response != 'y' && response != 'n') {
                std::cout << "please, press 'y' or 'n'!" << '\n';
                std::cin >> response;
            }
        }
    }

    void InteractiveMode_Impl::UpdateCommand() {
        std::cout << "Location commands" << std::endl;
        std::cout << "00 - Create new Location" << std::endl;
        std::cout << "01 - Create 'n' random Locations" << std::endl;
        std::cout << "02 - Check if Location exists in memory" << std::endl;
        std::cout << "03 - Print all Locations from memory" << std::endl;
        std::cout << "04 - Print all Locations from file" << std::endl;
        std::cout << "05 - Save all Location from memory to file" << std::endl;
        std::cout << "06 - Load all Location from file to memory" << std::endl;
        std::cout << "07 - Delete all Locations from memory" << std::endl;
        std::cout << "08 - Delete all Locations from file" << std::endl;
        std::cout << "Monster commands" << std::endl;
        std::cout << "10 - Create new Monster" << std::endl;
        std::cout << "11 - Create 'n' random Monsters" << std::endl;
        std::cout << "12 - Print all Monsters from memory" << std::endl;
        std::cout << "13 - Print all Monsters from file" << std::endl;


        std::cin >> m_Command;
    }

    InteractiveMode_Impl::InteractiveMode_Impl()
        : m_Command(-1) {}

    void InteractiveMode_Impl::ParseCommand() const {
        auto& db = DataBase::Get();

        switch (m_Command / 10) {
            case 0:
                ParseLocationsCommand();
                break;

            case 1:
                ParseMonstersCommand();
                break;

            default:
                std::cout << "Fuck this shit" << std::endl;
        }
    }

    void InteractiveMode_Impl::ParseLocationsCommand() const {
        switch (m_Command % 10) {
            case 0:
                Internal::CreateLocation();
                break;

            case 1:
                Internal::CreateNRandomLocations();
                break;

            case 2:
                Internal::IsLocationExists();
                break;

            case 3:
                Internal::PrintLocationsFromMemory();
                break;

            case 4:
                Internal::PrintLocationsFromFile();
                break;

            case 5:
                Internal::SaveLocation();
                break;

            case 6:
                Internal::LoadLocations();
                break;

            case 7:
                Internal::DeleteLocationsFromMemory();
                break;

            case 8:
                Internal::DeleteLocationsFromFile();
                break;

            default:
                std::cout << "Fuck" << std::endl;
                break;
        }
    }

    void InteractiveMode_Impl::ParseMonstersCommand() const {
        switch (m_Command % 10) {
            case 0:
                Internal::CreateMonster();
                break;

            case 1:
                Internal::CreateNRandomMonsters();
                break;

            case 2:
                Internal::PrintMonstersFromMemory();
                break;

            case 3:
                Internal::PrintMonstersFromFile();
                break;

            default:
                std::cout << "Fuck this monsters" << std::endl;
        }
    }

} // namespace RefactoredProject