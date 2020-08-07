#include "rand.hpp"

#include <random>
#include <ctime>

int ng::rand::irand(const int& left, const int& right) {

	static std::random_device rd;
	static std::seed_seq seq{rd(), static_cast<unsigned>(time(nullptr))};
	static std::mt19937 mt(seq);

	std::uniform_int_distribution dist(left, right);

	return dist(mt);

}