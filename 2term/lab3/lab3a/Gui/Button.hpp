#pragma once

#include <string>

#include <SFML/Graphics.hpp>

class Button {
private:
	enum STATES { IDLE = 0, HOVER, ACTIVE };

	// variables
	short bstate;															// button state

	sf::RectangleShape shape;												// button shape
	sf::Text text;															// button text

	sf::Color buttonIdleColor;												// idle color
	sf::Color buttonHoverColor;												// hover color
	sf::Color buttonActiveColor;											// active color

	sf::Color textIdleColor;												// text idle color
	sf::Color textHoverColor;												// text hover color
	sf::Color textActiveColor;												// text active color

public:
	// constructor / destructor
	Button(float x, float y, float width, float height,
		sf::Font& font, const std::string& text, unsigned characterSize,
		sf::Color buttonIdleColor, sf::Color buttonHoverColor, sf::Color buttonActiveColor,
		sf::Color textIdleColor = sf::Color::Black,
		sf::Color textHoverColor = sf::Color::Black,
		sf::Color textActiveColor = sf::Color::Black);
	virtual ~Button() = default;

	// accessors
	bool isPressed() const;

	// methods
	void update(const sf::Vector2i& mousePosWindow);
	void render(sf::RenderTarget &target);

};