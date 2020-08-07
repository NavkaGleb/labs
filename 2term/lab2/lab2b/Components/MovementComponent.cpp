#include "MovementComponent.hpp"

// constructor / destructor

MovementComponent::MovementComponent(sf::RectangleShape& rect, float maxVelocity, float acceleration, float deceleration)
	: rect(rect), maxVelocity(maxVelocity), acceleration(acceleration), deceleration(deceleration) {



}

MovementComponent::~MovementComponent() {



}

// accessors

bool MovementComponent::getState(const short state) const {

	if (state == MSTATES::IDLE)
		if (this->velocity.x == 0.f && this->velocity.y == 0.f)
			return true;

	if (state == MSTATES::MOVING)
		if (this->velocity.x != 0 && this->velocity.y != 0.f)
			return true;

	if (state == MSTATES::MOVING_UP)
		if (this->velocity.y < 0.f)
			return true;

	if (state == MSTATES::MOVING_DOWN)
		if (this->velocity.y > 0.f)
			return true;

	return false;
}

// modifiers

void MovementComponent::stopVelocityY() {

	this->velocity.x = 0.f;

}

// methods

void MovementComponent::move(const float dirX, const float dirY, const float& deltatime) {

	this->velocity.x += this->acceleration * dirX * deltatime;
	this->velocity.y += this->acceleration * dirY * deltatime;

}

void MovementComponent::update(const float& deltatime) {

	/* decelerates the sprite and controls the maximum velocity. moves the sprite */

	if (this->velocity.x > 0.f) {

		// max velocity check x positive
		if (this->velocity.x > this->maxVelocity)
			this->velocity.x = this->maxVelocity;

		// deceleration x positive
		this->velocity.x -= this->deceleration * deltatime;
		if (this->velocity.x < 0.f)
			this->velocity.x = 0.f;

	} else if (this->velocity.x < 0.f) {

		// max velocity check x negative
		if (this->velocity.x < -this->maxVelocity)
			this->velocity.x = -this->maxVelocity;

		// deceleration x negative
		this->velocity.x += this->deceleration * deltatime;
		if (this->velocity.x > 0.f)
			this->velocity.x = 0.f;

	}

	if (this->velocity.y > 0.f) {

		// max velocity check x positive
		if (this->velocity.y > this->maxVelocity)
			this->velocity.y = this->maxVelocity;

		// deceleration x positive
		this->velocity.y -= this->deceleration * deltatime;
		if (this->velocity.y == 0.f)
			this->velocity.y = 0.f;

	} else if (this->velocity.y < 0.f) {

		// max velocity check x negative
		if (this->velocity.y < -this->maxVelocity)
			this->velocity.y = -this->maxVelocity;


		this->velocity.y += this->deceleration * deltatime;
		if (this->velocity.y > 0.f)
			this->velocity.y = 0.f;

	}

	// final move
	this->rect.move(this->velocity * deltatime);

}

void MovementComponent::render(sf::RenderTarget& target) {



}