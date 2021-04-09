#include <iomanip>

#include <Random/Random.hpp>

#include "BPlusTree.hpp"

int main() {
    constexpr int count = 100;

    Ng::BPlusTree<int, int, 2> tree;
    std::vector<int>           keys;

    keys.reserve(count);

    for (int i = 0; i < count; ++i) {
        auto key = Ng::Random::Get(0, 100);

        keys.push_back(key);
        tree.Push(key, 1);
    }

    std::sort(keys.begin(), keys.end());
    keys.erase(std::unique(keys.begin(), keys.end()), keys.end());

    tree.Print();

    tree.Get(keys[Ng::Random::Get<std::size_t>(0, keys.size() - 1)]) = 228;

    std::cout << "Count: " << tree.GetCount() << ", Height: " << tree.GetHeight() << std::endl;

    for (auto&& [key, value] : tree) {
        std::cout << "{ " << std::setw(3) << std::setfill('0') << key << ", " << value << " }" << std::endl;
    }

    while (!keys.empty()) {
        auto index = Ng::Random::Get<std::size_t>(0, keys.size() - 1);

        tree.Pop(keys[index]);
        keys.erase(keys.begin() + static_cast<decltype(keys)::difference_type>(index));
    }

    std::cout << "After Clearing!" << std::endl;

    tree.Print();

    return 0;
}
