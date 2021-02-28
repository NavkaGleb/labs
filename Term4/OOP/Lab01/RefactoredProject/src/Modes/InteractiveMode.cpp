#include "InteractiveMode.hpp"

#include <iostream>

#include <Random/Random.hpp>

#include "DataBase/DataBase.hpp"
#include "Entities/Location.hpp"
#include "Entities/Monster.hpp"

namespace RefactoredProject {

    namespace Internal {

        namespace Locations {

            void Create() {
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

            void CreateRandom() {
                int n;

                std::cout << "Enter amount of locations:" << std::endl;
                std::cin >> n;

                for (int i = 0; i < n; ++i) {
                    auto& location = DataBase::Get().Create<Location>();

                    location.SetName(Ng::Random::GetString(Ng::Random::Get(2, 7)));
                    location.SetArea(Ng::Random::Get(10.0f, 500.0f));
                    location.SetMonstersMaxCount(Ng::Random::Get(5, 100));
                }
                
                std::cout << n << " Locations was successfully added" << std::endl;
            }

            void PrintFromMemory() {
                auto locations = DataBase::Get().GetFromMemory<Location>();

                if (locations.empty()) {
                    std::cout << "No Locations in memory!" << std::endl;
                    return;
                }

                for (const auto& location : locations)
                    std::cout << location << std::endl;
            }

            void PrintFromFile() {
                auto locations = DataBase::Get().GetFromFile<Location>();

                if (locations.empty()) {
                    std::cout << "No Locations in file!" << std::endl;
                    return;
                }

                for (const auto& location : locations)
                    std::cout << location << std::endl;
            }

            void Load() {
                DataBase::Get().LoadFromFile<Location>();
                std::cout << "Locations was successfully loaded from file" << std::endl;
            }
            
            void Save() {
                DataBase::Get().SaveToFile<Location>();
                std::cout << "Locations was successfully saved to file" << std::endl;
            }

            void DeleteFromMemory(bool single = false) {
                if (!single) {
                    DataBase::Get().DeleteFromMemory<Location>();
                    std::cout << "Locations was successfully deleted from memory" << std::endl;

                    return;
                }

                auto locations = DataBase::Get().GetFromMemory<Location>();

                if (locations.empty()) {
                    std::cout << "No Locations on memory!";
                    return;
                }

                for (const auto& location : locations)
                    std::cout << location << std::endl;

                int id;

                std::cout << "Enter Location id" << std::endl;
                std::cin >> id;

                DataBase::Get().DeleteFromMemory<Location>(id);
            }

            void DeleteFromFile(bool single = false) {
                DataBase::Get().DeleteFromFile<Location>();
                std::cout << "Locations was successfully deleted from file!" << std::endl;
            }

            void UpdateInMemory() {
                auto locations = DataBase::Get().GetFromMemory<Location>();

                if (locations.empty()) {
                    std::cout << "No Locations in memory!" << std::endl;
                    return;
                }

                for (const auto& location : locations)
                    std::cout << location << std::endl;

                int locationId;

                std::cout << "Enter Location id:" << std::endl;
                std::cin >> locationId;

                auto&       location = DataBase::Get().Get<Location>(locationId);
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

                DataBase::Get().Update(location);
            }

            void UpdateInFile() {
                auto locations = DataBase::Get().GetFromFile<Location>();

                if (locations.empty()) {
                    std::cout << "No Locations in file!" << std::endl;
                    return;
                }

                for (const auto& location : locations)
                    std::cout << location << std::endl;

                int locationId;

                std::cout << "Enter Location id:" << std::endl;
                std::cin >> locationId;

                auto        location = *DataBase::Get().GetFromFile<Location>(locationId);
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

                DataBase::Get().Update(location);
            }
            
        } // namespace Locations
        
        namespace Monsters {

            void Create() {
                using AttackType = Monster::AttackType;

                std::vector<Location> locations = DataBase::Get().SearchInFile<Location>([](const Location& location) {
                    return !location.IsFull();
                });

                if (locations.empty()) {
                    std::cout << "No Location to add Monster! All Locations are full or there are no Location in file!";
                    return;
                }

                int locationId;

                for (const auto& location : locations)
                    std::cout << location << std::endl;

                std::cout << "Enter monster locationId:" << std::endl;
                std::cin >> locationId;

                auto [location, monster] = DataBase::Get().Create<Location, Monster>(locationId);
                std::string name;
                int         health;
                int         damage;
                float       attack;
                AttackType  attackType;

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

                monster.SetName(name);
                monster.SetHealth(health);
                monster.SetDamage(damage);
                monster.SetAttack(attack);
                monster.SetAttackType(attackType);
                monster.SetLocationId(locationId);

                location.IncreaseMonsters();

                DataBase::Get().Update<Location>(location);

                std::cout << "Monster " << monster.GetName() << " was successfully created!" << std::endl;
            }

            void CreateRandom() {
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

            void PrintFromMemory() {
                auto monsters = DataBase::Get().GetFromMemory<Monster>();

                if (monsters.empty()) {
                    std::cout << "No Monsters in memory!" << std::endl;
                    return;
                }

                for (const auto& monster : monsters)
                    std::cout << monster << std::endl;
            }

            void PrintFromFile() {
                auto monsters = DataBase::Get().GetFromFile<Monster>();

                if (monsters.empty()) {
                    std::cout << "No Monsters in file!" << std::endl;
                    return;
                }

                for (const auto& monster : monsters)
                    std::cout << monster << std::endl;
            }

            void Load() {
                DataBase::Get().LoadFromFile<Monster>();
                std::cout << "Monsters was successfully loaded from file" << std::endl;
            }

            void Save() {
                DataBase::Get().SaveToFile<Monster>();
                std::cout << "Monsters was successfully saved to file" << std::endl;
            }

            void DeleteFromMemory() {
                DataBase::Get().DeleteFromMemory<Monster>();
                std::cout << "Monsters was successfully deleted from memory" << std::endl;
            }

            void DeleteFromFile() {
                DataBase::Get().DeleteFromFile<Monster>();



                std::cout << "Monsters was successfully deleted from file" << std::endl;
            }
            
        } // namespace Monsters

    }

    void InteractiveMode_Impl::Run() {
        while (true) {
            UpdateCommand();

            if (!ParseCommand())
                return;
        }
    }

    void InteractiveMode_Impl::UpdateCommand() {
        using std::cout;
        using std::endl;

        cout << "Location commands                  | Monster commands                 " << endl;
        cout << "00 - Create Location               | 20 - Create Monster              " << endl;
        cout << "01 - Create 'n' random Locations   | 21 - Create 'n' random Monsters  " << endl;
        cout << "02 - Print Locations from memory   | 22 - Print Monsters from memory  " << endl;
        cout << "03 - Print Locations from file     | 23 - Print Monsters from file    " << endl;
        cout << "04 - Load Locations                | 24 - Load Monsters               " << endl;
        cout << "05 - Save Locations                | 25 - Save Monsters               " << endl;
        cout << "06 - Delete Location from memory   | 26 - Delete Monster from memory " << endl;
        cout << "07 - Delete Location from file     | 27 - Delete Monster from file   " << endl;
        cout << "08 - Delete Locations from memory  | 28 - Delete Monsters from memory" << endl;
        cout << "09 - Delete Locations from file    | 29 - Delete Monsters from file  " << endl;
        cout << "10 - Update Location in memory     | 30 - Update Monster in memory " << endl;
        cout << "11 - Update Location in file       | 31 - Update Monster in file     " << endl;
        cout << "12 - Search Locations in memory    | 32 - Search Monsters in memory  " << endl;
        cout << "13 - Search Locations in file      | 33 - Search Monsters in file    " << endl;
        cout << "40 - Exit                                                           :)" << endl;

        std::cin >> m_Command;
    }

    InteractiveMode_Impl::InteractiveMode_Impl()
        : m_Command(-1) {}

    bool InteractiveMode_Impl::ParseCommand() const {
        switch (m_Command / 20) {
            case 0:
                ParseLocationsCommand();
                break;

            case 1:
                ParseMonstersCommand();
                break;

            case 2:
                return false;

            default:
                std::cout << "No such command. Try again!" << std::endl;
                break;
        }

        std::cout << std::endl;

        return true;
    }

    void InteractiveMode_Impl::ParseLocationsCommand() const {
        using namespace Internal::Locations;
        
        switch (m_Command % 10) {
            case 0:
                Create();
                break;

            case 1:
                CreateRandom();
                break;

            case 2:
                PrintFromMemory();
                break;

            case 3:
                PrintFromFile();
                break;

            case 4:
                Load();
                break;

            case 5:
                Save();
                break;

            case 6:
                DeleteFromMemory(true);
                break;

            case 7:
                DeleteFromFile(true);
                break;

            case 8:
                DeleteFromMemory(false);
                break;
                
            case 9:
                DeleteFromFile(false);
                break;

            case 10:
                UpdateInMemory();
                break;

            case 11:
                UpdateInFile();
                break;

            default:
                std::cout << "No such command. Try again!" << std::endl;
                break;
        }
    }

    void InteractiveMode_Impl::ParseMonstersCommand() const {
        using namespace Internal::Monsters;

        switch (m_Command % 10) {
            case 0:
                Create();
                break;

            case 1:
                CreateRandom();
                break;

            case 2:
                PrintFromMemory();
                break;

            case 3:
                PrintFromFile();
                break;

            case 4:
                Load();
                break;

            case 5:
                Save();
                break;

            case 6:
//                Internal::LoadLocations();
                break;

            case 7:
//                Internal::DeleteLocationsFromMemory();
                break;

            case 8:
                DeleteFromMemory();
                break;

            case 9:
                DeleteFromFile();
                break;

            default:
                std::cout << "No such command. Try again!" << std::endl;
                break;
        }
    }

} // namespace RefactoredProject