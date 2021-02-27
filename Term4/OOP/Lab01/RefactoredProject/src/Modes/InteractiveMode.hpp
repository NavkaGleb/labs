#pragma once

#include "IMode.hpp"

namespace RefactoredProject {

    class InteractiveMode : public IMode {
    public:
        InteractiveMode() = default;
        ~InteractiveMode() noexcept override = default;

        void Run();

    }; // class InteractiveMode

} // namespace RefactoredProject