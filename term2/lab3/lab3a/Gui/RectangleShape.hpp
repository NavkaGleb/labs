#pragma once

#include <SFML/Graphics.hpp>

namespace ng {

	class RectangleShape : public sf::Drawable {
	private:
		// variables
		sf::RectangleShape rect;

		// methods
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	public:
		// constructor
		RectangleShape() = default;

		// accessors
		sf::FloatRect getGlobalBounds() const;
		sf::Vector2f getPosition() const;

		// modifiers
		void setSize(const float& width, const float& height);
		void setSize(const sf::FloatRect& frect);
		void setPosition(const float& x, const float& y);
		void setXPosition(const float& x);
		void updateYPosition(const unsigned& height);
		void setFillColor(const sf::Color& color);

		// methods
		bool operator<(const RectangleShape& other) const;
		bool operator>(const RectangleShape& other) const;
		RectangleShape& operator=(const RectangleShape& other);

	};

}