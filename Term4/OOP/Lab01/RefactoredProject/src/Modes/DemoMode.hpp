#pragma once

#include "IMode.hpp"

namespace RefactoredProject {

    class DemoMode : public IMode {
    public:
        DemoMode() = default;
        ~DemoMode() noexcept override = default;

        void Run();

    }; // class DemoMode

} // namespace RefactoredProject