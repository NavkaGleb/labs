#pragma once

#include "IEntityManager.hpp"

namespace RefactoredProject {

    class LocationManager_Impl : public IEntityManager {
    public:
        friend class Singleton<LocationManager_Impl>;

    private:
        LocationManager_Impl() = default;
        ~LocationManager_Impl() noexcept override = default;

        void Create() override;
        void CreateRandom() override;
        bool PrintFromMemory() override;
        bool PrintFromFile() override;
        void Load() override;
        void Save() override;
        void DeleteFromMemory(CountType countType) override;
        void DeleteFromFile(CountType countType) override;
        void Truncate() override;
        void UpdateInMemory() override;
        void UpdateInFile() override;
        void SearchInMemory() override;
        void SearchInFile() override;

    }; // LocationManager_Impl

    using LocationManager = Singleton<LocationManager_Impl>;

} // RefactoredProject