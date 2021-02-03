#include <iostream>
#include <string>
#include <optional>
#include <map>

#include <Random/Random.hpp>

#include "HashTable.hpp"

enum class Category : int { First = 0, Second };

struct News {
    std::string Title;
    Category    Category;
};

int main() {
    Ng::StaticHashTable<int, std::string, 10> hashTable;

    std::cout << hashTable.GetSize() << std::endl;

    for (int i = 0; i < 6; ++i) {
        int rand = Ng::Random::Get(0, 100);
        std::cout << rand << std::endl;

        auto& [key, value] = *hashTable.Push(rand, std::to_string(Ng::Random::Get(0, 10)));
    }

    std::cout << std::endl;
    hashTable.Print();
//
//    for (int i = 0; i < 10; ++i) {
//        int rand = Ng::Random::Get(0, 100);
//
//        std::cout << rand << " " << hashTable.IsExists(rand) << std::endl;
//    }

    hashTable[32] = "Fuck this shit";

    for (const auto& [key, value] : hashTable)
        std::cout << "Key: " << key << ", Value: " << value << std::endl;

    return 0;
}
