#include "LocationManager.hpp"

#include <Random/Random.hpp>

#include "Entities/Location.hpp"

namespace RefactoredProject {
        
    LocationManager_Impl::LocationManager_Impl(std::istream& istream, std::ostream& ostream)
        : IEntityManager(istream, ostream){}
    
    void LocationManager_Impl::Create() {
        auto&         location = m_DataBase.Create<Location>();
        LocationProps props;

        m_OutputStream << "Enter location name:" << std::endl;
        Get(props.Name);

        m_OutputStream << "Enter location area:" << std::endl;
        m_InputStream >> props.Area;

        m_OutputStream << "Enter location monstersMaxCount:" << std::endl;
        m_InputStream >> props.MonstersMaxCount;

        location.SetProps(std::move(props));

        m_OutputStream << "Location " << location.GetName() << " was successfully created!" << std::endl;
    }

    void LocationManager_Impl::CreateRandom() {
        int n;

        m_OutputStream << "Enter amount of locations:" << std::endl;
        m_InputStream >> n;

        for (int i = 0; i < n; ++i) {
            auto& location = m_DataBase.Create<Location>();

            location.SetProps({
                Ng::Random::GetString(Ng::Random::Get(2, 7)),
                Ng::Random::Get(10.0f, 500.0f),
                Ng::Random::Get(5, 100)
            });
        }

        m_OutputStream << n << " Locations was successfully added" << std::endl;
    }

    bool LocationManager_Impl::PrintFromMemory() {
        auto locations = m_DataBase.GetFromMemory<Location>();

        if (locations.empty()) {
            m_OutputStream << "No Locations in memory!" << std::endl;
            return false;
        }

        for (const auto& location : locations)
            m_OutputStream << *location << std::endl;

        return true;
    }

    bool LocationManager_Impl::PrintFromFile() {
        auto locations = m_DataBase.GetFromFile<Location>();

        if (locations.empty()) {
            m_OutputStream << "No Locations in file!" << std::endl;
            return false;
        }

        for (const auto& location : locations)
            m_OutputStream << *location << std::endl;

        return true;
    }

    void LocationManager_Impl::Load() {
        m_DataBase.Load<Location>();
        m_OutputStream << "Locations was successfully loaded from file" << std::endl;
    }

    void LocationManager_Impl::Save() {
        m_DataBase.Save<Location>();
        m_OutputStream << "Locations was successfully saved to file" << std::endl;
    }

    void LocationManager_Impl::DeleteFromMemory(CountType countType) {
        if (countType == CountType::Single) {
            if (!PrintFromMemory())
                m_OutputStream << "No Locations in memory!" << std::endl;

            int locationId;

            m_OutputStream << "Enter locationId:" << std::endl;
            m_InputStream >> locationId;

            m_DataBase.DeleteFromMemory<Location>(locationId);

            m_OutputStream << "Location " << locationId << " was successfully deleted from memory!" << std::endl;
        } else {
            m_DataBase.DeleteFromMemory<Location>();

            m_OutputStream << "Locations was successfully deleted from memory!" << std::endl;
        }
    }

    void LocationManager_Impl::DeleteFromFile(CountType countType) {
        if (countType == CountType::Single) {
            if (!PrintFromFile())
                return;

            int locationId;

            m_OutputStream << "Enter locationId:" << std::endl;
            m_InputStream >> locationId;

            m_DataBase.DeleteFromFile<Location>(locationId);

            m_OutputStream << "Location " << locationId << " was successfully deleted from file!" << std::endl;
        } else {
            m_DataBase.DeleteFromFile<Location>();

            m_OutputStream << "Locations was successfully deleted from file!" << std::endl;
        }
    }
    
    void LocationManager_Impl::Truncate() {
        m_DataBase.Truncate<Location>();
    }

    void LocationManager_Impl::UpdateInMemory() {
        if (!PrintFromMemory())
            return;

        int locationId;

        m_OutputStream << "Enter Location id:" << std::endl;
        m_InputStream >> locationId;

        auto&         location = m_DataBase.GetFromMemory<Location>(locationId);
        LocationProps props;

        m_OutputStream << "Enter location name:" << std::endl;
        m_InputStream >> props.Name;

        m_OutputStream << "Enter location area:" << std::endl;
        m_InputStream >> props.Area;

        m_OutputStream << "Enter location monstersMaxCount:" << std::endl;
        m_InputStream >> props.MonstersMaxCount;

        location.SetProps(std::move(props));

        m_DataBase.Update<Location>(location);

        m_OutputStream << "Location " << location.GetId() << " was successfully updated" << std::endl;
    }

    void LocationManager_Impl::UpdateInFile() {
        if (!PrintFromFile())
            return;

        int locationId;

        m_OutputStream << "Enter Location id:" << std::endl;
        m_InputStream >> locationId;

        auto          location = m_DataBase.GetFromFile<Location>(locationId);
        LocationProps props;

        m_OutputStream << "Get location: " << location << std::endl;

        m_OutputStream << "Enter location name:" << std::endl;
        m_InputStream >> props.Name;

        m_OutputStream << "Enter location area:" << std::endl;
        m_InputStream >> props.Area;

        m_OutputStream << "Enter location monstersMaxCount:" << std::endl;
        m_InputStream >> props.MonstersMaxCount;

        location.SetProps(std::move(props));

        m_DataBase.Update<Location>(location);

        m_OutputStream << "Location " << location.GetId() << " was successfully updated" << std::endl;
    }
    
    void LocationManager_Impl::SearchInMemory() {
        
    }
    
    void LocationManager_Impl::SearchInFile() {
        
    }

} // namespace RefactoredProject