#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"

class Player : public Entity {
private:
	// variables
	unsigned mbamount;															// max bullet amount
	int bstrength;															// bullet strength

	float btime;																// bullet time
	float mbtime;																// max bullet time

	std::vector<Bullet*> bullets;												// player bullets

	gui::Bar* bulletBar;														// bullet bar

public:
	// constructor / destructor
	Player(const int& hp, const unsigned& bamount, const float& x);
	~Player() override;

	// accessors
	bool getBtime();

	// methods
	void addBamount(const int& amount);
	void addHp(const int& hp);
	void move(float dirX, float dirY, const float& deltatime);
	void attack();

	void updateBtime(const float& deltatime);
	void updateBullets(const float& deltatime);
	void update(const float& deltatime) override;

	void renderGui(sf::RenderTarget& target);
	void render(sf::RenderTarget& target) override;

	friend class Map;

};

#endif // PLAYER_HPP
