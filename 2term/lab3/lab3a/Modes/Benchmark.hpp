#pragma once

#include <map>
#include <vector>
#include <chrono>
#include <bitset>
#include <random>
#include <ctime>
#include <fstream>

#include "Mode.hpp"

class Benchmark : public Mode {
private:
	// inner class
	class Timer {
	public:
		// variables
		std::map<std::string, std::vector<double>>& data;
		std::string key;
		std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;

		// constructor / destructor
		Timer(std::map<std::string, std::vector<double>>& data, std::string key);
		virtual ~Timer();

	};

	// variables
	int power;																			// current power of 2
	int spower;																			// start power
	int n;																				// amount of elements
	int* array;																			// sorting array

	int tamount;																		// tab amount

	std::map<std::string, std::vector<double>> data;									// store measurements

	// filepath
	const char* n2ShuffleArray;
	const char* n2AlmostShuffleArray;
	const char* n2ReverseArray;

	const char* nlognShuffleArray;
	const char* nlognAlmostShuffleArray;
	const char* nlognReverseArray;														// there is stack overflow in quicksort

	const char* qpShuffleArray;															// quicksort shuffle array
	const char* qpAlmostShuffleArray;													// quicksort almost shuffle array
	const char* qpReverseArray;															// there is stack overflow in quicksort

	const char* combinedSorting;

	const char* parallelSorting;

	// methods
	void save(const char* filepath, const std::_Ios_Openmode& mode);

	void update();																		// array update
	void reset();																		// array reset

	template <typename ShuffleFunc, typename SortFunc, typename... Args>
	void sortRun(const std::string& name, ShuffleFunc shuffleFunc, SortFunc sortFunc, Args... args);

	/* ----------------- standard sort ----------------- */
	template <typename ShuffleFunc>
	void n2Run(ShuffleFunc shuffleFunc, const char* filepath);
	
	template <typename ShuffleFunc>
	void nlognRun(ShuffleFunc shuffleFunc, const char* filepath);

	template <typename ShuffleFunc>
	void quicksortPartitionsRun(ShuffleFunc shuffleFunc, const char* filepath);

	void combinedSortingRun();

	template <typename ShuffleFunc>
	void parallelSortingRun(ShuffleFunc shuffleFunc);

public:
	// constructor / destructor
	explicit Benchmark(int power);
	~Benchmark() override;

	// methods
	void run() override;

};