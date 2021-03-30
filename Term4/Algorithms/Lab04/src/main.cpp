#include <iostream>

#include <Random/Random.hpp>

#include "OptimalBinarySearchTree.hpp"

enum class Category : int {
    None = 0,
    Sport,
    Politics,
    Science,
    Economics
};

std::ostream& operator <<(std::ostream& ostream, const Category& category) {
    switch (category) {
        case Category::None:      ostream << "None";      break;
        case Category::Sport:     ostream << "Sport";     break;
        case Category::Politics:  ostream << "Politics";  break;
        case Category::Science:   ostream << "Science";   break;
        case Category::Economics: ostream << "Economics"; break;
    }

    return ostream;
}

template <typename Iterator>
void PrintData(Iterator begin, Iterator end) {
    fort::char_table table;

    table << fort::header << "Keys" << "Values" << "Prob" << fort::endr;

    for (auto it = begin; it != end; ++it)
        table << it->Key << it->Value << it->Probability << fort::endr;

    std::cout << table.to_string() << std::endl;
}

int main() {
    const std::size_t size = 7;

    Ng::OptimalBinarySearchTree<int, Category> tree;
    std::vector<decltype(tree)::DataCell> data(size);

    for (int i = 0; i < size; ++i) {
        data[i] = {
            i,
            static_cast<Category>(Ng::Random::Get<int>(1, 4)),
            1.0f / size
        };
    }

    tree.SetData(data);

    PrintData(data.begin(), data.end());

    // Range based for
    for (const auto& [key, value] : tree)
        std::cout << "{ " << key << " -> " << value << " }" << std::endl;
    std::cout << std::endl;

    // Keys
    std::cout << "Keys: ";
    for (const auto& key : tree.GetKeys())
        std::cout << key << " ";
    std::cout << std::endl;

    // Values
    std::cout << "Values: ";
    for (const auto& value : tree.GetValues())
        std::cout << value << " ";
    std::cout << std::endl;

    std::cout << "Size: " << tree.GetSize() << std::endl;
    std::cout << "Height: " << tree.GetHeight() << std::endl;

    tree.Print();
    tree.PrintTable();

    return 0;
}
