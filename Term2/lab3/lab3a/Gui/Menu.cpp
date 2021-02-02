#include "Menu.hpp"

// constructor / destructor

Menu::Menu(const sf::VideoMode& videoMode, float pwidth, float pheight, sf::Font& font, float x, float y)
	: font(font) {

	// init container
	auto width = static_cast<float>(videoMode.width) * pwidth;
	auto height = static_cast<float>(videoMode.height) * pheight;
	this->container.setSize(sf::Vector2f(width, height));
	this->container.setPosition(x, y);
	this->container.setFillColor(sf::Color(100, 100, 100, 200));

}

Menu::~Menu() {

	for (auto& p : this->buttons)
		delete p.second;

}

// methods

void Menu::addButton(const std::string& key, const float& y, const float& width, const float& height,
	const std::string& text, const unsigned& characterSize) {

	this->buttons[key] = new Button(
		this->container.getPosition().x + (this->container.getGlobalBounds().width - width) / 2.f, y, width, height,
		this->font, text, characterSize,
		sf::Color(210, 210, 210, 235), sf::Color(220, 220, 220, 240), sf::Color(230, 230, 230, 250)
	);

}

bool Menu::isButtonPressed(const std::string& key) const {

	return this->buttons.at(key)->isPressed();

}

void Menu::update(const sf::Vector2i& mousePosWindow) {

	// update buttons
	for (auto& p : this->buttons)
		p.second->update(mousePosWindow);

}

void Menu::render(sf::RenderTarget& target) {

	target.draw(this->container);

	// render buttons
	for (auto& p : this->buttons)
		p.second->render(target);

}