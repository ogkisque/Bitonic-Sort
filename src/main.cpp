#ifndef CL_TARGET_OPENCL_VERSION
#define CL_TARGET_OPENCL_VERSION 120
#endif

#include "bitonic.hpp"

#include <iostream>
#include <vector>

namespace {
    bool GetInput(std::vector<float> &data) {
        size_t size = 0;
        std::cin >> size;
        if (!std::cin.good() || size <= 0) {
            std::cout << "Incorrect data" << std::endl;
            return false;
        }

        data.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            double x = 0;
            std::cin >> x;
            data.push_back(x);
            if (!std::cin.good()) {
                std::cout << "Incorrect data" << std::endl;
                return false;
            }
        }
    
        return true;
    }
} // namespace

int main() {
    std::vector<float> data{};
    if (!GetInput(data))
        return 1;
    
    bitonic_sort::BitonicSort(data);

    for (auto it : data)
        std::cout << it << std::endl;

    return 0;
}