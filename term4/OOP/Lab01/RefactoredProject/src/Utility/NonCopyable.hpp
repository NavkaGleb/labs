#pragma once

namespace RefactoredProject {

    class NonCopyable {
    public:
        NonCopyable() = default;
        NonCopyable(const NonCopyable& other) noexcept = delete;
        virtual ~NonCopyable() noexcept = default;

        NonCopyable& operator =(const NonCopyable& other) = delete;

    }; // class NonCopyable

} // namespace RefactoredProject