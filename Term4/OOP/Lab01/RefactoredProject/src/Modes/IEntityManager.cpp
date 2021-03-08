#include "IEntityManager.hpp"

#include <iostream>

namespace RefactoredProject {

    IEntityManager::IEntityManager()
        : m_DataBase(DataBase::Get()) {}

    void IEntityManager::Run(int command) {
        switch (command % 20) {
            case 0:  Create();                                                break;
            case 1:  CreateRandom();                                          break;
            case 2:  PrintFromMemory();                                       break;
            case 3:  PrintFromFile();                                         break;
            case 4:  Load();                                                  break;
            case 5:  Save();                                                  break;
            case 6:  DeleteFromMemory(CountType::Single);                     break;
            case 7:  DeleteFromFile(CountType::Single);                       break;
            case 8:  DeleteFromMemory(CountType::Multiple);                   break;
            case 9:  DeleteFromFile(CountType::Multiple);                     break;
            case 10: Truncate();                                              break;
            case 11: UpdateInMemory();                                        break;
            case 12: UpdateInFile();                                          break;
            default: std::cout << "No such command. Try again!" << std::endl; break;
        }
    }

} // namespace RefactoredProject