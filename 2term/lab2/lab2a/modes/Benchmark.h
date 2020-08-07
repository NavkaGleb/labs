#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "Mode.h"

class Benchmark : public Mode {
private:
	// inner class
	class BenchmarkData  {
	public:
		// variables
		unsigned amount;
		std::map<std::string, std::vector<long long>> measurements;

		// constructor
		BenchmarkData();

		// methods
		void addMeasurement(const std::string& key, long long time);

	};

	// variables
	unsigned amount;									// amount of entities
	unsigned amount_;
	long long stopTime;									// in seconds
	long long duration;									// duration of one iteration

	std::vector<BenchmarkData*> data;					// stores the benchmark data

	// file path
	const char* lpath;									// path to list deques file
	const char* apath;									// path to array deques file
	const char* epath;									// path to experiment with deques

	// random
	std::mt19937 mt;
	std::uniform_real_distribution<double> drand;

	// methods
	void update();
	void reset();

	template <class T>
	void save(const std::vector<ng::DequeInterface<T>*>& containers, const char* path);

	template <class T>
	long long sdequeRun(const std::vector<ng::DequeInterface<T>*>& containers, BenchmarkData* bdata);

	template <class T>
	long long edequeRun(const std::vector<ng::DequeInterface<T>*>& containers, BenchmarkData* bdata);

	template <class T>
	void loopRun(const std::vector<ng::DequeInterface<T>*>& containers, const char* path, char func);

public:
	// constructor
	Benchmark(unsigned amount, long long stopTime);
	~Benchmark() override;

	// methods
	void run() override;

};

#endif // BENCHMARK_H