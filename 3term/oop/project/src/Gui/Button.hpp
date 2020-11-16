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
        // constructor / destructor
        Button();
        ~Button() override = default;

        // accessors
        [[nodiscard]] inline const sf::Vector2f& getSize() const { return m_rect.getSize(); }
        [[nodiscard]] inline const sf::String& getText() const { return m_text.getString(); }

        // modifiers
        void setPosition(float x, float y);
        void setPosition(const sf::Vector2f& position);
        void setSize(const sf::Vector2f& size);
        void setText(const sf::String& string);
        void setFont(const sf::Font& font);
        void setCharacterSize(unsigned size);
        void setBackgroundColor(const sf::Color& color);

        // public methods
        void update(const float& ftime);

    private:
        // member data
        sf::RectangleShape m_rect;
        sf::Text m_text;
        sf::Color m_backgroundColor;

        // member methods
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    }; // class Button

} // namespace ng