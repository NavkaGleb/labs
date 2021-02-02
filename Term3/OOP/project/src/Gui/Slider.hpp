#pragma once

#include <iostream>
#include <cmath>
#include <unordered_map>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Mouse.hpp>

namespace ng::gui {

    class Slider : public sf::Drawable {
    public:
        // constructor / destructor
        Slider(float minValue, float maxValue, float step);
        ~Slider() override = default;

        // accessors
        [[nodiscard]] inline float getMinValue() const { return m_minValue; }
        [[nodiscard]] inline float getMaxValue() const { return m_maxValue; }
        [[nodiscard]] inline float getValue() const { return m_value; }
        [[nodiscard]] inline float getStep() const { return m_step; }
        [[nodiscard]] inline bool isHovered() const { return m_status == Status::Hover; }
        [[nodiscard]] inline bool isPressed() const { return m_status == Status::Active; }

        // modifiers
        void setValue(float value);
        void setPosition(float x, float y);
        void setSize(const sf::Vector2f& size);
        void setPointerRadius(float radius);
        void setBackgroundColor(const sf::Color& color);
        void setForegroundColor(const sf::Color& color);
        void setPointerIdleColor(const sf::Color& color);
        void setPointerHoverColor(const sf::Color& color);
        void setPointerActiveColor(const sf::Color& color);

        // public methods
        void movePointer(float x);
        void update(const sf::Vector2i& mousePosition);

    private:
        // member enums
        enum class Status    : short { Idle = 0, Hover, Active };
        enum class ColorRole : short { Background = 0, Foreground, PointerIdle, PointerHover, PointerActive };

        // aliases
        using ColorContainer = std::unordered_map<ColorRole, sf::Color>;

        // member data
        float m_minValue;
        float m_maxValue;
        float m_value;
        float m_step;
        sf::RectangleShape m_backRect;
        sf::RectangleShape m_frontRect;
        sf::CircleShape m_pointer;
        Status m_status;
        ColorContainer m_colors;

        // member methods
        void updateValue();
        void updateFrontRect();
        void draw(sf::RenderTarget& target, sf::RenderStates) const override;

    }; // class Slider

} // namespace ng::gui