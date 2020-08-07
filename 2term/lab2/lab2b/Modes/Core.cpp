#include "Core.hpp"

// private methods

void Core::initWindow() {

	std::string title;
	unsigned width;
	unsigned height;
	unsigned framerateLimit;

	std::ifstream infile(this->windowPath);

	if (!infile.is_open())
		throw std::invalid_argument("error Core::initWindow: failed to open the file");

	std::getline(infile, title);
	infile >> width;
	infile >> height;
	infile >> framerateLimit;

	this->sdata.videoMode = sf::VideoMode(width, height);

	this->window = new sf::RenderWindow(this->sdata.videoMode, title);
	this->window->setFramerateLimit(framerateLimit);

}

void Core::initSupportedKeys() {

	std::string key;
	int value;

	std::ifstream infile(this->supportedKeysPath);

	if (!infile.is_open())
		throw std::invalid_argument("error Core::initSupportedKeys: failed to open the file");

	while (infile >> key >> value)
		this->sdata.supportedKeys[key] = value;

	infile.close();

}

void Core::initStates() {

	this->states.push(new GameState(this->sdata, 8));
	this->sdata.states = &this->states;

}

void Core::updateDt() {

	// update the deltatime with the time it takes to update and render 1 frame
	this->deltatime = this->dtClock.restart().asSeconds();

}

void Core::updateEvents() {

	while (this->window->pollEvent(this->event))
		if (this->event.type == sf::Event::Closed)
			this->window->close();

}

void Core::update() {

	this->updateEvents();

	if (!this->states.empty()) {

		this->states.top()->update(this->deltatime);

		if (this->states.top()->getQuit()) {

			this->states.top()->end();
			delete this->states.top();
			this->states.pop();

		}

	} else {

		this->end();
		this->window->close();

	}

}

void Core::render() {

	this->window->clear();

	if (!this->states.empty())
		this->states.top()->render(*this->window);

	this->window->display();

}

void Core::end() const {

	std::cout << "end of game (((" << std::endl;

}

// constructor / destructor

Core::Core() : window(nullptr), event(sf::Event()), deltatime(0.f) {

	this->supportedKeysPath = "../Config/supportedKeys.ini";
	this->windowPath = "../Config/window.ini";

	this->initWindow();
	this->initSupportedKeys();
	this->initStates();

}

Core::~Core() {

	delete this->window;

	while (!this->states.empty()) {

		delete this->states.top();
		this->states.pop();

	}

}

// methods

void Core::run() {

	while (this->window->isOpen()) {

		this->updateDt();
		this->update();
		this->render();

	}

}