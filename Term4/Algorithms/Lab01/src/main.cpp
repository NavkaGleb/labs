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

    std::cout << hashTable.GetSize() << std::endl;

    auto it = hashTable.Push(3241);

    std::cout << hashTable.GetSize() << " " << *it << std::endl;

    return 0;
}
