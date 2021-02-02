#include "Benchmark.h"

// private methods

void Benchmark::reset() {

	this->iterationTime = 0;
	this->n = copyn;
	this->startTime = 0;
	this->counter = 1;
	this->freeLocations = true;

	for (auto& e : this->data)
		delete e;
	this->data.clear();

}

void Benchmark::update() {

	if (this->iterationTime <= 1)
		this->n *= 2, this->_n = this->n;
	else
		this->n = this->_n * ++this->counter;

}

void Benchmark::clearMStorage() {

	for (auto& monster : this->mstorage)
		delete monster;
	this->mstorage.clear();

}

void Benchmark::clearMLStorage() {

	for (auto& p : this->mlstorage) {

		delete p.first;
		delete p.second;

	}

	this->mlstorage.clear();

}

void Benchmark::save(const std::string& name, const std::string& path) {

	std::ofstream outfile(path);

	outfile << "------------------- " << name << " -------------------" << '\n';
	for (auto* e : this->data)
		outfile << *e << '\n';

	outfile.close();

}

void Benchmark::runSimpleFile() {

	// ------------------- search by nameFragment -------------------

	while (this->iterationTime < 10 && this->freeLocations) {

		this->startTime = clock();

		auto* benchData = new BenchmarkData(this->n);

		this->freeLocations = this->database.createMonsters(false, this->n);
		benchData->freeLocations = this->freeLocations;
		benchData->pushTime("creating", static_cast<double>(clock() - this->startTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		this->database.saveMonstersToFile(false);
		benchData->pushTime("saving", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		this->database.deleteMonstersFromMemory();
		benchData->pushTime("clearing memory", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		benchData->fileSize = this->database.loadMonsters();
		benchData->pushTime("loading", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		this->database.search(this->nameFragment, &this->mstorage);
		benchData->pushTime("searching", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);
		benchData->foundRecords = this->mstorage.size();

		this->currentTime = clock();
		this->database.deleteMonstersFromMemory();
		this->database.deleteMonsters(true, false);
		this->clearMStorage();
		benchData->pushTime("clearing memory and file", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->iterationTime = static_cast<double>(clock() - this->startTime) / CLOCKS_PER_SEC;
		benchData->pushTime("iteration time", this->iterationTime);

		this->data.emplace_back(benchData);

		// update n
		this->update();

	}

	this->save("search by nameFragment", this->filepath1);
	this->reset();
	std::cout << "1st stage passed)(" << '\n';

	// ------------------- search by hp and by damage -------------------

	while (this->iterationTime < 10 && this->freeLocations) {

		this->startTime = clock();

		auto* benchData = new BenchmarkData(this->n);

		this->freeLocations = this->database.createMonsters(false, this->n);
		benchData->freeLocations = this->freeLocations;
		benchData->pushTime("creating", static_cast<double>(clock() - this->startTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		this->database.saveMonstersToFile(false);
		benchData->pushTime("saving", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		this->database.deleteMonstersFromMemory();
		benchData->pushTime("clearing memory", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		benchData->fileSize = this->database.loadMonsters();
		benchData->pushTime("loading", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		this->database.search(this->hp, this->damage, &this->mstorage);
		benchData->pushTime("searching", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);
		benchData->foundRecords = this->mstorage.size();

		this->currentTime = clock();
		this->database.deleteMonstersFromMemory();
		this->database.deleteMonsters(true, false);
		this->clearMStorage();
		benchData->pushTime("clearing memory and file", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->iterationTime = static_cast<double>(clock() - this->startTime) / CLOCKS_PER_SEC;
		benchData->pushTime("iteration time", this->iterationTime);

		this->data.emplace_back(benchData);

		// update n
		this->update();

	}

	this->save("search by hp and by damage", this->filepath2);
	this->reset();
	std::cout << "2nd stage passed)(" << '\n';

	// ------------------- search by attack type and by datetime -------------------

	while (this->iterationTime < 10 && this->freeLocations) {

		this->startTime = clock();

		auto* benchData = new BenchmarkData(this->n);

		this->freeLocations = this->database.createMonsters(false, this->n);
		benchData->freeLocations = this->freeLocations;
		benchData->pushTime("creating", static_cast<double>(clock() - this->startTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		this->database.saveMonstersToFile(false);
		benchData->pushTime("saving", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		this->database.deleteMonstersFromMemory();
		benchData->pushTime("clearing memory", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		benchData->fileSize = this->database.loadMonsters();
		benchData->pushTime("loading", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		this->database.search(this->attackType, this->datetime, &this->mstorage);
		benchData->pushTime("searching", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);
		benchData->foundRecords = this->mstorage.size();

		this->currentTime = clock();
		this->database.deleteMonstersFromMemory();
		this->database.deleteMonsters(true, false);
		this->clearMStorage();
		benchData->pushTime("clearing memory and file", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->iterationTime = static_cast<double>(clock() - this->startTime) / CLOCKS_PER_SEC;
		benchData->pushTime("iteration time", this->iterationTime);

		this->data.emplace_back(benchData);

		// update n
		this->update();

	}

	this->save("search by attack type and by datetime", this->filepath3);
	this->reset();
	std::cout << "3rd stage passed)(" << '\n';

	// ------------------- search by area range, by attackType and by specialAttack -------------------

	while (this->iterationTime < 10 && this->freeLocations) {

		this->startTime = clock();

		auto* benchData = new BenchmarkData(this->n);

		this->freeLocations = this->database.createMonsters(false, this->n);
		benchData->freeLocations = this->freeLocations;
		benchData->pushTime("creating", static_cast<double>(clock() - this->startTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		this->database.saveMonstersToFile(false);
		benchData->pushTime("saving", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		this->database.deleteMonstersFromMemory();
		benchData->pushTime("clearing memory", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		benchData->fileSize = this->database.loadMonsters();
		benchData->pushTime("loading", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		this->database.search(this->leftArea, this->rightArea, this->attackType, this->specialAttack, &mlstorage);
		benchData->pushTime("searching", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);
		benchData->foundRecords = this->mlstorage.size();

		this->currentTime = clock();
		this->database.deleteMonstersFromMemory();
		this->database.deleteMonsters(true, false);
		this->clearMLStorage();
		benchData->pushTime("clearing memory and file", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->iterationTime = static_cast<double>(clock() - this->startTime) / CLOCKS_PER_SEC;
		benchData->pushTime("iteration time", this->iterationTime);

		this->data.emplace_back(benchData);

		// update n
		this->update();

	}

	this->save("search by area range, by attackType and by specialAttack", this->filepath4);
	this->reset();
	std::cout << "4th stage passed)(" << '\n';

}

void Benchmark::runBinaryFile() {

	// ------------------- search by nameFragment -------------------

	while (this->iterationTime < 10 && this->freeLocations) {

		this->startTime = clock();

		auto* benchData = new BenchmarkData(this->n);

		this->freeLocations = this->database.createMonsters(false, this->n);
		benchData->freeLocations = this->freeLocations;
		benchData->pushTime("creating", static_cast<double>(clock() - this->startTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		this->database.saveMonstersToBinFile(false);
		benchData->pushTime("saving", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		this->database.deleteMonstersFromMemory();
		benchData->pushTime("clearing memory", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		benchData->fileSize = this->database.loadBinMonsters();
		benchData->pushTime("loading", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->currentTime = clock();
		this->database.bsearch(this->nameFragment, &this->mstorage);
		benchData->pushTime("searching", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);
		benchData->foundRecords = this->mstorage.size();

		this->currentTime = clock();
		this->database.deleteMonstersFromMemory();
		this->database.deleteMonsters(false, true);
		this->clearMStorage();
		benchData->pushTime("clearing memory and file", static_cast<double>(clock() - this->currentTime) / CLOCKS_PER_SEC);

		this->iterationTime = static_cast<double>(clock() - this->startTime) / CLOCKS_PER_SEC;
		benchData->pushTime("iteration time", this->iterationTime);

		this->data.emplace_back(benchData);

		// update n
		this->update();

	}

	this->save("search by nameFragment", this->bfilepath);
	this->reset();
	std::cout << "5th stage passed" << '\n';

}

// constructor / destructor

Benchmark::Benchmark(DataBase& database, unsigned int n)
	: database(database) {

	// init variables
	this->n = n;
	this->copyn = n;
	this->_n = 0;
	this->startTime = 0;
	this->currentTime = 0;
	this->iterationTime = 0.f;
	this->counter = 1;
	this->freeLocations = true;

	// init file path
	this->filepath1 = "../files/benchmark1.txt";
	this->filepath2 = "../files/benchmark2.txt";
	this->filepath3 = "../files/benchmark3.txt";
	this->filepath4 = "../files/benchmark4.txt";
	this->bfilepath = "../files/bbenchmark1.txt";

	// random variables
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<unsigned short> ushort_rand(2, USHRT_MAX);
	std::uniform_real_distribution<float> float_rand(0, 1);
	std::uniform_real_distribution<double> double_rand(100, 2000);

	unsigned int wordLength = 3;

	// search variables
	for (unsigned int j = 0; j < ushort_rand(mt) % wordLength + 2; j++)
		this->nameFragment += SYMBOLS[ushort_rand(mt) % SYMBOL_AMOUNT];

	this->hp = ushort_rand(mt) % 50000;
	this->damage = ushort_rand(mt) % 1000;
	this->attackType = ushort_rand(mt) % AMOUNT_ATTACKS;
	this->datetime.setRandTime(mt, ushort_rand);
	this->leftArea = double_rand(mt);
	this->rightArea = double_rand(mt);
	this->specialAttack = float_rand(mt);

	if (leftArea > rightArea)
		std::swap(this->leftArea, this->rightArea);

	// clear files
	this->database.deleteMonsters(true, true);
	this->database.loadLocations();

}

Benchmark::~Benchmark() {

	for (auto& benchData : this->data)
		delete benchData;
	this->data.clear();

	this->clearMStorage();
	this->clearMLStorage();

}

// public methods

void Benchmark::run() {

	if (this->database.getLocations().empty()) {

		std::cout << "no locations. you must first create locations" << '\n';
		return;

	} else {

		std::cout << "running benchmark..." << '\n';

		this->runSimpleFile();
		this->runBinaryFile();

		std::cout << "benchmark end. you can check files with data" << '\n';

	}

}