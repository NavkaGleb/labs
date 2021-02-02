#pragma once

#include <random>
#include <ctime>

namespace ng {

	class rand {
	public:
		// static methods
		static int irand(const int& low, const int& high);
		static float frand(const float& low, const float& high);

	};

}