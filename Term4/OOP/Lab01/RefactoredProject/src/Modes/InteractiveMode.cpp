#include "InteractiveMode.hpp"

#include <iostream>

#include "DataBase/DataBase.hpp"
#include "Entities/Location.hpp"
#include "Entities/Monster.hpp"

namespace RefactoredProject {

    void InteractiveMode_Impl::Run() {
        char response = 'y';
        int  command;

        while (response == 'y') {
            std::cout << "0 = create monster" << std::endl;
            std::cout << "1 = get monster" << std::endl;
            std::cout << "2 = print monsters from file" << std::endl;
            std::cout << "3 = create location" << std::endl;
            std::cout << "4 - get location" << std::endl;

            std::cin >> command;

            if (command == 0)
                DataBase::Get().Create<Monster>(123);

            if (command == 1) {
                std::cout << "Monster id: " << DataBase::Get().Get<Monster>(123).GetId() << std::endl;
            }

            if (command == 3)
                DataBase::Get().Create<Location>(321);

            if (command == 4)
                std::cout << "Location id: " << DataBase::Get().Get<Location>(321).GetId() << std::endl;

            std::cout << "\ndo you want to continue? press 'y' or 'n'" << '\n';
            std::cin >> response;

            while (response != 'y' && response != 'n') {
                std::cout << "please, press 'y' or 'n'!" << '\n';
                std::cin >> response;
            }
        }
    }

} // namespace RefactoredProject