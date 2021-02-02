#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <ctime>
#include <future>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <map>

#include <SFML/Window.hpp>

#include "Mode.hpp"
#include "../Sorting/algo.hpp"
#include "../Gui/RectangleShape.hpp"
#include "../Gui/Menu.hpp"

class DemoMode : public Mode {
private:
	// variables
	sf::VideoMode videoMode;																// video mode
	sf::RenderWindow* window;																// render window
	sf::Event event;																		// event
	sf::Vector2u wsize;																		// window size
	sf::Vector2i mousePosWindow;															// mouse position relative window
	bool paused;																			// tracks pause status
	bool quit;																				// tracks quit status
	sf::Font font;																			// font

	Menu* menu;																				// sorting menu

	sf::Clock dtClock;																		// deltatime clock
	float dt;																				// deltatime

	float ktime;																			// key time
	float mktime;																			// max key time

	std::future<void>* sthread;																// sorting thread
	std::atomic_bool scancel;																// tracks of canceling sorting thread
	bool sorted;																			// tracks array status

	float width;																			// width of column
	std::map<std::string, sf::Color> colors;												// sorting colors

	int n;																					// amount of elements
	ng::RectangleShape* array;																// array of rectangles

	std::mt19937 mt;																		// random engine
	std::uniform_int_distribution<int> irand;												// number generator

	// sorting methods
	void swap(const int& i, const int& j);

	void shuffle(const int& delayTime);
	void almostShuffle(const int& delayTime);
	void reverse(const int& delayTime);

	void reset(const int& left, const int& right);

	void bubbleSort(const int& delayTime);
	void selectionSort(const int& delayTime);
	void insertionSort(const int& left, const int& right, const int& delayTime);

	void shellSort(const int& delayTime);

	int partition(const int& left, const int& right, const int& delayTime);
	void quicksort(const int& left, const int& right, const int& cutoff,
		const int& qdelayTime, const int& idelayTime, const bool& mthread);

	void merge(const int& left, const int& middle, const int& right, const int& delayTime);
	void mergeSort(const int& left, const int& right, const int& cutoff,
		const int& mdelayTime, const int& idelayTime, const bool& mthread);
	void bumergeSort(const int& delayTime);													// bottom up merge sort

	void rcountingSort(const int& exp, const int& delayTime);
	void radixSort(const int& delayTime);

	// init methods
	void initFonts();
	void initButtons();
	void initColors();
	void initArray();

	// accessors
	bool getKtime();

	// update methods
	void updateDt();
	void updateKtime();
	void updateMousePosition();
	void updateInput();
	void updateButtons();
	bool updateSthread();
	void update();

	// render methods
	void render();

public:
	// constructor / destructor
	DemoMode(unsigned width, unsigned height, int n);
	~DemoMode() override;

	// methods
	void run() override;

};