#include <iostream>

#include <IsCyclical.hpp>
#include <Random.hpp>

std::string ShiftRight(const std::string& str, std::size_t offset) {
    std::string result;

    result.resize(str.size());

    for (std::size_t i = 0; i < str.size() - offset; ++i)
        result[i + offset] = str[i];

    for (std::size_t i = str.size() - offset; i < str.size(); ++i)
        result[i - str.size() + offset] = str[i];

    return result;
}

bool Check(const std::string& left, const std::string& right, const Ng::IsCyclical& isCyclical = Ng::IsCyclical()) {
    return isCyclical(left, right) != std::string::npos;
}

int main() {
    int a = 0;

    for (int i = 0; i < 10'000; ++i) {
        std::string text;
        int size = Ng::Random::Irand(50, 100);

        for (int j = 0; j < size; ++j)
            text.push_back(static_cast<char>(Ng::Random::Irand(0, 1) + '0'));

        std::string s = ShiftRight(text, Ng::Random::Irand<std::size_t>(1, text.size() - 1));

        a += Check(text, s);
    }

    std::cout << a << std::endl;
}
