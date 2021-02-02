#ifndef MAP_HPP
#define MAP_HPP

#include "../States/State.hpp"
#include "../Entities/Box.hpp"

class Map {
private:
	// variables
	sf::RectangleShape area;															// map area
	std::vector<Entity*> entities;														// map entities
	std::vector<LifeCircle*> elives;													// extra lives

	std::mt19937 mt;																	// mersenne twister engine
	std::uniform_int_distribution<int> uirand;											// unsigned int rand
	std::uniform_real_distribution<float> frand;										// float rand
	std::uniform_real_distribution<float> tfxrand;										// temp float x rand
	std::uniform_real_distribution<float> tfyrand;										// temp float y rand

	float centerX;

	// methods
	void initEntities(const unsigned& eamount);

public:
	// constructor / destructor
	Map(const StateData& sdata, const unsigned& eamount, const float& width, const float& height);
	virtual ~Map();

	// methods
	bool updatePlayerCollision(Player* player) const;
	bool updateCollision(const float& deltatime, Player* player);
	void update(const float& deltatime);

	void render(sf::RenderTarget& target);

};

#endif // MAP_HPP
