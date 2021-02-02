#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "../database/DataBase.h"

class Benchmark {
private:
	// inner class
	class BenchmarkData {
	public:
		// variables
		unsigned int amount;
		std::map<std::string, double> time;
		unsigned int foundRecords;
		unsigned long long fileSize;
		bool freeLocations;

		// constructor
		explicit BenchmarkData(unsigned int amount)
			: amount(amount), foundRecords(0), fileSize(0), freeLocations(true) {}

		// methods
		void pushTime(const std::string& key, const double& t) {

			this->time[key] = t;

		}

		friend std::ostream& operator <<(std::ostream& out, const BenchmarkData& benchData) {

			out << "monsters amount = " << benchData.amount << '\n';
			for (auto& t : benchData.time)
				out << t.first << ": " << t.second << '\n';
			out << "found records: " << benchData.foundRecords << '\n';
			out << "free locations: " << benchData.freeLocations << '\n';
			out << "file size: " << benchData.fileSize << " bytes" << '\n';

			return out;

		}

	};

	// variables
	DataBase& database;
	unsigned int n;
	unsigned int copyn;
	unsigned int _n;
	clock_t startTime;
	clock_t currentTime;
	double iterationTime;
	unsigned int counter;
	bool freeLocations;
	std::vector<BenchmarkData*> data;
	std::vector<Monster*> mstorage;
	std::vector<std::pair<Monster*, Location*>> mlstorage;

	std::string filepath1;
	std::string filepath2;
	std::string filepath3;
	std::string filepath4;
	std::string bfilepath;

	// search
	std::string nameFragment;
	unsigned short hp;
	unsigned short damage;
	unsigned short attackType;
	float specialAttack;
	DateTime datetime;
	double leftArea;
	double rightArea;

	// methods
	void reset();
	void update();
	void clearMStorage();
	void clearMLStorage();
	void save(const std::string& name, const std::string& path);
	void runSimpleFile();
	void runBinaryFile();

public:
	// constructor / destructor
	Benchmark(DataBase& database, unsigned int n);
	~Benchmark();

	// methods
	void run();
};

#endif // BENCHMARK_H