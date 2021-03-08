#include "LocationManager.hpp"

#include <Random/Random.hpp>

#include "Entities/Location.hpp"

namespace RefactoredProject {
        
    void LocationManager_Impl::Create() {
        auto&       location = m_DataBase.Create<Location>();
        std::string name;
        float       area;
        int         monstersMaxCount;

        std::cout << "Enter location name:" << std::endl;
        std::cin >> name;

        std::cout << "Enter location area:" << std::endl;
        std::cin >> area;

        std::cout << "Enter location monstersMaxCount:" << std::endl;
        std::cin >> monstersMaxCount;

        location.SetName(std::move(name));
        location.SetArea(area);
        location.SetMonstersMaxCount(monstersMaxCount);

        std::cout << "Location " << location.GetName() << " was successfully created!" << std::endl;
    }

    void LocationManager_Impl::CreateRandom() {
        int n;

        std::cout << "Enter amount of locations:" << std::endl;
        std::cin >> n;

        for (int i = 0; i < n; ++i) {
            auto& location = m_DataBase.Create<Location>();

            location.SetName(Ng::Random::GetString(Ng::Random::Get(2, 7)));
            location.SetArea(Ng::Random::Get(10.0f, 500.0f));
            location.SetMonstersMaxCount(Ng::Random::Get(5, 100));
        }

        std::cout << n << " Locations was successfully added" << std::endl;
    }

    bool LocationManager_Impl::PrintFromMemory() {
        auto locations = m_DataBase.GetFromMemory<Location>();

        if (locations.empty()) {
            std::cout << "No Locations in memory!" << std::endl;
            return false;
        }

        for (const auto& location : locations)
            std::cout << *location << std::endl;

        return true;
    }

    bool LocationManager_Impl::PrintFromFile() {
        auto locations = m_DataBase.GetFromFile<Location>();

        if (locations.empty()) {
            std::cout << "No Locations in file!" << std::endl;
            return false;
        }

        for (const auto& location : locations)
            std::cout << *location << std::endl;

        return true;
    }

    void LocationManager_Impl::Load() {
        m_DataBase.Load<Location>();
        std::cout << "Locations was successfully loaded from file" << std::endl;
    }

    void LocationManager_Impl::Save() {
        m_DataBase.Save<Location>();
        std::cout << "Locations was successfully saved to file" << std::endl;
    }

    void LocationManager_Impl::DeleteFromMemory(CountType countType) {
        if (countType == CountType::Single) {
            if (!PrintFromMemory())
                std::cout << "No Locations in memory!" << std::endl;

            int locationId;

            std::cout << "Enter locationId:" << std::endl;
            std::cin >> locationId;

            m_DataBase.DeleteFromMemory<Location>(locationId);

            std::cout << "Location " << locationId << " was successfully deleted from memory!" << std::endl;
        } else {
            m_DataBase.DeleteFromMemory<Location>();

            std::cout << "Locations was successfully deleted from memory!" << std::endl;
        }
    }

    void LocationManager_Impl::DeleteFromFile(CountType countType) {
        if (countType == CountType::Single) {
            if (!PrintFromFile())
                return;

            int locationId;

            std::cout << "Enter locationId:" << std::endl;
            std::cin >> locationId;

            m_DataBase.DeleteFromFile<Location>(locationId);

            std::cout << "Location " << locationId << " was successfully deleted from file!" << std::endl;
        } else {
            m_DataBase.DeleteFromFile<Location>();

            std::cout << "Locations was successfully deleted from file!" << std::endl;
        }
    }
    
    void LocationManager_Impl::Truncate() {
        m_DataBase.Truncate<Location>();
    }

    void LocationManager_Impl::UpdateInMemory() {
        auto locations = m_DataBase.GetFromMemory<Location>();

        if (locations.empty()) {
            std::cout << "No Locations in memory!" << std::endl;
            return;
        }

        for (const auto& location : locations)
            std::cout << location << std::endl;

        int locationId;

        std::cout << "Enter Location id:" << std::endl;
        std::cin >> locationId;

        auto&       location = m_DataBase.GetFromMemory<Location>(locationId);
        std::string name;
        float       area;
        int         monstersMaxCount;

        std::cout << "Enter location name:" << std::endl;
        std::cin >> name;

        std::cout << "Enter location area:" << std::endl;
        std::cin >> area;

        std::cout << "Enter location monstersMaxCount:" << std::endl;
        std::cin >> monstersMaxCount;

        location.SetName(std::move(name));
        location.SetArea(area);
        location.SetMonstersMaxCount(monstersMaxCount);

        m_DataBase.Update(location);
    }

    void LocationManager_Impl::UpdateInFile() {
        auto locations = m_DataBase.GetFromFile<Location>();

        if (locations.empty()) {
            std::cout << "No Locations in file!" << std::endl;
            return;
        }

        for (const auto& location : locations)
            std::cout << *location << std::endl;

        int locationId;

        std::cout << "Enter Location id:" << std::endl;
        std::cin >> locationId;

        auto        location = m_DataBase.GetFromFile<Location>(locationId);
        std::string name;
        float       area;
        int         monstersMaxCount;

        std::cout << "Get location: " << location << std::endl;

        std::cout << "Enter location name:" << std::endl;
        std::cin >> name;

        std::cout << "Enter location area:" << std::endl;
        std::cin >> area;

        std::cout << "Enter location monstersMaxCount:" << std::endl;
        std::cin >> monstersMaxCount;

        location.SetName(std::move(name));
        location.SetArea(area);
        location.SetMonstersMaxCount(monstersMaxCount);

        m_DataBase.Update(location);
    }
    
    void LocationManager_Impl::SearchInMemory() {
        
    }
    
    void LocationManager_Impl::SearchInFile() {
        
    }

} // namespace RefactoredProject