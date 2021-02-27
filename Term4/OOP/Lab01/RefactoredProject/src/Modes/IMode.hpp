#pragma once

#include "Utility/Singleton.hpp"

namespace RefactoredProject {

    class IMode {
    public:
        IMode() = default;
        virtual ~IMode() noexcept = default;

        virtual void Run() = 0;

    }; // class IMode

} // namespace RefactoredProject