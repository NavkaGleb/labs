#include "Benchmark.hpp"

#include <iomanip>
#include <utility>

#include "../Sorting/algo.hpp"

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

void Benchmark::save(const char* filepath, const std::_Ios_Openmode& mode) {

	std::ofstream outfile(filepath, mode);

	if (!outfile.is_open())
		throw std::invalid_argument("failed to open the file | Benchmark");

	outfile << std::fixed;

	for (int i = 0; i < this->tamount; i++)
		outfile << "\t";

	for (int i = this->spower; i < this->power; i++)
		outfile << "2^" << i << "\t";
	outfile << std::endl;

	for (auto& p : this->data) {

		outfile << p.first;

		for (int i = 0; i < this->tamount - (p.first.length() / 4); i++)
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

	this->power++;
	this->n *= 2;

	delete [] this->array;

	this->array = new int[this->n];
	for (int i = 0; i < this->n; i++)
		this->array[i] = i;

}

void Benchmark::reset() {

	this->power = this->spower;
	this->n = static_cast<int>(std::pow(2, this->power));

	delete [] this->array;

	this->array = new int[this->n];
	for (int i = 0; i < this->n; i++)
		this->array[i] = i;

	this->data.clear();

}

template <typename ShuffleFunc, typename SortFunc, typename... Args>
void Benchmark::sortRun(const std::string& name, ShuffleFunc shuffleFunc, SortFunc sortFunc, Args... args) {

	shuffleFunc(this->array, this->n);

	{
		Timer timer(this->data, name);
		sortFunc(args...);
	}

}

template <typename ShuffleFunc>
void Benchmark::n2Run(ShuffleFunc shuffleFunc, const char* filepath) {

	while (this->power <= 18) {

		this->sortRun("1. bubble sort", shuffleFunc, ng::algo::bubbleSort<int>, this->array, this->n);
		this->sortRun("2. selection sort", shuffleFunc, ng::algo::selectionSort<int>, this->array, this->n);
		this->sortRun("3. insertion sort", shuffleFunc, ng::algo::insertionSort<int>, this->array, 0, this->n - 1);

		this->update();

	}

	this->save(filepath, std::ios_base::out);
	this->reset();

}

template <typename ShuffleFunc>
void Benchmark::nlognRun(ShuffleFunc shuffleFunc, const char* filepath) {

	while (this->power <= 26) {

		this->sortRun("1. shell sort", shuffleFunc, ng::algo::shellSort<int>, this->array, this->n);
		this->sortRun("2. quicksort", shuffleFunc,ng::algo::quicksort<int>, this->array, 0, this->n - 1, ng::algo::hmof3partition<int>);
		this->sortRun("3. cquicksort", shuffleFunc, ng::algo::cquicksort<int>, this->array, 0, this->n - 1, 5, ng::algo::hmof3partition<int>);
		this->sortRun("4. merge sort", shuffleFunc, ng::algo::mergeSort<int>, this->array, 0, this->n - 1);
		this->sortRun("5. combined merge sort", shuffleFunc, ng::algo::cmergeSort<int>, this->array, 0, this->n - 1, 5);
		this->sortRun("6. bottom-up merge sort", shuffleFunc, ng::algo::buMergeSort<int>, this->array, this->n);
		this->sortRun("7. std::sort", shuffleFunc, std::sort<int*>, this->array, this->array + this->n);
		this->sortRun("8. std::stable_sort", shuffleFunc, std::stable_sort<int*>, this->array, this->array + this->n);

		this->update();

	}

	this->save(filepath, std::ios_base::out);
	this->reset();

}

template <typename ShuffleFunc>
void Benchmark::quicksortPartitionsRun(ShuffleFunc shuffleFunc, const char* filepath) {

	while (this->power <= 26) {

		this->sortRun(
			"1. quicksort llpartition", shuffleFunc,
			ng::algo::quicksort<int>, this->array, 0, this->n - 1, ng::algo::llpartition<int>
		);

		this->sortRun(
			"2. quicksort lrpartition", shuffleFunc,
			ng::algo::quicksort<int>, this->array, 0, this->n - 1, ng::algo::lrpartition<int>
		);

		this->sortRun(
			"3. quicksort hlpartition", shuffleFunc,
			ng::algo::quicksort<int>, this->array, 0, this->n - 1, ng::algo::hlpartition<int>
		);

		this->sortRun(
			"4. quicksort hrpartition", shuffleFunc,
			ng::algo::quicksort<int>, this->array, 0, this->n - 1, ng::algo::hrpartition<int>
		);

		this->sortRun(
			"5. quicksort lmof3partition", shuffleFunc,
			ng::algo::quicksort<int>, this->array, 0, this->n - 1, ng::algo::lmof3partition<int>
		);

		this->sortRun(
			"6. quicksort hmof3partition", shuffleFunc,
			ng::algo::quicksort<int>, this->array, 0, this->n - 1, ng::algo::hmof3partition<int>
		);

		this->update();

	}

	this->save(filepath, std::ios_base::out);
	this->reset();

}

void Benchmark::combinedSortingRun() {

	while (this->power <= 26) {

		for (int i = 5; i < 300; i += 30) {

			this->sortRun(
				std::bitset<6>((i - 5) / 30).to_string() + ". cquicksort & " + std::to_string(i),
				ng::algo::shuffle<int>, ng::algo::cquicksort<int>, this->array, 0, this->n - 1, i,
				ng::algo::lmof3partition<int>
			);

		}

		for (int i = 5; i < 300; i += 30) {

			this->sortRun(
				std::bitset<6>((i - 5) / 30 + 10).to_string() + ". cmerge sort & " + std::to_string(i),
				ng::algo::shuffle<int>, ng::algo::cmergeSort<int>, this->array, 0, this->n - 1, i
			);

		}

		this->update();

	}

	this->save(this->combinedSorting, std::ios_base::out);
	this->reset();

}

template <typename ShuffleFunc>
void Benchmark::parallelSortingRun(ShuffleFunc shuffleFunc) {

	while (this->power <= 26) {

		this->sortRun("1. std::sort", shuffleFunc, std::sort<int*>, this->array, this->array + this->n);
		this->sortRun("2. cquicksort", shuffleFunc, ng::algo::cquicksort<int>, this->array, 0, this->n - 1, 5, ng::algo::hmof3partition<int>);
		this->sortRun("3. parallel quicksort", shuffleFunc, ng::algo::pquicksort<int*>, this->array, this->array + this->n - 1);
		this->sortRun("4. combined merge sort", shuffleFunc, ng::algo::cmergeSort<int>, this->array, 0, this->n - 1, 5);
		this->sortRun("5. parallel merge sort", shuffleFunc, ng::algo::pmergeSort<int>, this->array, 0, this->n - 1);

		this->update();

	}

	this->save(this->parallelSorting, std::ios_base::out);
	this->reset();

	for (int i = 0; i < 3; i++) {

		while (this->power <= 26) {

			this->sortRun("1. std::sort", shuffleFunc, std::sort<int*>, this->array, this->array + this->n);
			this->sortRun("2. cquicksort", shuffleFunc, ng::algo::cquicksort<int>, this->array, 0, this->n - 1, 5, ng::algo::hmof3partition<int>);
			this->sortRun("3. parallel quicksort", shuffleFunc, ng::algo::pquicksort<int*>, this->array, this->array + this->n - 1);
			this->sortRun("4. combined merge sort", shuffleFunc, ng::algo::cmergeSort<int>, this->array, 0, this->n - 1, 5);
			this->sortRun("5. parallel merge sort", shuffleFunc, ng::algo::pmergeSort<int>, this->array, 0, this->n - 1);

			this->update();

		}

		this->save(this->parallelSorting, std::ios_base::app);
		this->reset();

	}

}

// constructor / destructor

Benchmark::Benchmark(int power) {

	this->power = power;
	this->spower = this->power;
	this->n = static_cast<int>(std::pow(2, this->power));

	this->array = new int[this->n];
	for (int i = 0; i < this->n; i++)
		this->array[i] = i;

	this->tamount = 7;

	// init filepath
	this->n2ShuffleArray = "../Files/n2/shuffleArray.txt";
	this->n2AlmostShuffleArray = "../Files/n2/almostShuffleArray.txt";
	this->n2ReverseArray = "../Files/n2/reverseArray.txt";

	this->nlognShuffleArray = "../Files/nlogn/shuffleArray.txt";
	this->nlognAlmostShuffleArray = "../Files/nlogn/almostShuffleArray.txt";
	this->nlognReverseArray = "../Files/nlogn/reverseArray.txt";

	this->qpShuffleArray = "../Files/quicksortPartitions/shuffleArray.txt";
	this->qpAlmostShuffleArray = "../Files/quicksortPartitions/almostShuffleArray.txt";
	this->qpReverseArray = "../Files/quicksortPartitions/reverseArray.txt";

	this->combinedSorting = "../Files/combinedSorting/combinedSorting.txt";

	this->parallelSorting = "../Files/parallelSorting/parallelSorting.txt";

}

Benchmark::~Benchmark() {

	// delete array
	delete [] this->array;

}

// public methods

void Benchmark::run() {

	std::cout << "running..." << std::endl;

	// n2 run
	this->n2Run(ng::algo::shuffle<int>, this->n2ShuffleArray);
	this->n2Run(ng::algo::almostShuffle<int>, this->n2AlmostShuffleArray);
	this->n2Run(ng::algo::reverse<int>, this->n2ReverseArray);

	// nlogn 2 run
	this->nlognRun(ng::algo::shuffle<int>, this->nlognShuffleArray);
	this->nlognRun(ng::algo::almostShuffle<int>, this->nlognAlmostShuffleArray);

	// quicksort partitions run
	this->quicksortPartitionsRun(ng::algo::shuffle<int>, this->qpShuffleArray);
	this->quicksortPartitionsRun(ng::algo::almostShuffle<int>, this->qpAlmostShuffleArray);

	// combined sorting run
	this->combinedSortingRun();

	// parallel sorting
	this->parallelSortingRun(ng::algo::shuffle<int>);

	std::cout << "end (((" << std::endl;

}