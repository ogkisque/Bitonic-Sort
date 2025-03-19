#include "bitonic.hpp"
#include <benchmark/benchmark.h>
#include <random>

void CreateData(size_t size);
std::vector<float> data1;
std::vector<float> data2;

static void BM_BitonicSortTest1(benchmark::State& state) {
    CreateData(15'000'000);

    for (auto _ : state) {
        bitonic_sort::BitonicSort(data1);
        benchmark::DoNotOptimize(data1.data());
    }
}

static void BM_StdSortTest1(benchmark::State& state) {
    for (auto _ : state) {
        std::sort(data2.begin(), data2.end());
        benchmark::DoNotOptimize(data2);
    }
}

static void BM_BitonicSortTest2(benchmark::State& state) {
    CreateData(16'777'216);

    for (auto _ : state) {
        bitonic_sort::BitonicSort(data1);
        benchmark::DoNotOptimize(data1.data());
    }
}

static void BM_StdSortTest2(benchmark::State& state) {
    for (auto _ : state) {
        std::sort(data2.begin(), data2.end());
        benchmark::DoNotOptimize(data2);
    }
}

BENCHMARK(BM_BitonicSortTest1);
BENCHMARK(BM_StdSortTest1);
BENCHMARK(BM_BitonicSortTest2);
BENCHMARK(BM_StdSortTest2);

BENCHMARK_MAIN();

void CreateData(size_t size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 200000);
    data1.clear();
    data1.reserve(size);

    for (int i = 0; i < size; i++)
        data1.push_back(distr(gen));

    data2 = data1;
}