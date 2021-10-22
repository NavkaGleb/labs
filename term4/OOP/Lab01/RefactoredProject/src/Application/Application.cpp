#include "Application.hpp"

#include <iostream>

#include "Modes/InteractiveMode.hpp"
#include "Modes/DemoMode.hpp"
#include "Modes/BenchmarkMode.hpp"

#include "DataBase/DataBase.hpp"
#include "Entities/Location.hpp"
#include "Entities/Monster.hpp"

namespace RefactoredProject {

    void Application_Impl::Run() {
        int mode;

        while (true) {
            std::cout << "Select the mode:" << std::endl;
            std::cout << "0 - Interactive mode" << std::endl;
            std::cout << "1 - Demo mode" << std::endl;
            std::cout << "2 - Benchmark mode" << std::endl;
            std::cout << "3 - Exit" << std::endl;

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

                case 3:
                    return;

                default:
                    std::cout << "try again!" << std::endl;
                    break;
            }
        }
    }

    Application_Impl::Application_Impl() {
        DataBase::Create();
        DataBase::Get().CreateRelation<Location, Monster>();
        DataBase::Get().Init<Location>();
        DataBase::Get().Init<Monster>();
    }

    Application_Impl::~Application_Impl() noexcept {
        DataBase::Destroy();
    }

} // namespace RefactoredProject