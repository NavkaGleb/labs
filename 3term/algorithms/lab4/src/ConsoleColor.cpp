#include "Lab4/ConsoleColor.hpp"

#include <utility>

namespace Ng::Console {

    // static member data
    const ForeGround ForeGround::Black   = ForeGround(ForeGround::Color::Black);
    const ForeGround ForeGround::Red     = ForeGround(ForeGround::Color::Red);
    const ForeGround ForeGround::Green   = ForeGround(ForeGround::Color::Green);
    const ForeGround ForeGround::Yellow  = ForeGround(ForeGround::Color::Yellow);
    const ForeGround ForeGround::Blue    = ForeGround(ForeGround::Color::Blue);
    const ForeGround ForeGround::Magenta = ForeGround(ForeGround::Color::Magenta);
    const ForeGround ForeGround::Cyan    = ForeGround(ForeGround::Color::Cyan);
    const ForeGround ForeGround::White   = ForeGround(ForeGround::Color::White);
    const ForeGround ForeGround::Default = ForeGround(ForeGround::Color::Default);

    const ForeGround ForeGround::BoldBlack   = ForeGround(ForeGround::Color::Black, ForeGround::Style::Bold);
    const ForeGround ForeGround::BoldRed     = ForeGround(ForeGround::Color::Red, ForeGround::Style::Bold);
    const ForeGround ForeGround::BoldGreen   = ForeGround(ForeGround::Color::Green, ForeGround::Style::Bold);
    const ForeGround ForeGround::BoldYellow  = ForeGround(ForeGround::Color::Yellow, ForeGround::Style::Bold);
    const ForeGround ForeGround::BoldBlue    = ForeGround(ForeGround::Color::Blue, ForeGround::Style::Bold);
    const ForeGround ForeGround::BoldMagenta = ForeGround(ForeGround::Color::Magenta, ForeGround::Style::Bold);
    const ForeGround ForeGround::BoldCyan    = ForeGround(ForeGround::Color::Cyan, ForeGround::Style::Bold);
    const ForeGround ForeGround::BoldWhite   = ForeGround(ForeGround::Color::White, ForeGround::Style::Bold);
    const ForeGround ForeGround::BoldDefault = ForeGround(ForeGround::Color::Default, ForeGround::Style::Bold);

    // operators
    ForeGround& ForeGround::operator=(const ForeGround& other) {
        if (this != &other) {
            this->color = other.color;
            this->style = other.style;
        }

        return *this;
    }

    ForeGround& ForeGround::operator=(ForeGround&& other) noexcept {
        if (this != &other) {
            this->color = std::exchange(other.color, Color::Default);
            this->style = std::exchange(other.style, Style::Default);
        }

        return *this;
    }

    std::ostream& operator<<(std::ostream& stream, const ForeGround& fore_ground) {
        return stream << "\033[" << static_cast<int>(fore_ground.style) << ";"
                                 << static_cast<int>(fore_ground.color) << "m";
    }

} // namespace Ng