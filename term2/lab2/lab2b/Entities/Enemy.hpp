#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Player.hpp"

class Enemy : public Entity {
private:
	// variables
	float btime;														// control the generating bullet time
	float btimeMax;														// time for generating new bullet

public:
	// constructor / destructor
	Enemy(const int& hp, const int& damage, const float& size,
		const float& btimeMax, const float& x, const float& y);
	~Enemy() override;

	// accessors
	[[nodiscard]] bool getBtime();

	// methods
	void attack();

	void updateBtime(const float& deltatime);
	void update(const float& deltatime) override;

	void render(sf::RenderTarget& target) override;

};

#endif // ENEMY_HPP
