#include "Application.hpp"

#include <iostream>

#include "Modes/InteractiveMode.hpp"
#include "Modes/DemoMode.hpp"
#include "Modes/BenchmarkMode.hpp"

#include "DataBase/DataBase.hpp"

namespace RefactoredProject {

    void Application_Impl::Run() {
        char response = 'y';
        int  mode;

        while (response == 'y') {
            std::cout << "select the mode:" << std::endl;
            std::cout << "0 - interactive mode" << std::endl;
            std::cout << "1 - demo mode" << std::endl;
            std::cout << "2 - benchmark mode" << std::endl;

            std::cin >> mode;

            switch (mode) {
                case 0:
                    InteractiveMode::Create().Run();
                    InteractiveMode::Destroy();
                    break;

                case 1:
                    RefactoredProject::DemoMode::Create().Run();
                    DemoMode::Destroy();
                    break;

                case 2:
                    BenchmarkMode::Create().Run();
                    BenchmarkMode::Destroy();
                    break;

                default:
                    std::cout << "try again!" << std::endl;
                    break;
            }

            std::cout << '\n' << "do you want to select another mode? press 'y' or 'q'. 'y' - yes, 'q' - quit" << '\n';
            std::cin >> response;

            while (response != 'y' && response != 'q') {

                std::cout << "please, press 'y' or 'q'!" << '\n';
                std::cin >> response;

            }
        }
    }

    Application_Impl::Application_Impl() {
        DataBase::Create();
    }

    Application_Impl::~Application_Impl() noexcept {
        DataBase::Destroy();
    }

} // namespace RefactoredProject