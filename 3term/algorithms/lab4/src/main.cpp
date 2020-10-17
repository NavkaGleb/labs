#include "Lab4/Solution.hpp"
#include "Lab4/ConsoleColor.hpp"

int main() {
    using ForeGround = Ng::Console::ForeGround;

    const ForeGround green = ForeGround::BoldGreen;
    const ForeGround blue = ForeGround::BoldBlue;
    const ForeGround reset(ForeGround::Color::Default);

    Ng::Solution solution;
    std::vector<Ng::Solution::Pair> data;

    solution.Rand(7);
    data = solution.Data();

    std::cout << green << "Init" << reset << std::endl;
    std::cout << solution << std::endl;
    std::cout << std::endl;

    std::cout << blue << "Sort12 (O(n), stable)" << reset << std::endl;
    solution.Sort12();
    std::cout << solution << std::endl;

    std::cout << blue << "Sort13 (O(n), extra memory O(1))" << reset << std::endl;
    solution.Data(data);
    solution.Sort13();
    std::cout << solution << std::endl;

    std::cout << blue << "Sort23 (stable, extra memory O(1))" << reset << std::endl;
    solution.Data(data);
    solution.Sort23();
    std::cout << solution << std::endl;

    return 0;
}
