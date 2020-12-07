#include "Slider.hpp"

namespace ng::gui {

// constructor / destructor
    
    Slider::Slider(float minValue, float maxValue, float step)
        : m_minValue(minValue),
          m_maxValue(maxValue),
          m_value(minValue),
          m_step(step),
          m_status(Status::Idle) {}

    // modifiers
    void Slider::setValue(float value) {
        m_value = value;
    }
    
    void Slider::setPosition(float x, float y) {
        m_backRect.setPosition(x, y);
        m_frontRect.setPosition(x, y);
        m_pointer.setPosition(x, y);
    }
    
    void Slider::setSize(const sf::Vector2f& size) {
        m_backRect.setSize(size);
        m_frontRect.setSize(sf::Vector2f(
            std::floor(static_cast<float>(m_value - m_minValue) / static_cast<float>(m_maxValue - m_minValue) * size.x),
            size.y
        ));
    }

    void Slider::setPointerRadius(float radius) {
        m_pointer.setRadius(radius);
        m_pointer.setOrigin(m_pointer.getRadius(), m_pointer.getRadius());
        m_pointer.setPosition(
            m_pointer.getPosition().x + std::floor(static_cast<float>(m_value - m_minValue) / static_cast<float>(m_maxValue - m_minValue) * m_backRect.getSize().x),
            m_pointer.getPosition().y + m_backRect.getSize().y / m_pointer.getRadius() * 4.0f
        );
    }
    
    void Slider::setFont(const sf::Font& font) {

    }

    void Slider::setBackgroundColor(const sf::Color& color) {
        m_colors[ColorRole::Background] = color;
        m_backRect.setFillColor(m_colors[ColorRole::Background]);
    }

    void Slider::setForegroundColor(const sf::Color& color) {
        m_colors[ColorRole::Foreground] = color;
        m_frontRect.setFillColor(m_colors[ColorRole::Foreground]);
    }

    void Slider::setPointerIdleColor(const sf::Color& color) {
        m_colors[ColorRole::PointerIdle] = color;
    }

    void Slider::setPointerHoverColor(const sf::Color& color) {
        m_colors[ColorRole::PointerHover] = color;
    }

    void Slider::setPointerActiveColor(const sf::Color& color) {
        m_colors[ColorRole::PointerActive] = color;
    }

    // public methods
    void Slider::movePointer(float x) {
        float sliderStep = m_step / (m_maxValue - m_minValue) * m_backRect.getSize().x;
        float leftLimit  = std::floor(x / sliderStep) * sliderStep;
        float rightLimit = std::floor(x / sliderStep + 1.0f) * sliderStep;

        x = (std::abs(leftLimit - x) < std::abs(rightLimit - x)) ? leftLimit : rightLimit;

        m_pointer.setPosition(
            std::min(
                m_backRect.getGlobalBounds().width + m_backRect.getGlobalBounds().left,
                std::max(m_backRect.getGlobalBounds().left, x)
            ),
            m_pointer.getPosition().y
        );

        updateValue();
        updateFrontRect();
    }

    
    void Slider::update(const sf::Vector2i& mousePosition) {
        if (m_status == Status::Active && sf::Mouse::isButtonPressed(sf::Mouse::Left))
            return;

        m_status = Status::Idle;

        if (m_pointer.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
            m_status = Status::Hover;

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                m_status = Status::Active;
        }

        switch (m_status) {
            case Status::Idle:
                m_pointer.setFillColor(m_colors[ColorRole::PointerIdle]);
                break;

            case Status::Hover:
                m_pointer.setFillColor(m_colors[ColorRole::PointerHover]);
                break;

            case Status::Active:
                m_pointer.setFillColor(m_colors[ColorRole::PointerActive]);
                break;
        }
    }

    // member methods
    void Slider::updateValue() {
        m_value = (m_pointer.getGlobalBounds().left - m_backRect.getGlobalBounds().left + m_pointer.getRadius()) /
                  m_backRect.getGlobalBounds().width *
                  (m_maxValue - m_minValue) + m_minValue;
    }

    void Slider::updateFrontRect() {
        m_frontRect.setSize(sf::Vector2f(
            std::floor(
                static_cast<float>(m_value - m_minValue) /
                static_cast<float>(m_maxValue - m_minValue) *
                m_backRect.getSize().x
            ),
            m_frontRect.getSize().y
        ));
    }

    void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(m_backRect, states);
        target.draw(m_frontRect, states);
        target.draw(m_pointer, states);
    }

} // namespace ng::gui