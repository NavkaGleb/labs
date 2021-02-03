#include <iostream>
#include <string>
#include <optional>

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
    Ng::StaticHashTable<int, 10> hashTable;
    int ranad;

    std::cout << hashTable.GetSize() << std::endl;

    for (int i = 0; i < 6; ++i) {
        int rand = Ng::Random::Get(0, 5943);
        std::cout << rand << std::endl;

        hashTable.Push(rand);
        ranad = rand;
    }

    std::cout << std::endl;
    hashTable.Print();

    std::cout << "Find: " << *hashTable.Find(ranad) << std::endl;

    return 0;
}
