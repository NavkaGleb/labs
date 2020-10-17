#include "Lab4/ConsoleColor.hpp"

#include <utility>

namespace Ng::Console {

    // static methods
    Color Color::Reset() { return Color(ForeGround::Default); }

    Color Color::Default() { return Color(ForeGround::Default); }

    Color Color::Black() { return Color(ForeGround::Black); }

    Color Color::Red() { return Color(ForeGround::Red); }

    Color Color::Green() { return Color(ForeGround::Green); }

    Color Color::Yellow() { return Color(ForeGround::Yellow); }

    Color Color::Blue() { return Color(ForeGround::Blue); }

    Color Color::Magenta() { return Color(ForeGround::Magenta); }

    Color Color::Cyan() { return Color(ForeGround::Cyan); }

    Color Color::White() { return Color(ForeGround::White); }

    // accessors
    const Color::ForeGround& Color::F() const { return this->foreground; }

    const Color::BackGround& Color::B() const { return this->background; }

    const Color::Style& Color::S() const { return this->style; }

    // modifiers
    void Color::F(const Color::ForeGround& _foreground) { this->foreground = _foreground; }

    void Color::B(const BackGround& _background) { this->background = _background; }

    void Color::S(const Style& _style) { this->style = _style; }

    // operators
    Color& Color::operator=(const Color& other) {
        if (this != &other) {
            this->foreground = other.foreground;
            this->background = other.background;
            this->style = other.style;
        }

        return *this;
    }

    Color& Color::operator=(Color&& other) noexcept {
        if (this != &other) {
            this->foreground = std::exchange(other.foreground, ForeGround::Default);
            this->background = std::exchange(other.background, BackGround::Default);
            this->style = std::exchange(other.style, Style::Default);
        }

        return *this;
    }

    std::ostream& operator<<(std::ostream& stream, const Color& color) {
        return stream << "\033[" << static_cast<int>(color.style) << ";"
                                 << static_cast<int>(color.foreground) << ";"
                                 << static_cast<int>(color.background) << "m";
    }

} // namespace Ng