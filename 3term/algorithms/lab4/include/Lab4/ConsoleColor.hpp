#pragma once

#include <ostream>

namespace Ng::Console {

    class Color {
    public:
        // enums
        enum class ForeGround { Default = 39, Black = 30, Red, Green, Yellow, Blue, Magenta, Cyan, White };
        enum class BackGround { Default = 49, Black = 40, Red, Green, Yellow, Blue, Magenta, Cyan, White };
        enum class Style { Default = 0, Bold = 1, Italic = 3, UnderLine = 4 };

        // static methods
        static Color Reset();
        static Color Default();
        static Color Black();
        static Color Red();
        static Color Green();
        static Color Yellow();
        static Color Blue();
        static Color Magenta();
        static Color Cyan();
        static Color White();

        // constructor
        Color() : foreground(ForeGround::Default), background(BackGround::Default), style(Style::Default) {}
        explicit Color(const ForeGround& _foreground, const BackGround& _background = BackGround::Default,
                       const Style& _style = Style::Default)
                            : foreground(_foreground), background(_background), style(_style) {}
        Color(const ForeGround& _foreground, const Style& _style)
            : foreground(_foreground), background(BackGround::Default), style(_style) {}

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
        ForeGround foreground;
        BackGround background;
        Style style;
    };

} // namespace Ng