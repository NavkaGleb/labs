#include "Bullet.hpp"

// constructor / destructor

Bullet::Bullet(const float& size, const float& acceleration, const int& damage, const int& strength, const float& direction,
	const sf::Color color, const float& x, const float& y) {

	this->center.x = x - size;
	this->center.y = y;

	this->acceleration = acceleration;

	this->shape.setRadius(size);
	this->shape.setPosition(this->center);
	this->shape.setFillColor(color);

	this->damage = damage;
	this->strength = strength;
	this->direction = direction;
	this->motion = true;
	this->hit = false;

}

Bullet::~Bullet() {



}

// accessors

sf::Vector2f Bullet::getPosition() const {

	return this->shape.getPosition();

}

sf::FloatRect Bullet::getGlobalBounds() const {

	return this->shape.getGlobalBounds();

}

const int& Bullet::getDamage() const {

	return this->damage;

}

const int& Bullet::getStrength() const {

	return this->strength;

}

const bool& Bullet::getHit() const {

	return this->hit;

}

// modifiers

void Bullet::loseStrength() {

	this->strength--;

}

void Bullet::setHit(const bool& hit_) {

	this->hit = hit_;

}

// methods

void Bullet::update(const float& deltatime) {

	if (this->motion)
		this->velocity.y = this->direction * this->acceleration;

	this->shape.move(this->velocity * deltatime);

}

void Bullet::render(sf::RenderTarget& target) {

	target.draw(this->shape);

}