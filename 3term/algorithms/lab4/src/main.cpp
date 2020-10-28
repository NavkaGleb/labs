#include "Solution.hpp"
#include "ConsoleColor.hpp"

int main() {
    using Color = Ng::Console::Color;

    Ng::Solution<int> solution;
    std::vector<decltype(solution)::Pair> data;

    solution.Rand(7);
    data = solution.Data();

    std::cout << Color::Green() << "Init" << Color::Reset() << std::endl;
    std::cout << solution << std::endl;
    std::cout << std::endl;

    std::cout << Color::Blue() << "Sort12 (O(n), stable)" << Color::Reset() << std::endl;
    solution.Sort12();
    std::cout << solution << std::endl;
    std::cout << std::endl;

    std::cout << Color::Blue() << "Sort13 (O(n), extra memory O(1))" << Color::Reset() << std::endl;
    solution.Data(data);
    solution.Sort13();
    std::cout << solution << std::endl;
    std::cout << std::endl;

    std::cout << Color::Blue() << "Sort23 (stable, extra memory O(1))" << Color::Reset() << std::endl;
    solution.Data(data);
    solution.Sort23();
    std::cout << solution << std::endl;
    std::cout << std::endl;

    return 0;
}
