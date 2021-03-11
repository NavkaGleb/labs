#include <iostream>

#include <Random/Random.hpp>

#include "OptimalBinarySearchTree.hpp"

template <typename Iterator>
void PrintData(Iterator begin, Iterator end) {
    fort::char_table table;

    table << fort::header << "Keys" << "Values" << "Prob" << fort::endr;

    for (auto it = begin; it != end; ++it)
        table << it->Key << it->Value << it->Probability << fort::endr;

    std::cout << table.to_string() << std::endl;
}

int main() {
    const std::size_t size = 4;

    Ng::OptimalBinarySearchTree<int, int> tree;
    std::vector<decltype(tree)::DataCell> data(size);

    for (int i = 0; i < size; ++i)
        data[i] = { (i + 1) * 10, (i + 1) * 10, Ng::Random::Get(0.0f, 1.0f) };

    data[0].Probability = 4;
    data[1].Probability = 2;
    data[2].Probability = 6;
    data[3].Probability = 3;

    tree.SetData(data);

    PrintData(data.begin(), data.end());

    for (const auto& [key, value] : tree)
        std::cout << key << " -> " << value << std::endl;

    tree.Print();
    tree.PrintTable();

    return 0;
}
