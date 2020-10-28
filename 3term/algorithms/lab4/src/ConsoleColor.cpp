#include "ConsoleColor.hpp"

#include <utility>

namespace Ng::Console {

    // static methods
    std::ostream& Color::Reset(std::ostream& stream) { return Paint(stream, ForeGround::Default); }

    std::ostream& Color::Default(std::ostream& stream) { return Paint(stream, ForeGround::Default); }

    std::ostream& Color::Black(std::ostream& stream) { return Paint(stream, ForeGround::Black); }

    std::ostream& Color::Red(std::ostream& stream) { return Paint(stream, ForeGround::Red); }

    std::ostream& Color::Green(std::ostream& stream) { return Paint(stream, ForeGround::Green); }

    std::ostream& Color::Yellow(std::ostream& stream) { return Paint(stream, ForeGround::Yellow); }

    std::ostream& Color::Blue(std::ostream& stream) { return Paint(stream, ForeGround::Blue); }

    std::ostream& Color::Magenta(std::ostream& stream) { return Paint(stream, ForeGround::Magenta); }

    std::ostream& Color::Cyan(std::ostream& stream) { return Paint(stream, ForeGround::Cyan); }

    std::ostream& Color::White(std::ostream& stream) { return Paint(stream, ForeGround::White); }

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

    std::ostream& Color::Paint(std::ostream& stream, const ForeGround& foreground) {
    #if defined(NG_OS_WINDOWS)
        system("");
    #endif

        return stream << "\033[" << static_cast<int>(Style::Default) << ";"
                      << static_cast<int>(foreground) << ";"
                      << static_cast<int>(BackGround::Default) << "m";
    }

} // namespace Ng