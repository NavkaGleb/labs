#pragma once

#include <iostream>
#include <cmath>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Mouse.hpp>

namespace ng::gui {

    ////////////////////////////////////////////////////////////////////////////////////////////
    /// Header
    ////////////////////////////////////////////////////////////////////////////////////////////
    template <typename T, typename PointerShape>
    class Slider : public sf::Drawable {
    public:
        // constructor / destructor
        Slider(const T& minValue, const T& maxValue);
        ~Slider() override = default;

        // accessors
        [[nodiscard]] inline const T& getMinValue() const { return m_minValue; }
        [[nodiscard]] inline const T& getMaxValue() const { return m_maxValue; }
        [[nodiscard]] inline const T& getValue() const { return m_value; }
        [[nodiscard]] inline bool isHovered() const { return m_status == Status::Hover; }
        [[nodiscard]] inline bool isPressed() const { return m_status == Status::Active; }

        // modifiers
        void setValue(const T& value);
        void setPosition(float x, float y);
        void setSize(const sf::Vector2f& size);
        void setFont(const sf::Font& font);
        void setBackgroundColor(const sf::Color& color);
        void setForegroundColor(const sf::Color& color);
//        void setText(const sf::String& text)

        // public methods
        void update(const sf::Vector2i& mousePosition);

    private:
        // member enums
        enum class Status { Idle = 0, Hover, Active };

        // inner struct

        // member data
        T m_minValue;
        T m_maxValue;
        T m_value;
        sf::RectangleShape m_backRect;
        sf::RectangleShape m_frontRect;
        PointerShape m_pointer;
        Status m_status;

        // member methods
        void draw(sf::RenderTarget& target, sf::RenderStates) const override;

    }; // class Slider

    ////////////////////////////////////////////////////////////////////////////////////////////
    /// Source
    ////////////////////////////////////////////////////////////////////////////////////////////
    // constructor / destructor
    template <typename T, typename PointerShape>
    Slider<T, PointerShape>::Slider(const T& minValue, const T& maxValue)
        : m_minValue(minValue),
          m_maxValue(maxValue),
          m_value(minValue) {



    }

    // modifiers
    template <typename T, typename PointerShape>
    void Slider<T, PointerShape>::setValue(const T& value) {
        m_value = value;
    }

    template <typename T, typename PointerShape>
    void Slider<T, PointerShape>::setPosition(float x, float y) {
        m_backRect.setPosition(x, y);
        m_frontRect.setPosition(x, y);
        m_pointer.setPosition(x, y);
    }

    template <typename T, typename PointerShape>
    void Slider<T, PointerShape>::setSize(const sf::Vector2f& size) {
        m_backRect.setSize(size);
        m_frontRect.setSize(sf::Vector2f(
            std::floor(static_cast<float>(m_value) / static_cast<float>(m_maxValue - m_minValue) * size.x),
            size.y
        ));
        m_pointer.setSize(sf::Vector2f(size.y, size.y));
        m_pointer.setPosition(
            m_pointer.getPosition().x + std::floor(static_cast<float>(m_value) / static_cast<float>(m_maxValue - m_minValue) * size.x),
            m_pointer.getPosition().y
        );
    }

    template <typename T, typename PointerShape>
    void Slider<T, PointerShape>::setFont(const sf::Font& font) {

    }

    template <typename T, typename PointerShape>
    void Slider<T, PointerShape>::setBackgroundColor(const sf::Color& color) {
        m_backRect.setFillColor(color);
    }

    template <typename T, typename PointerShape>
    void Slider<T, PointerShape>::setForegroundColor(const sf::Color& color) {
        m_frontRect.setFillColor(color);
    }
    
    // public methods

    template <typename T, typename PointerShape>
    void Slider<T, PointerShape>::update(const sf::Vector2i& mousePosition) {
        m_status = Status::Idle;

        if (m_pointer.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
            m_status = Status::Hover;

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                m_status = Status::Active;
        }

//        switch (status) {
//            case Status::Idle:
//                status.setFillColor(m_backgroundIdleColor);
//                break;
//
//            case Status::Hover:
//                status.setFillColor(m_backgroundHoverColor);
//                break;
//
//            case Status::Active:
//                status.setFillColor(m_backgroundActiveColor);
//        }
    }

    // member methods
    template <typename T, typename PointerShape>
    void Slider<T, PointerShape>::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(m_backRect, states);
        target.draw(m_frontRect, states);
        target.draw(m_pointer, states);
    }

} // namespace ng::gui