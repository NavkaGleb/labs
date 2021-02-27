#pragma once

#include "IMode.hpp"

namespace RefactoredProject {

    class InteractiveMode_Impl : public IMode {
    public:
        void Run() override;

        friend class Singleton<InteractiveMode_Impl>;

    private:
        InteractiveMode_Impl() = default;
        ~InteractiveMode_Impl() noexcept override = default;

    }; // class InteractiveMode

    using InteractiveMode = Singleton<InteractiveMode_Impl>;

} // namespace RefactoredProject