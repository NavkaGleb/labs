#pragma once

#include "Button.hpp"

class Menu {
private:
	// variables
	sf::RectangleShape container;
	sf::Font& font;
	std::map<std::string, Button*> buttons;

public:
	Menu(const sf::VideoMode& videoMode, float pwidth, float pheight, sf::Font& font, float x = 0.f, float y = 0.f);
	virtual ~Menu();

	// methods
	void addButton(const std::string& key, const float& y, const float& width, const float& height,
		const std::string& text, const unsigned& characterSize);
	bool isButtonPressed(const std::string& key) const;

	void update(const sf::Vector2i& mousePosWindow);
	void render(sf::RenderTarget& target);

};