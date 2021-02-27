#pragma once

#include "IMode.hpp"

namespace RefactoredProject {

    class BenchmarkMode_Impl : public IMode {
    public:
        void Run();

        friend class Singleton<BenchmarkMode_Impl>;

    private:
        BenchmarkMode_Impl() = default;
        ~BenchmarkMode_Impl() noexcept override = default;

    }; // class BenchmarkMode

    using BenchmarkMode = Singleton<BenchmarkMode_Impl>;

} // namespace RefactoredProject