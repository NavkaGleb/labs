#include <iostream>
#include <string>
#include <optional>
#include <map>

#include "HashTable.hpp"

int main() {
    Ng::StaticHashTable<int, std::string, 10> hashTable;

    for (int i = 0; i < 6; ++i) {
        int rand = Ng::Random::Get(0, 100);
        std::cout << rand << std::endl;

        auto& [key, value] = *hashTable.Push(rand, std::to_string(Ng::Random::Get(0, 10)));
    }

    hashTable.Print();

    hashTable[32] = "Fuck this shit";

    for (const auto& [key, value] : hashTable)
        std::cout << "Key: " << key << ", Value: " << value << std::endl;

    return 0;
}
