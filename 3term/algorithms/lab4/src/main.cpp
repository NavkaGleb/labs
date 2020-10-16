#include "Lab4/Solution.hpp"

int main() {
    Ng::Solution solution;

    solution.Rand(5);
    std::cout << "Init" << std::endl;
    std::cout << solution << std::endl;

    std::cout << "Sort12" << std::endl;
    solution.Sort12();
    std::cout << solution << std::endl;

    solution.Rand(5);
    std::cout << "Init" << std::endl;
    std::cout << solution << std::endl;

    std::cout << "Sort13" << std::endl;
    solution.Sort13();
    std::cout << solution << std::endl;

    solution.Rand(5);
    std::cout << "Init" << std::endl;
    std::cout << solution << std::endl;

    std::cout << "Sort23" << std::endl;
    solution.Sort23();
    std::cout << solution << std::endl;

    return 0;
}
