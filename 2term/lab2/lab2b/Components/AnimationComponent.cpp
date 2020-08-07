#include "AnimationComponent.hpp"

// constructor

AnimationComponent::AnimationComponent(sf::RectangleShape& rect)
	: rect(rect) {

	this->dcolor = rect.getFillColor();
	this->animate = false;

	this->atime = 0.f;
	this->atimeMax = 10.f;

}

// accessors

bool AnimationComponent::getAtime() {

	if (this->atime >= this->atimeMax) {

		this->atime = 0.f;
		return true;

	}

	return false;

}

// methods

void AnimationComponent::takeDamage() {

	this->rect.setFillColor(sf::Color(this->dcolor.r, this->dcolor.g, this->dcolor.b, 150));
	this->animate = true;

}

void AnimationComponent::reset() {

	this->rect.setFillColor(this->dcolor);
	this->animate = false;

}

void AnimationComponent::updateAtime(const float& deltatime) {

	if (this->atime < this->atimeMax && this->animate)
		this->atime += 40.f * deltatime;

}

void AnimationComponent::update(const float& deltatime) {

	this->updateAtime(deltatime);

	if (this->getAtime() && this->animate)
		this->reset();

}