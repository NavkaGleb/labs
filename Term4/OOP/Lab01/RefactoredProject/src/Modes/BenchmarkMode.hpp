#pragma once

#include "IMode.hpp"

namespace RefactoredProject {

    class BenchmarkMode : public IMode {
    public:
        BenchmarkMode() = default;
        ~BenchmarkMode() noexcept override = default;

        void Run();

    }; // class BenchmarkMode

} // namespace RefactoredProject