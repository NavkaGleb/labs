#pragma once

#include "Utility/Singleton.hpp"

namespace RefactoredProject {

    class Application_Impl {
    public:
        void Run();

        friend class Singleton<Application_Impl>;

    private:
        enum class ResponseType : int { None = 0, Yes, No, Quit };

        Application_Impl();
        ~Application_Impl() noexcept;

    }; // class Application

    using Application = Singleton<Application_Impl>;

} // namespace RefactoredProject