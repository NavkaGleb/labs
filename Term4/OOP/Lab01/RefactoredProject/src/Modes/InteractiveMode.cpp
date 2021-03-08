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

    void InteractiveMode_Impl::UpdateCommand() {
        using std::cout;
        using std::endl;

        cout << "Location commands                  | Monster commands                " << endl;
        cout << "00 - Create Location               | 20 - Create Monster             " << endl;
        cout << "01 - Create 'n' random Locations   | 21 - Create 'n' random Monsters " << endl;
        cout << "02 - Print Locations from memory   | 22 - Print Monsters from memory " << endl;
        cout << "03 - Print Locations from file     | 23 - Print Monsters from file   " << endl;
        cout << "04 - Load Locations                | 24 - Load Monsters              " << endl;
        cout << "05 - Save Locations                | 25 - Save Monsters              " << endl;
        cout << "06 - Delete Location from memory   | 26 - Delete Monster from memory " << endl;
        cout << "07 - Delete Location from file     | 27 - Delete Monster from file   " << endl;
        cout << "08 - Delete Locations from memory  | 28 - Delete Monsters from memory" << endl;
        cout << "09 - Delete Locations from file    | 29 - Delete Monsters from file  " << endl;
        cout << "10 - Truncate Locations            | 30 - Truncate Monsters          " << endl;
        cout << "11 - Update Location in memory     | 31 - Update Monster in memory   " << endl;
        cout << "12 - Update Location in file       | 32 - Update Monster in file     " << endl;
        cout << "13 - Search Locations in memory    | 33 - Search Monsters in memory  " << endl;
        cout << "14 - Search Locations in file      | 34 - Search Monsters in file    " << endl;
        cout << "40 - Exit                                                          :)" << endl;
        cout << "60 - Print RelationTable                                             " << endl;
        cout << "80 - Print IndexTable" << endl;

        std::cin >> m_Command;
    }

    InteractiveMode_Impl::InteractiveMode_Impl()
        : m_Command(-1) {

        LocationManager::Create();
        MonsterManager::Create();
    }

    InteractiveMode_Impl::~InteractiveMode_Impl() noexcept {
        LocationManager::Destroy();
    }

    bool InteractiveMode_Impl::ParseCommand() const {
        switch (m_Command / 20) {
            case 0: LocationManager::Get().Run(m_Command); break;
            case 1: MonsterManager::Get().Run(m_Command);  break;
            case 2: return false;
            case 3: DataBase::Get().PrintRelationTable();  break;
            case 4: DataBase::Get().PrintIndexTable();     break;

            default:
                std::cout << "No such command. Try again!" << std::endl;
                break;
        }

        std::cout << std::endl;

        return true;
    }

} // namespace RefactoredProject