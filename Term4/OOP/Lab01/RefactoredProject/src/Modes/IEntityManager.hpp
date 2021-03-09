#pragma once

#include <functional>

#include "DataBase/DataBase.hpp"

namespace RefactoredProject {

    class IEntityManager {
    public:
        enum class CountType : int { Single = 0, Multiple };
        void Run(int command);

    protected:
        IEntityManager(std::istream& istream, std::ostream& ostream);
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

        template <typename T>
        void Get(std::function<void(T&)>&& inputFunc) {
            inputFunc();
        }

    protected:
        DataBase::TypeRef m_DataBase;
        std::istream&     m_InputStream;
        std::ostream&     m_OutputStream;

    }; // class IEntityManager
    }

} // namespace RefactoredProject