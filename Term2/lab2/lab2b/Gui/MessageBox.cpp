#include "MessageBox.hpp"

// constructor

MessageBox::MessageBox(const std::string& text, const sf::Font& font, const sf::Color& tcolor,
	const float& width, const float& height, const sf::VideoMode& videoMode) {

	// init back
	this->back.setSize(sf::Vector2f(videoMode.width, videoMode.height));
	this->back.setPosition(0.f, 0.f);
	this->back.setFillColor(sf::Color(100, 100, 100, 150));

	// init rect
	this->rect.setSize(sf::Vector2f(width, height));
	this->rect.setPosition(
			(static_cast<float>(videoMode.width) - width) / 2.f,
			(static_cast<float>(videoMode.height) - height) / 2.f
	);
	this->rect.setFillColor(sf::Color(200, 200, 200, 255));

	// init text
	this->text.setString(text);
	this->text.setCharacterSize(30);
	this->text.setStyle(sf::Text::Bold);
	this->text.setFont(font);
	this->text.setPosition(
			(static_cast<float>(videoMode.width) - this->text.getGlobalBounds().width) / 2.f,
			(static_cast<float>(videoMode.height) - this->text.getGlobalBounds().height) / 2.f - 5.f
	);
	this->text.setFillColor(tcolor);

}

// methods

void MessageBox::render(sf::RenderTarget& target) {

	target.draw(this->back);
	target.draw(this->rect);
	target.draw(this->text);

}