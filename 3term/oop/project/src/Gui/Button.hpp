#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Color.hpp>

namespace ng::gui {

    class Button final : public sf::Drawable {
    public:
        // enums
        enum class HorizontalAlign { Left = 0, Center, Right };
        enum class VerticalAlign { Top = 0, Center, Bottom };

        // constructor / destructor
        Button();
        ~Button() override = default;

        // accessors
        [[nodiscard]] inline const sf::Vector2f& getSize() const { return m_rect.getSize(); }
        [[nodiscard]] inline const sf::String& getText() const { return m_text.getString(); }
        [[nodiscard]] inline bool isPressed() const { return m_status == Status::Active; };
        [[nodiscard]] inline bool isHovered() const { return m_status == Status::Hover; }

        // modifiers
        void setPosition(float x, float y);
        void setPosition(const sf::Vector2f& position);
        void setSize(const sf::Vector2f& size);
        void setFont(const sf::Font& font);
        void setText(const sf::String& string);
        void setTextHorizontalAlign(const HorizontalAlign& align);
        void setTextVerticalAlign(const VerticalAlign& align);
        void setCharacterSize(unsigned size);
        void setBackgroundIdleColor(const sf::Color& color);
        void setBackgroundHoverColor(const sf::Color& color);
        void setBackgroundActiveColor(const sf::Color& color);
        void setTextColor(const sf::Color& color);

        // public methods
        void update(const sf::Vector2i& mousePosition);

    private:
        // member enums
        enum class Status { Idle = 0, Hover, Active };

        // member data
        sf::RectangleShape m_rect;
        sf::Text           m_text;
        sf::Color          m_backgroundIdleColor;
        sf::Color          m_backgroundHoverColor;
        sf::Color          m_backgroundActiveColor;
        HorizontalAlign    m_horizontalAlign;
        VerticalAlign      m_verticalAlign;
        Status             m_status;

        // member methods
        void updateHorizontalAlign();
        void updateVerticalAlign();
        void updateLocalBounds();
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    }; // class Button

} // namespace ng