#pragma once

#include "DataBase/DataBase.hpp"

namespace RefactoredProject {

    class IEntityManager {
    public:
        enum class CountType : int { Single = 0, Multiple };
        void Run(int command);

    protected:
        IEntityManager();
        virtual ~IEntityManager() noexcept = default;

        virtual void Create()                              = 0;
        virtual void CreateRandom()                        = 0;
        virtual bool PrintFromMemory()                     = 0;
        virtual bool PrintFromFile()                       = 0;
        virtual void Load()                                = 0;
        virtual void Save()                                = 0;
        virtual void DeleteFromMemory(CountType countType) = 0;
        virtual void DeleteFromFile(CountType countType)   = 0;
        virtual void Truncate()                            = 0;
        virtual void UpdateInMemory()                      = 0;
        virtual void UpdateInFile()                        = 0;
        virtual void SearchInMemory()                      = 0;
        virtual void SearchInFile()                        = 0;

    protected:
        DataBase::TypeRef m_DataBase;

    }; // class IEntityManager

} // namespace RefactoredProject