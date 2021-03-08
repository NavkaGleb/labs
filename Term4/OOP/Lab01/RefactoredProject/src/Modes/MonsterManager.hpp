#pragma once

#include "IEntityManager.hpp"

namespace RefactoredProject {

    class MonsterManager_Impl : public IEntityManager {
    public:
        friend class Singleton<MonsterManager_Impl>;

    private:
        MonsterManager_Impl() = default;
        ~MonsterManager_Impl() noexcept override = default;

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

    using MonsterManager = Singleton<MonsterManager_Impl>;

} // namespace RefactoredProject