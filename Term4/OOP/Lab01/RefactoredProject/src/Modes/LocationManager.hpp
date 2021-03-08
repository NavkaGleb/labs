#pragma once

#include "DataBase/DataBase.hpp"

namespace RefactoredProject {

    class LocationManager_Impl {
    public:
        void Run(int command);

        friend class Singleton<LocationManager_Impl>;

    private:
        enum class CountType : int { Single = 0, Multiple };

        LocationManager_Impl();
        virtual ~LocationManager_Impl() = default;

        void Create();
        void CreateRandom();
        bool PrintFromMemory();
        bool PrintFromFile();
        void Load();
        void Save();
        void DeleteFromMemory(CountType countType);
        void DeleteFromFile(CountType countType);
        void Truncate();
        void UpdateInMemory();
        void UpdateInFile();
        void SearchInMemory();
        void SearchInFile();

    private:
        DataBase::TypeRef m_DataBase;
        int               m_Command;

    }; // LocationManager

    using LocationManager = Singleton<LocationManager_Impl>;

} // RefactoredProject