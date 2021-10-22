#include "State.hpp"

// constructor

State::State(StateData& sdata) : sdata(sdata), keytime(0.f), keytimeMax(10.f), quit(false), paused(false) {}

// accessors

bool State::getKeytime() {

	if (this->keytime >= this->keytimeMax) {

		this->keytime = 0.f;
		return true;

	}

	return false;

}

const bool& State::getQuit() const {

	return this->quit;

}

// methods

void State::end() {

	this->quit = true;

}

void State::updateKeytime(const float& deltatime) {

	if (this->keytime < this->keytimeMax)
		this->keytime += 40.f * deltatime;

}