#pragma once

#include <ostream>

namespace Ng::Console {

    class ForeGround {
    public:
        // enums
        enum class Color { Default = 39, Black = 30, Red, Green, Yellow, Blue, Magenta, Cyan, White };
        enum class Style { Default = 0, Bold = 1 };

        // static member data
        static const ForeGround Black;
        static const ForeGround Red;
        static const ForeGround Green;
        static const ForeGround Yellow;
        static const ForeGround Blue;
        static const ForeGround Magenta;
        static const ForeGround Cyan;
        static const ForeGround White;
        static const ForeGround Default;

        static const ForeGround BoldBlack;
        static const ForeGround BoldRed;
        static const ForeGround BoldGreen;
        static const ForeGround BoldYellow;
        static const ForeGround BoldBlue;
        static const ForeGround BoldMagenta;
        static const ForeGround BoldCyan;
        static const ForeGround BoldWhite;
        static const ForeGround BoldDefault;

        // constructor
        ForeGround() : color(Color::Default), style(Style::Default) {}
        explicit ForeGround(const Color& _color, const Style& _style = Style::Default)
            : color(_color), style(_style) {}
        ForeGround(const ForeGround& other) : color(other.color), style(other.style) { *this = other; }

        // operators
        ForeGround& operator=(const ForeGround& other);
        ForeGround& operator=(ForeGround&& other) noexcept;
        friend std::ostream& operator<<(std::ostream& stream, const ForeGround& fore_ground);

    private:
        // member data
        Color color;
        Style style;
    };

} // namespace Ng