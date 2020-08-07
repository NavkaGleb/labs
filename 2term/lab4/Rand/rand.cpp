#include "rand.hpp"

// static methods
int ng::rand::irand(const int& low, const int& high) {

	static std::random_device rd;
	std::seed_seq seed{rd(), static_cast<unsigned int>(time(nullptr))};
	static std::mt19937 mt(std::clock());

	std::uniform_int_distribution<int> dist(low, high);

	return dist(mt);

}

float ng::rand::frand(const float& low, const float& high) {

	static std::random_device rd;
	std::seed_seq seed{rd(), static_cast<unsigned int>(time(nullptr))};
	static std::mt19937 mt(std::clock());

	std::uniform_real_distribution<float> dist(low, high);

	return dist(mt);

}