#include "GameState.hpp"

// private

void GameState::initDeferredRender() {

	// init rtexture
	this->rtexture.create(this->sdata.videoMode.width, this->sdata.videoMode.height);

	// init rsprite
	this->rsprite.setTexture(this->rtexture.getTexture());
	this->rsprite.setTextureRect(sf::IntRect(0, 0, this->sdata.videoMode.width, this->sdata.videoMode.height));

}

void GameState::initKeys() {

	std::string key1;
	std::string key2;

	std::ifstream infile(this->keybindsPath);

	if (!infile.is_open())
		throw std::invalid_argument("error GameState::initKeys: failed to open the file");

	while (infile >> key1 >> key2)
		this->keybinds[key1] = this->sdata.supportedKeys[key2];

	infile.close();

}

void GameState::initView() {

	this->view.setSize(
		static_cast<float>(sdata.videoMode.width),
		static_cast<float>(sdata.videoMode.height)
	);

}

void GameState::updateView(const float& deltatime) {

	this->view.setCenter(
		this->player->getPosition().x + this->player->getGlobalBounds().width / 2.f,
		this->player->getPosition().y - static_cast<float>(sdata.videoMode.height) / 4.f
	);

}

void GameState::updateInput(const float& deltatime) {

	if (!this->paused) {

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds["MOVE_UP"])))
			this->player->move(0.f, -1.f, deltatime);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds["MOVE_DOWN"])))
			this->player->move(0.f, 1.f, deltatime);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds["ATTACK"])) && this->getKeytime())
			this->player->attack();

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds["CLOSE"])) && this->getKeytime())
		this->end();

}

// constructor / destructor

GameState::GameState(StateData& sdata, const unsigned& entitiesAmount)
	: State(sdata), mt(std::clock()) {

	this->keybindsPath = "../Config/GameStateKeybinds.ini";
	this->uirand = std::uniform_int_distribution<int>(0, 10000);

	this->player = new Player(500, 4, static_cast<float>(sdata.videoMode.width) / 2.f);
	this->map = new Map(sdata, 40, 70.f, 3000.f);

	this->font.loadFromFile("../Fonts/Dosis-Light.ttf");
	this->messageBox = nullptr;

	this->initDeferredRender();
	this->initKeys();
	this->initView();

}

GameState::~GameState() {

	delete this->player;
	delete this->map;
	delete this->messageBox;

}

// public methods

void GameState::update(const float& deltatime) {

	this->updateKeytime(deltatime);
	this->updateInput(deltatime);

	if (!this->paused) {

		this->updateView(deltatime);

		if (!this->map->updatePlayerCollision(this->player)) {

			this->messageBox = new MessageBox("YOU DIED", this->font, sf::Color::Black, 250.f, 100.f, sdata.videoMode);
			this->paused = true;

		}

		this->map->update(deltatime);

		if (!this->map->updateCollision(deltatime, this->player)) {

			this->messageBox = new MessageBox("YOU WIN", this->font, sf::Color::Black, 250.f, 100.f, sdata.videoMode);
			this->paused = true;

		}

		this->player->update(deltatime);

	}

}

void GameState::render(sf::RenderTarget& target) {

	// clear texture
	this->rtexture.clear();

	this->rtexture.setView(this->view);

	this->map->render(this->rtexture);
	this->player->render(this->rtexture);

	this->rtexture.setView(this->rtexture.getDefaultView());

	// render player gui
	this->player->renderGui(this->rtexture);

	// render message box
	if (this->messageBox)
		this->messageBox->render(this->rtexture);

	this->rtexture.display();
	this->rsprite.setTexture(this->rtexture.getTexture());

	// final render
	target.draw(this->rsprite);

}