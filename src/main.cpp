#ifndef CL_TARGET_OPENCL_VERSION
#define CL_TARGET_OPENCL_VERSION 120
#endif

#include "bitonic.hpp"

#include <iostream>
#include <vector>


int main() {
    std::vector<int> data{1, 6, 2, 7, 7, 2, 9, 3, 11, 14, 57, 19, 29, 31, 3, 9};
    bitonic_sort::bitonic_sort(data.begin(), data.end());

    for (auto &it : data)
        std::cout << it << std::endl;

    return 0;
}

