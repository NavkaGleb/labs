#ifndef CORE_HPP
#define CORE_HPP

#include "../States/GameState.hpp"

class Core {
private:
	// variables
	StateData sdata;

	sf::RenderWindow* window;
	sf::Event event;

	std::stack<State*> states;

	sf::Clock dtClock;
	float deltatime;

	const char* supportedKeysPath;
	const char* windowPath;

	// methods
	void initWindow();
	void initSupportedKeys();
	void initStates();

	void updateDt();
	void updateEvents();
	void update();
	void render();

	void end() const;

public:
	// constructor / destructor
	Core();
	virtual ~Core();

	// methods
	void run();
};

#endif // CORE_HPP
