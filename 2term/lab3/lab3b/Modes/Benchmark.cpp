#include "Benchmark.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>

#include "../algo/algo.hpp"

/* ----------------------------------------------- class Timer ----------------------------------------------- */

// constructor / destructor

Benchmark::Timer::Timer(std::map<std::string, std::vector<double>>& data, std::string key)
	: data(data), key(std::move(key)) {

	this->startTimePoint = std::chrono::high_resolution_clock::now();

}

Benchmark::Timer::~Timer() {

	using namespace std::chrono;

	time_point<high_resolution_clock> endTimePoint = high_resolution_clock::now();

	auto start = time_point_cast<milliseconds>(this->startTimePoint).time_since_epoch().count();
	auto end = time_point_cast<milliseconds>(endTimePoint).time_since_epoch().count();

	this->data[this->key].emplace_back(static_cast<double>(end - start) * 0.001);

}

/* ----------------------------------------------- class Benchmark ----------------------------------------------- */

// private methods

void Benchmark::save(const char* name, bool append) {

	std::ofstream outfile(this->_bfilepath, append ? std::ios_base::app : std::ios_base::trunc);

	if (!outfile.is_open())
		throw std::invalid_argument("failed to open the file | Benchmark");

	outfile << "---------------------------------- " << name << " ----------------------------------" << std::endl;

	outfile << std::fixed;

	for (int i = 0; i < this->_tamount; i++)
		outfile << "\t";

	for (int i = this->_spower; i < this->_power; i++)
		outfile << "2^" << i << "\t";
	outfile << std::endl;

	for (auto& p : this->_data) {

		outfile << p.first;

		for (int i = 0; i < this->_tamount - (p.first.length() / 4); i++)
			outfile << "\t";

		for (const auto& e : p.second)
			outfile << std::setprecision(3) << e << "\t";
		p.second.clear();

		outfile << std::endl;

	}

	outfile << std::endl;

	outfile.close();

}

void Benchmark::update() {

	this->_power++;
	this->_n *= 2;

	delete [] this->_array;

	this->_array = new Monster[this->_n];

}

void Benchmark::reset() {

	this->_power = this->_spower;
	this->_n = static_cast<int>(std::pow(2, this->_power));

	delete [] this->_array;
	this->_data.clear();

	this->_array = new Monster[this->_n];

}

template <typename SortFunction, typename... Args>
void Benchmark::sortRun(Monster* carray, const std::string& name, SortFunction sortFunction, Args... args) {

	for (int i = 0; i < this->_n; i++)
		this->_array[i] = carray[i];

	Timer timer(this->_data, name);
	sortFunction(args...);

}

void Benchmark::oneFieldRun() {

	while (this->_power <= 21) {

		ng::algo::shuffle(this->_array, this->_n);

		auto* carray = new Monster[this->_n];
		for (int i = 0; i < this->_n; i++)
			carray[i] = this->_array[i];

		this->sortRun(
			carray, "quicksort", ng::algo::quicksort<Monster*, std::function<int(const Monster&, const Monster&)>>,
			this->_array, this->_array + this->_n - 1, this->_cfunctions["name"]
		);

		this->sortRun(
			carray, "merge sort", ng::algo::mergeSort<Monster, std::function<bool(const Monster&, const Monster&)>>,
			this->_array, 0, this->_n - 1, this->_cfunctions["name"]
		);

		this->sortRun(
			carray, "countingSort", ng::algo::countingSort<std::string, Monster, std::function<std::string(const Monster&)>>,
			this->_array, this->_n, [](const Monster& a) { return a.name(); }
		);

		this->sortRun(
			carray, "std::sort", std::sort<Monster*, std::function<bool(const Monster&, const Monster&)>>,
			this->_array, this->_array + this->_n, this->_cfunctions["name"]
		);


		this->update();
		delete [] carray;

	}

	this->save("name field", false);
	this->reset();

	while (this->_power <= 21) {

		ng::algo::shuffle(this->_array, this->_n);

		auto* carray = new Monster[this->_n];
		for (int i = 0; i < this->_n; i++)
			carray[i] = this->_array[i];

		this->sortRun(
			carray, "quicksort", ng::algo::quicksort<Monster*, std::function<int(const Monster&, const Monster&)>>,
			this->_array, this->_array + this->_n - 1, this->_cfunctions["hp"]
		);

		this->sortRun(
			carray, "merge sort", ng::algo::mergeSort<Monster, std::function<bool(const Monster&, const Monster&)>>,
			this->_array, 0, this->_n - 1, this->_cfunctions["hp"]
		);

		this->sortRun(
			carray, "countingSort", ng::algo::countingSort<int, Monster, std::function<int(const Monster&)>>,
			this->_array, this->_n, [](const Monster& a) { return a.hp(); }
		);

		this->sortRun(
			carray, "std::sort", std::sort<Monster*, std::function<bool(const Monster&, const Monster&)>>,
			this->_array, this->_array + this->_n, this->_cfunctions["hp"]
		);

		this->sortRun(
			carray, "radix sort", ng::algo::radixSort<Monster, std::function<bool(const Monster&)>>,
			this->_array, this->_n, [](const Monster& monster) { return monster.hp(); }
		);

		this->update();
		delete [] carray;

	}

	this->save("hp field");
	this->reset();

	while (this->_power <= 21) {

		ng::algo::shuffle(this->_array, this->_n);

		auto* carray = new Monster[this->_n];
		for (int i = 0; i < this->_n; i++)
			carray[i] = this->_array[i];

		this->sortRun(
			carray, "quicksort", ng::algo::quicksort<Monster*, std::function<int(const Monster&, const Monster&)>>,
			this->_array, this->_array + this->_n - 1, this->_cfunctions["achance"]
		);

		this->sortRun(
			carray, "merge sort", ng::algo::mergeSort<Monster, std::function<bool(const Monster&, const Monster&)>>,
			this->_array, 0, this->_n - 1, this->_cfunctions["achance"]
		);

		this->sortRun(
			carray, "countingSort", ng::algo::countingSort<int, Monster, std::function<float(const Monster&)>>,
			this->_array, this->_n, [](const Monster& a) { return a.achance(); }
		);

		this->sortRun(
			carray, "std::sort", std::sort<Monster*, std::function<bool(const Monster&, const Monster&)>>,
			this->_array, this->_array + this->_n, this->_cfunctions["achance"]
		);

		this->update();
		delete [] carray;

	}

	this->save("attack chance field");
	this->reset();

}

void Benchmark::multipleFieldRun() {

	while (this->_power <= 21) {

		ng::algo::shuffle(this->_array, this->_n);

		auto* carray = new Monster[this->_n];
		for (int i = 0; i < this->_n; i++)
			carray[i] = this->_array[i];

		this->sortRun(
			carray, "quicksort", ng::algo::quicksort<Monster*, std::function<int(const Monster&, const Monster&)>>,
			this->_array, this->_array + this->_n - 1, this->_cfunctions["multi"]
		);

		this->sortRun(
			carray, "merge sort", ng::algo::mergeSort<Monster, std::function<bool(const Monster&, const Monster&)>>,
			this->_array, 0, this->_n - 1, this->_cfunctions["multi"]
		);

		this->sortRun(
			carray, "std::sort", std::sort<Monster*, std::function<bool(const Monster&, const Monster&)>>,
			this->_array, this->_array + this->_n, this->_cfunctions["multi"]
		);

		this->update();
		delete [] carray;

	}

	this->save("multi");
	this->reset();

}

// constructor / destructor

Benchmark::Benchmark(int power) {

	this->_power = power;
	this->_spower = power;
	this->_n = static_cast<int>(std::pow(2, this->_power));
	this->_array = new Monster[this->_n];
	this->_tamount = 7;

	this->_cfunctions["name"] = [](const Monster& a, const Monster& b) { return a.name() < b.name(); };
	this->_cfunctions["hp"] = [](const Monster& a, const Monster& b) { return a.hp() < b.hp(); };
	this->_cfunctions["achance"] = [](const Monster& a, const Monster& b) { return a.achance() < b.achance(); };
	this->_cfunctions["multi"] = [](const Monster& a, const Monster& b) {

		if (a.name() != b.name())
			return a.name() < b.name();
		if (a.hp() != b.hp())
			return a.hp() < b.hp();
		if (a.damage() != b.damage())
			return a.damage() < b.damage();
		if (a.achance() != b.achance())
			return a.achance() < b.achance();
		return a.atype() < b.atype();

	};

	this->_bfilepath = "../Files/benchmark.txt";

}

// public methods

void Benchmark::run() {

	std::cout << "running..." << std::endl;

	this->oneFieldRun();
	this->multipleFieldRun();

	std::cout << "end (((" << std::endl;

}