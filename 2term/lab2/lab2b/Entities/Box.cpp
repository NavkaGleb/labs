#include "Box.hpp"

// constructor / destructor

Box::Box(const int& hp, const unsigned& bamount, const float& size, const float& x, const float& y) {

	this->hp = hp;
	this->mhp = hp;
	this->type = "box";

	this->bamount = bamount;

	this->rect.setSize(sf::Vector2f(size, size));
	this->rect.setFillColor(sf::Color::Green);
	this->rect.setPosition(x - size / 2.f, y);

	this->initAnimationComponent();

	this->initHpBar(this->mhp, sf::Color(200, 200, 200, 200), x, y - 9.f, size, 5.f);

}

Box::~Box() {



}

// methods

void Box::update(const float& deltatime) {

	this->animationComponent->update(deltatime);

	this->hpBar->update(this->hp);

}

void Box::render(sf::RenderTarget& target) {

	target.draw(this->rect);

	this->hpBar->render(target);

}