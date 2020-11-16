#include "Button.hpp"

namespace ng::gui {

    // constructor / destructor
    Button::Button() {

    }

    // modifiers
    void Button::setPosition(float x, float y) {
        m_rect.setPosition(x, y);
        m_text.setPosition(x, y);
    }

    void Button::setPosition(const sf::Vector2f& position) {
        m_rect.setPosition(position);
        m_text.setPosition(position);
    }

    void Button::setSize(const sf::Vector2f& size) {
        m_rect.setSize(size);
    }

    void Button::setText(const sf::String& string) {
        m_text.setString(string);
    }

    void Button::setFont(const sf::Font& font) {
        m_text.setFont(font);
    }

    void Button::setCharacterSize(unsigned size) {
        m_text.setCharacterSize(size);
    }

    void Button::setBackgroundColor(const sf::Color& color) {
        m_backgroundColor = color;
        m_rect.setFillColor(m_backgroundColor);
    }

    // public methods
    void Button::update(const float& ftime) {

    }

    // member methods
    void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(m_rect, states);
        target.draw(m_text, states);
    }

} // namespace ng