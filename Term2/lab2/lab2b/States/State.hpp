#ifndef STATE_HPP
#define STATE_HPP

#include "../Entities/Enemy.hpp"

class StateData;

// abstract class
class State {
protected:
	// variables
	StateData& sdata;
	float keytime;
	float keytimeMax;
	bool quit;
	bool paused;

public:
	// destructor
	explicit State(StateData& sdata);
	virtual ~State() = default;

	// accessors
	bool getKeytime();
	[[nodiscard]] const bool& getQuit() const;

	// methods
	void end();
	void updateKeytime(const float& deltatime);
	virtual void update(const float& deltatime) = 0;
	virtual void render(sf::RenderTarget& target) = 0;

};

class StateData {
public:
	// variables
	std::map<std::string, int> supportedKeys;
	sf::VideoMode videoMode;
	std::stack<State*>* states;

};

#endif // STATE_HPP
