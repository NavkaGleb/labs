#include "LifeCircle.hpp"

// constructor

LifeCircle::LifeCircle(const int& hp, const float& size, const float& x, const float& y) {

	this->hp = hp;

	this->shape = sf::CircleShape(size, 5);
	this->shape.setPosition(x, y);
	this->shape.setFillColor(sf::Color::Green);
	this->shape.setOutlineThickness(-2.f);
	this->shape.setOutlineColor(sf::Color::Black);

}

// accessors

const int& LifeCircle::getHp() const {

	return this->hp;

}

sf::FloatRect LifeCircle::getGlobalBounds() const {

	return this->shape.getGlobalBounds();

}

// methods

void LifeCircle::render(sf::RenderTarget& target) {

	target.draw(this->shape);

}