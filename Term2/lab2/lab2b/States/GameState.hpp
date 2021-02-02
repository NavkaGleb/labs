#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "../Map/Map.hpp"

class GameState : public State {
private:
	// variables
	sf::View view;																	// player view
	sf::RenderTexture rtexture;														// render texture
	sf::Sprite rsprite;																// render sprite
	Player* player;																	// player
	Map* map;																		// map

	sf::Font font;																	// font
	MessageBox* messageBox;															// message box

	const char* keybindsPath;														// path to keyBinds file
	std::map<std::string, int> keybinds;											// keybinds

	// random
	std::mt19937 mt;																// random engine
	std::uniform_int_distribution<int> uirand;										// unsigned int rand

	// methods
	void initDeferredRender();
	void initKeys();
	void initView();

	void updateView(const float& deltatime);
	void updateInput(const float& deltatime);

public:
	// constructor / destructor
	GameState(StateData& sdata, const unsigned& entitiesAmount);
	~GameState() override;

	// methods
	void update(const float& deltatime) override;
	void render(sf::RenderTarget& target) override;

};

#endif // GAMESTATE_HPP
