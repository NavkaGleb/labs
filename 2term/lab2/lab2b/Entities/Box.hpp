#ifndef BOX_HPP
#define BOX_HPP

#include "Enemy.hpp"

class Box : public Entity {
private:
	// variables

public:
	// constructor / destructor
	Box(const int& hp, const unsigned& bamount, const float& size, const float& x, const float& y);
	~Box() override;

	// accessors

	// methods
	void update(const float& deltatime) override;
	void render(sf::RenderTarget& target) override;

};

#endif // BOX_HPP
