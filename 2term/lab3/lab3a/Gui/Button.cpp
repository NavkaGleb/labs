#include "Button.hpp"

#include <cmath>

// constructor / destructor

Button::Button(float x, float y, float width, float height,
	sf::Font& font, const std::string& text, unsigned characterSize,
	sf::Color buttonIdleColor, sf::Color buttonHoverColor, sf::Color buttonActiveColor,
	sf::Color textIdleColor, sf::Color textHoverColor, sf::Color textActiveColor) {

	this->bstate = STATES::IDLE;

	// init shape
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setPosition(x, y);
	this->shape.setFillColor(buttonIdleColor);

	// init text
	this->text.setFont(font);
	this->text.setString(text);
	this->text.setFillColor(textIdleColor);
	this->text.setCharacterSize(characterSize);
	this->text.setPosition(std::floor(x + (width / 2.f) - (this->text.getGlobalBounds().width / 2.f)), std::floor(y));

	// init button color
	this->buttonIdleColor = buttonIdleColor;
	this->buttonHoverColor = buttonHoverColor;
	this->buttonActiveColor = buttonActiveColor;

	// init text color
	this->textIdleColor = textIdleColor;
	this->textHoverColor = textHoverColor;
	this->textActiveColor = textActiveColor;

}

// accessors

bool Button::isPressed() const {

	return this->bstate == STATES::ACTIVE;

}

// methods

void Button::update(const sf::Vector2i& mousePosWindow) {

	// idle
	this->bstate = STATES::IDLE;

	if (this->shape.getGlobalBounds().contains(sf::Vector2f(mousePosWindow))) {

		// hover
		this->bstate = STATES::HOVER;

		// active
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			this->bstate = STATES::ACTIVE;

	}

	switch (this->bstate) {

		case STATES::IDLE:
			this->shape.setFillColor(this->buttonIdleColor);
			this->text.setFillColor(this->textIdleColor);
			break;

		case STATES::HOVER:
			this->shape.setFillColor(this->buttonHoverColor);
			this->text.setFillColor(this->textHoverColor);
			break;

		case STATES::ACTIVE:
			this->shape.setFillColor(this->buttonActiveColor);
			this->text.setFillColor(this->textActiveColor);
			break;

		default:
			this->shape.setFillColor(this->buttonIdleColor);
			this->text.setFillColor(this->textIdleColor);
			break;

	}

}

void Button::render(sf::RenderTarget& target) {

	target.draw(this->shape);
	target.draw(this->text);

}