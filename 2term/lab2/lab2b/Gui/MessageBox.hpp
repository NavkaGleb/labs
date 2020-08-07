#ifndef MESSAGEBOX_HPP
#define MESSAGEBOX_HPP

#include "../Inventory/LifeCircle.hpp"

class MessageBox {
private:
	// variables
	sf::RectangleShape back;
	sf::RectangleShape rect;
	sf::Text text;

public:
	// constructor / destructor
	MessageBox(const std::string& text, const sf::Font& font, const sf::Color& tcolor,
		const float& width, const float& height, const sf::VideoMode& videoMode);
	virtual ~MessageBox() = default;

	// methods
	void render(sf::RenderTarget& target);
};

#endif // MESSAGEBOX_HPP
