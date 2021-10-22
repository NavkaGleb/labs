#pragma once

#include <ostream>

#if defined(_WIN32) || defined(_WIN64)
    #define NG_OS_WINDOWS
#elif defined(__APPLE__)
    #define NG_OS_MACOS
#elif defined(__unix__) || defined(__unix)
    #define NG_OS_LINUX
#else
    #error unsupported platform
#endif

namespace Ng::Console {

    class Color {
    public:
        // enums
        enum class ForeGround { Default = 39, Black = 30, Red, Green, Yellow, Blue, Magenta, Cyan, White };
        enum class BackGround { Default = 49, Black = 40, Red, Green, Yellow, Blue, Magenta, Cyan, White };
        enum class Style { Default = 0, Bold = 1, Italic = 3, UnderLine = 4 };

        // static methods
        static std::ostream& Reset(std::ostream& stream);
        static std::ostream& Default(std::ostream& stream);
        static std::ostream& Black(std::ostream& stream);
        static std::ostream& Red(std::ostream& stream);
        static std::ostream& Green(std::ostream& stream);
        static std::ostream& Yellow(std::ostream& stream);
        static std::ostream& Blue(std::ostream& stream);
        static std::ostream& Magenta(std::ostream& stream);
        static std::ostream& Cyan(std::ostream& stream);
        static std::ostream& White(std::ostream& stream);

        // constructor
        Color() : m_Foreground(ForeGround::Default), m_Background(BackGround::Default), m_Style(Style::Default) {}
        explicit Color(const ForeGround& foreground, const BackGround& background = BackGround::Default,
                       const Style& style = Style::Default)
            : m_Foreground(foreground), m_Background(background), m_Style(style) {}
        Color(const ForeGround& foreground, const Style& style)
            : m_Foreground(foreground), m_Background(BackGround::Default), m_Style(style) {}

        // accessors
        [[nodiscard]] const ForeGround& F() const;
        [[nodiscard]] const BackGround& B() const;
        [[nodiscard]] const Style& S() const;

        // modifiers
        void F(const ForeGround& _foreground);
        void B(const BackGround& _background);
        void S(const Style& _style);

        // operators
        Color& operator=(const Color& other);
        Color& operator=(Color&& other) noexcept;
        friend std::ostream& operator<<(std::ostream& stream, const Color& color);

    private:
        // member data
        ForeGround m_Foreground;
        BackGround m_Background;
        Style m_Style;

        static std::ostream& Paint(std::ostream& stream, const ForeGround& foreground);
    };

} // namespace Ng