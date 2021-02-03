#include <iostream>
#include <string>
#include <optional>
#include <map>

#include <Random/Random.hpp>

#include "HashTable.hpp"
#include "UniversalHash.hpp"

enum class Category : int { First = 0, Second };

struct News {
    std::string Title;
    Category    Category;
};

std::optional<std::string> Create(bool x) {
    return x ? std::optional<std::string>("Fuck") : std::nullopt;
}

int main() {
    Ng::StaticHashTable<int, std::string, 10> hashTable;

    std::cout << hashTable.GetSize() << std::endl;

    for (int i = 0; i < 6; ++i) {
        int rand = Ng::Random::Get(0, 5943);
        std::cout << rand << std::endl;

        hashTable.Push(rand, std::to_string(Ng::Random::Get(0, 10)));
    }

    std::cout << std::endl;
    hashTable.Print();

    return 0;
}
