#include <iostream>

#include <Random/Random.hpp>

#include "BinomialHeap.hpp"

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

struct News {
    std::string Name;
    Category Category;
};

std::ostream& operator <<(std::ostream& ostream, const News& news) {
    return ostream << news.Name << " (" << news.Category << ")";
}

struct Comparator {
    bool operator ()(const News& lhs, const News& rhs) {
        return std::stoi(lhs.Name) < std::stoi(rhs.Name);
    }
};

int main() {
    Ng::BinomialHeap<News, Comparator> heap;

    for (int i = 0; i < 20; ++i) {
//        heap.Push(Ng::Random::Get(-50, 50));
        heap.Push({ std::to_string(i), static_cast<Category>(Ng::Random::Get(1, 4)) });
    }

    heap.Print();

    while (!heap.IsEmpty())
        heap.PopPeak();

    std::cout << "after" << std::endl;
    heap.Print();

    return 0;
}
