#pragma once

#include <fstream>

#include "IMode.hpp"

namespace RefactoredProject {

    class DemoMode_Impl : public IMode {
    public:
        void Run() override;

        friend class Singleton<DemoMode_Impl>;

    private:
        DemoMode_Impl();
        ~DemoMode_Impl() noexcept override = default;

        void UpdateCommand();
        [[nodiscard]] bool ParseCommand() const;

    private:
        std::basic_ifstream<char> m_InputStream;
        int                       m_Command;

    }; // class DemoMode

    using DemoMode = Singleton<DemoMode_Impl>;

} // namespace RefactoredProject