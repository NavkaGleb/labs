#include "ConsoleColor.hpp"

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
    const Color::ForeGround& Color::F() const { return m_Foreground; }

    const Color::BackGround& Color::B() const { return m_Background; }

    const Color::Style& Color::S() const { return m_Style; }

    // modifiers
    void Color::F(const Color::ForeGround& _foreground) { m_Foreground = _foreground; }

    void Color::B(const BackGround& _background) { m_Background = _background; }

    void Color::S(const Style& _style) { m_Style = _style; }

    // operators
    Color& Color::operator=(const Color& other) {
        if (this != &other) {
            m_Foreground = other.m_Foreground;
            m_Background = other.m_Background;
            m_Style      = other.m_Style;
        }

        return *this;
    }

    Color& Color::operator=(Color&& other) noexcept {
        if (this != &other) {
            m_Foreground = std::exchange(m_Foreground, ForeGround::Default);
            m_Background = std::exchange(m_Background, BackGround::Default);
            m_Style = std::exchange(m_Style, Style::Default);
        }

        return *this;
    }

    std::ostream& operator<<(std::ostream& stream, const Color& color) {
    #if defined(NG_OS_WINDOWS)
        system("");
    #endif

        return stream << "\033[" << static_cast<int>(color.m_Style) << ";"
                                 << static_cast<int>(color.m_Foreground) << ";"
                                 << static_cast<int>(color.m_Background) << "m";
    }

} // namespace Ng