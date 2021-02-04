#include <iostream>
#include <string>

#include "StaticHashTable.hpp"

enum Category : int { Sport = 0, Policy, Science };

std::ostream& operator <<(std::ostream& ostream, const Category& category) {
    switch (category) {
        case Category::Sport:   ostream << "Sport";   break;
        case Category::Policy:  ostream << "Policy";  break;
        case Category::Science: ostream << "Science"; break;
    }

    return ostream;
}

int main() {
    Ng::StaticHashTable<std::string, Category, 10> hashTable;

    for (int i = 0; i < 10; ++i) {
        auto& [key, value] = *hashTable.Push(
            std::to_string(i),
            static_cast<Category>(Ng::Random::Get(0, 2))
        );
    }

    hashTable.Print();

    for (const auto& [key, value] : hashTable)
        std::cout << "Key: " << key << ", Value: " << value << std::endl;

    return 0;
}
