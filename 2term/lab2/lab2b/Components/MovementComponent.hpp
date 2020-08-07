#ifndef MOVEMENTCOMPONENT_HPP
#define MOVEMENTCOMPONENT_HPP

#include "../Components/AnimationComponent.hpp"

// movement states
enum MSTATES { IDLE = 0, MOVING, MOVING_UP, MOVING_DOWN };

class MovementComponent {
private:
	// variables
	sf::RectangleShape& rect;

	float maxVelocity;
	float acceleration;
	float deceleration;

	sf::Vector2f velocity;

public:
	// constructor / destructor
	MovementComponent(sf::RectangleShape& rect, float maxVelocity, float acceleration, float deceleration);
	virtual ~MovementComponent();

	// accessors
	[[nodiscard]] bool getState(short state) const;

	// modifiers
	void stopVelocityY();

	// methods
	void move(float dirX, float dirY, const float& deltatime);

	void update(const float& deltatime);
	void render(sf::RenderTarget& target);
};

#endif // MOVEMENTCOMPONENT_HPP
