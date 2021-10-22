#include "Benchmark.h"

/* ------------------------------------------ BenchmarkData class ------------------------------------------ */

// constructor

Benchmark::BenchmarkData::BenchmarkData() : amount(0) {}

// methods

void Benchmark::BenchmarkData::addMeasurement(const std::string& key, const long long time) {

	this->measurements[key].emplace_back(time);

}

/* ------------------------------------------ Benchmark class  ------------------------------------------ */

void Benchmark::update() {

	this->amount *= 2;

}

void Benchmark::reset() {

	this->amount = amount_;
	this->duration = 0;

}

template <class T>
void Benchmark::save(const std::vector<ng::DequeInterface<T>*>& containers, const char* path) {

	std::ofstream outfile(path);

	if (!outfile.is_open()) {

		std::cout << "failed to open the " << path << " file" << std::endl;
		return;

	}

	outfile << std::fixed;

	for (auto bdata : this->data) {

		outfile << "-------------------------- " << bdata->amount << " --------------------------" << std::endl;
		outfile << '\t' << '\t' << '\t' << '\t';

		for (auto& container : containers)
			outfile << container->get_allocator_type() << '\t';
		outfile << std::endl;

		for (auto& p : bdata->measurements) {

			outfile << p.first << ':' << '\t';

			for (auto& time : p.second)
				outfile << std::setprecision(3) << static_cast<double>(time) / CLOCKS_PER_SEC << '\t' << '\t';

			outfile << std::endl;

		}

		outfile << std::endl;

	}

	for (auto& bdata : this->data)
		delete bdata;
	this->data.clear();

	outfile.close();

}

template <class T>
long long Benchmark::sdequeRun(const std::vector<ng::DequeInterface<T>*>& containers, BenchmarkData* bdata) {

	/* simple deque run */

	long long bdataDurations = 0;
	clock_t mstart;
	clock_t start;
	clock_t end;

	for (auto& container : containers) {

		// push back
		mstart = clock();
		start = mstart;
		for (unsigned i = 0; i < this->amount; i++)
			container->pushBack(Circle(this->drand(this->mt), this->drand(this->mt), this->drand(this->mt)));
		bdata->addMeasurement("1. push back", clock() - start);

		// pop back
		start = clock();
		while (!container->empty())
			container->popBack();
		bdata->addMeasurement("2. pop back", clock() - start);

		// push front
		start = clock();
		for (unsigned i = 0; i < this->amount; i++)
			container->pushFront(Circle(this->drand(this->mt), this->drand(this->mt), this->drand(this->mt)));
		bdata->addMeasurement("3. push front", clock() - start);

		// pop front
		start = clock();
		while (!container->empty())
			container->popFront();
		bdata->addMeasurement("4. pop front", clock() - start);

		// push back
		start = clock();
		for (unsigned i = 0; i < this->amount; i++)
			container->pushBack(Circle(this->drand(this->mt), this->drand(this->mt), this->drand(this->mt)));
		bdata->addMeasurement("5. push back 2", clock() - start);

		// clear
		start = clock();
		container->clear();
		end = clock();
		bdata->addMeasurement("6. clear deque", end - start);
		bdata->addMeasurement("7. duration", end - mstart);

		bdataDurations += (end - mstart);

	}

	return bdataDurations / containers.size();

}

template <class T>
long long Benchmark::edequeRun(const std::vector<ng::DequeInterface<T>*>& containers, BenchmarkData* bdata) {

	/* experiment deque run */

	long long bdataDurations = 0;
	clock_t mstart;
	clock_t start;
	clock_t end;

	for (auto& container : containers) {

		// push back
		mstart = clock();
		start = mstart;
		for (unsigned i = 0; i < this->amount / 2; i++)
			container->pushBack(Circle(this->drand(this->mt), this->drand(this->mt), this->drand(this->mt)));
		bdata->addMeasurement("1. push b n/2", clock() - start);

		// pop front
		start = clock();
		for (unsigned i = 0; i < this->amount / 4; i++)
			container->popFront();
		bdata->addMeasurement("2. pop fr n/4", clock() - start);

		// push front
		start = clock();
		for (unsigned i = 0; i < this->amount / 4; i++)
			container->pushFront(Circle(this->drand(this->mt), this->drand(this->mt), this->drand(this->mt)));
		bdata->addMeasurement("3. push fr n/4", clock() - start);

		// pop back
		start = clock();
		for (unsigned i = 0; i < this->amount / 4; i++)
			container->popBack();
		bdata->addMeasurement("4. pop b n/4", clock() - start);

		// push back
		start = clock();
		for (unsigned i = 0; i < this->amount / 4 * 3; i++)
			container->pushBack(Circle(this->drand(this->mt), this->drand(this->mt), this->drand(this->mt)));
		bdata->addMeasurement("5. push b2 3/4", clock() - start);

		// clear
		start = clock();
		container->clear();
		end = clock();
		bdata->addMeasurement("6. clear deque", end - start);
		bdata->addMeasurement("7. duration", end - mstart);

		bdataDurations += (end - mstart);

	}

	return bdataDurations / containers.size();

}

template <class T>
void Benchmark::loopRun(const std::vector<ng::DequeInterface<T>*>& containers, const char* path, char func) {

	while (this->duration < this->stopTime * CLOCKS_PER_SEC) {

		auto* bdata = new BenchmarkData;
		bdata->amount = this->amount;

		if (func == 's')
			this->duration = this->sdequeRun(containers, bdata);
		else if (func == 'e')
			this->duration = this->edequeRun(containers, bdata);

		this->data.emplace_back(bdata);

		// update amount
		this->update();

	}

	this->save(containers, path);
	this->reset();

}

// constructor

Benchmark::Benchmark(unsigned amount, long long stopTime)
	: duration(0), mt(std::clock()) {

	this->lpath = "../files/benchmarkList.txt";
	this->apath = "../files/benchmarkArray.txt";
	this->epath = "../files/benchmarkExperiment.txt";

	this->amount = amount;
	this->amount_ = amount;
	this->stopTime = stopTime;

	// random
	this->drand = std::uniform_real_distribution<double>(-10000, 10000);

}

Benchmark::~Benchmark() {

	for (auto& bdata : this->data)
		delete bdata;
	this->data.clear();

}

// public methods

void Benchmark::run() {

	// local variables
	std::vector<ng::DequeInterface<Circle>*> ldeques;					// list deques
	std::vector<ng::DequeInterface<Circle>*> adeques;					// array deques

	// init list deques
	ldeques.emplace_back(new ng::Deque<Circle>);
	ldeques.emplace_back(new ngca::Deque<Circle>(10000000, 4000000));
	ldeques.emplace_back(new stdng::Deque<Circle>);

	// init array deques
	adeques.emplace_back(new ngvc::Deque<Circle>);
	adeques.emplace_back(new ng::Deque<Circle, ng::Array>);

	std::cout << "running benchmark..." << std::endl;

	// run loops
	this->loopRun(ldeques, this->lpath, 's');
	this->loopRun(adeques, this->apath, 's');
	this->loopRun(ldeques, this->epath, 'e');

	// clear deques
	for (auto& deq : ldeques)
		delete deq;

	for (auto& deq : adeques)
		delete deq;

	std::cout << "benchmark end(" << std::endl;

}
