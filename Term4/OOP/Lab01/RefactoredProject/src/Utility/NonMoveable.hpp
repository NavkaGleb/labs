#pragma once

namespace RefactoredProject {

    class NonMoveable {
    public:
        NonMoveable() = default;
        NonMoveable(NonMoveable&& other) noexcept = delete;
        virtual ~NonMoveable() noexcept = default;

        NonMoveable& operator =(NonMoveable&& other) = delete;

    }; // class NonMoveable

} // namespace RefactoredProject