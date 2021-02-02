#include "Entity.hpp"

// private methods

void Entity::initMovementComponent(const float& maxVelocity, const float& acceleration, const float& deceleration) {

	this->movementComponent = new MovementComponent(this->rect, maxVelocity, acceleration, deceleration);

}

void Entity::initAnimationComponent() {

	this->animationComponent = new AnimationComponent(this->rect);

}

void Entity::initHpBar(const int& mvalue, const sf::Color& fillColor,
	const float& x, const float& y, const float& width, const float& height) {

	this->hpBar = new gui::Bar(mvalue, fillColor, x, y, width, height);

}

// constructor

Entity::Entity() : hp(0), mhp(0), damage(0), bamount(0), bullet(nullptr),
	movementComponent(nullptr), animationComponent(nullptr), hpBar(nullptr) {}

// accessors

sf::Vector2f Entity::getPosition() const {

	return this->rect.getPosition();

}

sf::FloatRect Entity::getGlobalBounds() const {

	return this->rect.getGlobalBounds();

}

Bullet* Entity::getBullet() const {

	return this->bullet;

}

const int& Entity::getHp() const {

	return this->hp;

}

const int& Entity::getDamage() const {

	return this->damage;

}

const unsigned& Entity::getBamount() const {

	return this->bamount;

}

const std::string& Entity::getType() const {

	return this->type;

}

bool Entity::intersects(const sf::FloatRect& frect) const {

	return this->rect.getGlobalBounds().intersects(frect);

}

// modifiers

void Entity::stopVelocityY() {

	if (this->movementComponent)
		this->movementComponent->stopVelocityY();

}

void Entity::setPositionY(const float& y) {

	this->rect.setPosition(this->rect.getPosition().x, y);

}

// methods

void Entity::loseHp(const int& hp_) {

	this->hp -= hp_;

}

void Entity::takeDamage() {

	if (this->animationComponent)
		this->animationComponent->takeDamage();

}

void Entity::deleteBullet() {

	delete this->bullet;
	this->bullet = nullptr;

}