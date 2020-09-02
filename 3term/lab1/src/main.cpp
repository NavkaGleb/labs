#include <iostream>

#include <Array.hpp>

int main() {

	ng::Array<int> array = { 1, 5, 36, 94 };

	for (const auto& e : array)
		std::cout << e << " ";
	std::cout << std::endl;

	std::cout << array[1] << std::endl;

}