#include "EntityGui.hpp"

// constructor / destructor

gui::Bar::Bar(const int& mvalue, const sf::Color& fillColor, const float& x, const float& y,
	const float& width, const float& height) {

	this->mvalue = mvalue;
	this->mwidth = width;

	this->backBar.setSize(sf::Vector2f(width, height));
	this->backBar.setPosition(x - width / 2.f, y);
	this->backBar.setFillColor(sf::Color(70, 70, 70, 150));

	this->innerBar.setSize(sf::Vector2f(width, height));
	this->innerBar.setPosition(x - width / 2.f, y);
	this->innerBar.setFillColor(fillColor);

}

gui::Bar::~Bar() {



}

// methods

void gui::Bar::update(const int& value) {

	float percent = static_cast<float>(value) / static_cast<float>(this->mvalue);
	this->innerBar.setSize(sf::Vector2f(std::floor(percent * this->mwidth), this->innerBar.getSize().y));

}

void gui::Bar::render(sf::RenderTarget& target) {

	target.draw(this->backBar);
	target.draw(this->innerBar);

}