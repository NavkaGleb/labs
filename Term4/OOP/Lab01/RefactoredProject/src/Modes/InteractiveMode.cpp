#include "InteractiveMode.hpp"

#include <iostream>

#include "LocationManager.hpp"
#include "MonsterManager.hpp"

namespace RefactoredProject {

    void InteractiveMode_Impl::Run() {
        while (true) {
            UpdateCommand();

            if (!ParseCommand())
                return;
        }
    }

    InteractiveMode_Impl::InteractiveMode_Impl()
        : m_Command(-1) {

        LocationManager::Create(std::cin, std::cout);
        MonsterManager::Create(std::cin, std::cout);

        LocationManager::Get().
    }

    InteractiveMode_Impl::~InteractiveMode_Impl() noexcept {
        LocationManager::Destroy();
    }

    void InteractiveMode_Impl::UpdateCommand() {
        std::cout << "Location commands                  | Monster commands                " << std::endl;
        std::cout << "00 - Create Location               | 20 - Create Monster             " << std::endl;
        std::cout << "01 - Create 'n' random Locations   | 21 - Create 'n' random Monsters " << std::endl;
        std::cout << "02 - Print Locations from memory   | 22 - Print Monsters from memory " << std::endl;
        std::cout << "03 - Print Locations from file     | 23 - Print Monsters from file   " << std::endl;
        std::cout << "04 - Load Locations                | 24 - Load Monsters              " << std::endl;
        std::cout << "05 - Save Locations                | 25 - Save Monsters              " << std::endl;
        std::cout << "06 - Delete Location from memory   | 26 - Delete Monster from memory " << std::endl;
        std::cout << "07 - Delete Location from file     | 27 - Delete Monster from file   " << std::endl;
        std::cout << "08 - Delete Locations from memory  | 28 - Delete Monsters from memory" << std::endl;
        std::cout << "09 - Delete Locations from file    | 29 - Delete Monsters from file  " << std::endl;
        std::cout << "10 - Truncate Locations            | 30 - Truncate Monsters          " << std::endl;
        std::cout << "11 - Update Location in memory     | 31 - Update Monster in memory   " << std::endl;
        std::cout << "12 - Update Location in file       | 32 - Update Monster in file     " << std::endl;
        std::cout << "13 - Search Locations in memory    | 33 - Search Monsters in memory  " << std::endl;
        std::cout << "14 - Search Locations in file      | 34 - Search Monsters in file    " << std::endl;
        std::cout << "40 - Exit                                                          :)" << std::endl;
        std::cout << "60 - Print RelationTable                                             " << std::endl;
        std::cout << "80 - Print IndexTable                                                " << std::endl;

        std::cin >> m_Command;
    }

    bool InteractiveMode_Impl::ParseCommand() const {
        switch (m_Command / 20) {
            case 0:  LocationManager::Get().Run(m_Command);                   break;
            case 1:  MonsterManager::Get().Run(m_Command);                    break;
            case 2:  return false;
            case 3:  DataBase::Get().PrintRelationTable();                    break;
            case 4:  DataBase::Get().PrintIndexTable();                       break;
            default: std::cout << "No such command. Try again!" << std::endl; break;
        }

        std::cout << std::endl;

        return true;
    }

} // namespace RefactoredProject