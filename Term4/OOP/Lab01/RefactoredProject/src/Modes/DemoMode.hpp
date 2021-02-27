#pragma once

#include "IMode.hpp"

namespace RefactoredProject {

    class DemoMode_Impl : public IMode {
    public:
        void Run() override;

        friend class Singleton<DemoMode_Impl>;

    private:
        DemoMode_Impl() = default;
        ~DemoMode_Impl() noexcept override = default;

    }; // class DemoMode

    using DemoMode = Singleton<DemoMode_Impl>;

} // namespace RefactoredProject