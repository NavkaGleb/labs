#pragma once

#include "NonCopyable.hpp"
#include "NonMoveable.hpp"

#include <sstream>

namespace RefactoredProject {

    template <typename T>
    class Singleton : public NonCopyable, public NonMoveable {
    public:
        template <typename... Args>
        static T& Create(Args&&... args) {
            if (s_Instance) {
                std::stringstream ss;
                ss << "RefactoredProject::Singleton<" << typeid(T).name() << ">::Create: s_Instance is not nullptr!";
                throw std::runtime_error(ss.str());
            }

            return *(s_Instance = new T(std::forward<Args>(args)...));
        }

        static void Destroy() {
            delete s_Instance;
            s_Instance = nullptr;
        }

        static inline T& Get() {
            if (!s_Instance) {
                std::stringstream ss;
                ss << "Engine::Singleton::Get: s_Instance(";
                ss << typeid(T).name();
                ss << ") is nullptr!";

                throw std::runtime_error(ss.str());
            }

            return *s_Instance;
        }

        static inline T* GetPointer() { return s_Instance; }

    protected:
        Singleton() = default;
        ~Singleton() override = default;

        static inline T* s_Instance = nullptr;

    }; // class Singleton

} // namespace RefactoredProject