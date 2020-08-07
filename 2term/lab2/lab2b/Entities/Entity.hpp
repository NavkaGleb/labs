#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "../Components/MovementComponent.hpp"

class Entity {
protected:
	// variables
	sf::RectangleShape rect;														// shape of entity
	int hp;																			// hp of entity
	int mhp;																		// max hp of entity
	int damage;																		// damage of entity
	unsigned bamount;																// amount of bullets
	std::string type;																// type of entity

	Bullet* bullet;																	// entity bullet

	MovementComponent* movementComponent;											// movement component
	AnimationComponent* animationComponent;											// animation component

	gui::Bar* hpBar;																// hp bar

	// methods
	void initMovementComponent(const float& maxVelocity, const float& acceleration, const float& deceleration);
	void initAnimationComponent();

	void initHpBar(const int&mvalue, const sf::Color& fillColor, const float& x, const float& y,
		const float& width, const float& height);

public:
	// constructor / destructor
	Entity();
	virtual ~Entity() = default;

	// accessors
	sf::Vector2f getPosition() const;
	sf::FloatRect getGlobalBounds() const;
	Bullet* getBullet() const;
	[[nodiscard]] const int& getHp() const;
	[[nodiscard]] const int& getDamage() const;
	[[nodiscard]] const unsigned& getBamount() const;
	[[nodiscard]] const std::string& getType() const;
	[[nodiscard]] bool intersects(const sf::FloatRect& frect) const;

	// modifiers
	void stopVelocityY();
	void setPositionY(const float& y);

	// methods
	void loseHp(const int& hp_);
	void takeDamage();
	void deleteBullet();

	virtual void update(const float& deltatime) = 0;
	virtual void render(sf::RenderTarget& target) = 0;

};

#endif // ENTITY_HPP
