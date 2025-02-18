#pragma once
#include "cl.hpp"
#include <vector>
#include <limits>
#include <cmath>

namespace bitonic_sort {

namespace details {
    template <typename IterT>
    void BitonicSortBase(IterT start_it, IterT end_it, int size) {
        std::vector<cl::Platform> platforms;
        cl::Platform::GetPlatforms(platforms);
        cl::Platform platform = platforms.front();

        std::vector<cl::Device> devices;
        platform.GetDevices(CL_DEVICE_TYPE_ALL, devices);
        cl::Device device = devices.front();

        cl::Context context(device);

        cl::CommandQueue queue(context);
        std::string prog_name = "include/bit.cl";
        cl::Program program(context, prog_name);

        std::string func_name = "bitonic_sort";
        cl::Kernel kernel(program, queue, func_name);

        cl::Buffer buf(queue, start_it, end_it, CL_MEM_READ_WRITE);

        kernel.SetArg(0, buf);
        kernel.SetArg(1, size);

        size_t global_work_size = size;
        kernel.Run(global_work_size);
        queue.Finish();

        buf.Read(start_it);
    }

    template <typename T>
    int ComplementVector(std::vector<T> &data, int size) {
        int size_pow2 = std::pow(2, static_cast<int>(std::log2(size)) + 1);
        T max_elem = std::numeric_limits<T>::max();
        for (int i = size; i < size_pow2; i++)
            data.push_back(max_elem);
        
        return size_pow2;
    }

} // namespace details

template <typename T>
void BitonicSort(std::vector<T> &data) {
    int size = data.size();
    int new_size = size;

    if (size == 0 || size == 1)
        return;

    if (!((size & (size - 1)) == 0)) // not power of 2
        new_size = details::ComplementVector(data, size);

    details::BitonicSortBase(data.begin(), data.end(), new_size);

    if (!((size & (size - 1)) == 0)) { // not power of 2
        for (int i = size; i < new_size; i++)
            data.pop_back();
    }
}

} // namespace bitonic_sort
