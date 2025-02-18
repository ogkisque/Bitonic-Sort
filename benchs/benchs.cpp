#include <cassert>
#include <iostream>
#include <vector>
#include <ctime>
#include <vector>
#include <random>
#include <algorithm>

#include "hayai.hpp"
#include "hayai_main.hpp"
#include "bitonic.hpp"

std::vector<int> data1;
std::vector<int> data2;
std::vector<int> data1_copy;
std::vector<int> data2_copy;

BENCHMARK(Sort1, Bitonic, 1, 1) {
    bitonic_sort::BitonicSort(data1);
}

BENCHMARK(Sort1, Standart, 1, 1) {
    std::sort(data1_copy.begin(), data1_copy.end());
}

BENCHMARK(Sort2, Bitonic, 1, 1) {
    bitonic_sort::BitonicSort(data2);
}

BENCHMARK(Sort2, Standart, 1, 1) {
    std::sort(data2_copy.begin(), data2_copy.end());
}

int main() {
    const int size1 = 100000;
    const int size2 = 131072;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 200000);

    data1.reserve(size1);

    for (int i = 0; i < size1; i++)
        data1.push_back(distr(gen));

    data1_copy = data1;

    data2.reserve(size2);

    for (int i = 0; i < size2; i++)
        data2.push_back(distr(gen));
    data2_copy = data2;

    hayai::MainRunner runner;
    return runner.Run();
}