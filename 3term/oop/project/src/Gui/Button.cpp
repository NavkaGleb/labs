#include "Button.hpp"

#include <cmath>
#include <iostream>

namespace ng::gui {

    // constructor / destructor
    Button::Button() {

    }

    // modifiers
    void Button::setPosition(float x, float y) {
        m_rect.setPosition(x, y);
        m_text.setPosition(x, y);
        updateLocalBounds();
    }

    void Button::setPosition(const sf::Vector2f& position) {
        m_rect.setPosition(position);
        m_text.setPosition(position);
        updateLocalBounds();
    }

    void Button::setSize(const sf::Vector2f& size) {
        m_rect.setSize(size);
        updateLocalBounds();
    }

    void Button::setFont(const sf::Font& font) {
        m_text.setFont(font);
        updateLocalBounds();
    }

    void Button::setText(const sf::String& string) {
        m_text.setString(string);
        updateLocalBounds();
    }

    void Button::setTextHorizontalAlign(const HorizontalAlign& align) {
        m_horizontalAlign = align;
        updateHorizontalAlign();
    }

    void Button::setTextVerticalAlign(const VerticalAlign& align) {
        m_verticalAlign = align;
        updateVerticalAlign();
    }

    void Button::setCharacterSize(unsigned size) {
        m_text.setCharacterSize(size);
        updateLocalBounds();
    }

    void Button::setBackgroundColor(const sf::Color& color) {
        m_backgroundColor = color;
        m_rect.setFillColor(m_backgroundColor);
    }

    void Button::setTextColor(const sf::Color& color) {
        m_text.setFillColor(color);
    }

    // public methods
    void Button::update(const float& ftime) {

    }

    // member methods
    void Button::updateHorizontalAlign() {
        float buttonWidth = m_rect.getGlobalBounds().width;
        float textWidth = m_text.getGlobalBounds().width;

        switch (m_horizontalAlign) {
            case HorizontalAlign::Left:
                m_text.setPosition(
                    std::floor(m_rect.getPosition().x),
                    std::floor(m_text.getPosition().y)
                );
                break;

            case HorizontalAlign::Center:
                m_text.setPosition(
                    std::floor(m_rect.getPosition().x + (buttonWidth - textWidth) / 2.0f),
                    std::floor(m_text.getPosition().y)
                );
                break;

            case HorizontalAlign::Right:
                m_text.setPosition(
                    std::floor(m_rect.getPosition().x + buttonWidth - textWidth),
                    std::floor(m_text.getPosition().y)
                );
                break;
        }
    }

    void Button::updateVerticalAlign() {
        float buttonHeight = m_rect.getGlobalBounds().height;
        float textHeight = m_text.getGlobalBounds().height;

        std::string string = m_text.getString();
        unsigned characterSize = m_text.getCharacterSize();
        const sf::Font* font = m_text.getFont();
        bool bold = (m_text.getStyle() & sf::Text::Bold);
        float maxHeight = 0;
        float minHeight = 0;
        float actualTextHeight;

        for (char character : string) {
            float height = font->getGlyph(character, characterSize, bold).bounds.height;

            maxHeight = std::max(maxHeight, height);
            minHeight = std::min(minHeight, height);
        }

        actualTextHeight = (maxHeight + minHeight) / 2;

        switch (m_verticalAlign) {
            case VerticalAlign::Top:
                m_text.setPosition(
                    std::floor(m_text.getPosition().x),
                    std::floor(m_rect.getPosition().y)
                );
                break;

            case VerticalAlign::Center:
                m_text.setPosition(
                    std::floor(m_text.getPosition().x),
                    std::floor(m_rect.getPosition().y + (
                        buttonHeight - actualTextHeight + textHeight - static_cast<float>(characterSize)
                    ) / 2.0f - textHeight + actualTextHeight)
                );
                break;

            case VerticalAlign::Bottom:
                m_text.setPosition(
                    std::floor(m_text.getPosition().x),
                    std::floor(m_rect.getPosition().y + buttonHeight - actualTextHeight - textHeight)
                );
                break;
        }
    }

    void Button::updateLocalBounds() {
        updateHorizontalAlign();
        updateVerticalAlign();
    }

    void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(m_rect, states);
        target.draw(m_text, states);
    }

} // namespace ng