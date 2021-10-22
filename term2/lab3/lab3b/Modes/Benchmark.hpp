#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <vector>
#include <chrono>
#include <map>
#include <functional>

#include "Mode.hpp"
#include "../Entities/Monster.hpp"

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
	int _power;																					// power of 2
	int _spower;																				// start power
	int _n;																						// amount of element
	Monster* _array;																			// monster array

	int _tamount;																				// tab amount

	std::map<std::string, std::vector<double>> _data;											// measurements
	std::map<std::string, std::function<bool(const Monster&, const Monster&)>> _cfunctions;		// comparable functions

	// filepath
	const char* _bfilepath;																		// benchmark filepath

	// methods
	void save(const char* name, bool append = true);
	void update();
	void reset();

	template <typename SortFunction, typename... Args>
	void sortRun(Monster* carray, const std::string& name, SortFunction sortFunction, Args... args);

	void oneFieldRun();
	void multipleFieldRun();

public:
	// constructor / destructor
	explicit Benchmark(int power);
	~Benchmark() override = default;

	// methods
	void run() override;

};

#endif // BENCHMARK_HPP