#ifndef LAB2B_BULLET_HPP
#define LAB2B_BULLET_HPP

#include <iostream>
#include <stack>
#include <map>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <vector>
#include <list>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Bullet {
private:
	// variables
	sf::CircleShape shape;
	sf::Vector2f center;
	sf::Vector2f velocity;
	float acceleration;
	int damage;
	int strength;
	bool motion;
	bool hit;
	float direction;

public:
	// constructor
	Bullet(const float& size, const float& acceleration, const int& damage, const int& strength, const float& direction,
		sf::Color color, const float& x, const float& y);
	virtual ~Bullet();

	// accessors
	sf::Vector2f getPosition() const;
	sf::FloatRect getGlobalBounds() const;
	[[nodiscard]] const int& getDamage() const;
	[[nodiscard]] const int& getStrength() const;
	[[nodiscard]] const bool& getHit() const;

	// modifiers
	void loseStrength();
	void setHit(const bool& hit_);

	// methods
	void update(const float& deltatime);
	void render(sf::RenderTarget& target);

};

#endif // BULLET_HPP
