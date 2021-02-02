#include "RectangleShape.hpp"

// private methods

void ng::RectangleShape::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	target.draw(this->rect, states);

}

// accessors

sf::FloatRect ng::RectangleShape::getGlobalBounds() const {

	return this->rect.getGlobalBounds();

}

sf::Vector2f ng::RectangleShape::getPosition() const {

	return this->rect.getPosition();

}

// modifiers

void ng::RectangleShape::setSize(const float& width, const float& height) {

	this->rect.setSize(sf::Vector2f(width, height));

}

void ng::RectangleShape::setSize(const sf::FloatRect& frect) {

	this->rect.setSize(sf::Vector2f(frect.width, frect.height));

}

void ng::RectangleShape::setPosition(const float& x, const float& y) {

	this->rect.setPosition(x, y);

}

void ng::RectangleShape::setXPosition(const float& x) {

	this->rect.setPosition(x, this->getPosition().y);

}

void ng::RectangleShape::updateYPosition(const unsigned& height) {

	this->rect.setPosition(this->getPosition().x, static_cast<float>(height) - this->getGlobalBounds().height);

}

void ng::RectangleShape::setFillColor(const sf::Color& color) {

	this->rect.setFillColor(color);

}

// methods

bool ng::RectangleShape::operator<(const ng::RectangleShape& other) const {

	return this->rect.getGlobalBounds().height < other.rect.getGlobalBounds().height;

}

bool ng::RectangleShape::operator>(const ng::RectangleShape& other) const {

	return this->rect.getGlobalBounds().height > other.rect.getGlobalBounds().height;

}

ng::RectangleShape& ng::RectangleShape::operator=(const ng::RectangleShape& other) {

	if (this == &other)
		return *this;

	this->rect = other.rect;
	return *this;

}