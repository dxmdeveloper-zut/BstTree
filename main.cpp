#include <iostream>
#include <random>
#include "bst_tree.hpp"

template<class T>
T random_int(T min, T max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<T> dis(min, max);
    return dis(gen);
}

int main() {
    bst_tree<int> test_tree{};
    auto subcmpr = [](auto a, auto b) { return a - b; };
    for (size_t i = 0; i < 28; i++) {
        test_tree.add(random_int<int>(-100, 100), subcmpr);
    }

    std::cout << test_tree.to_string();
    std::cout << test_tree.to_html();

    return 0;
}
