#ifndef LIFECIRCLE_HPP
#define LIFECIRCLE_HPP

#include "../Weapons/Bullet.hpp"

class LifeCircle {
private:
	sf::CircleShape shape;
	int hp;

public:
	// constructor / destructor
	LifeCircle(const int& hp, const float& size, const float& x, const float& y);
	virtual ~LifeCircle() = default;

	// accessors
	[[nodiscard]] const int& getHp() const;
	[[nodiscard]] sf::FloatRect getGlobalBounds() const;

	// methods
	void render(sf::RenderTarget& target);

};


#endif // LIFECIRCLE_HPP
