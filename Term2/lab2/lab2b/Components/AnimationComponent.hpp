#ifndef ANIMATIONCOMPONENT_HPP
#define ANIMATIONCOMPONENT_HPP

#include "../Gui/EntityGui.hpp"

class AnimationComponent {
private:
	// variables
	sf::RectangleShape& rect;										// shape of entity
	sf::Color dcolor;												// default color
	bool animate;													// control animation
	float atime;													// animation time
	float atimeMax;													// animation time max

public:
	// constructor
	explicit AnimationComponent(sf::RectangleShape& rect);

	// accessors
	[[nodiscard]] bool getAtime();

	// methods
	void takeDamage();
	void reset();

	void updateAtime(const float& deltatime);
	void update(const float& deltatime);

};

#endif // ANIMATIONCOMPONENT_HPP
