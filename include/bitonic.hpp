#pragma once
#include "cl.hpp"

namespace bitonic_sort
{

template <typename IterT>
void bitonic_sort(IterT start_it, IterT end_it)
{
    int size = std::distance(start_it, end_it);
    cl::Platform platform;
    cl::Device device(platform);
    cl::Context context(device);

    cl::CommandQueue queue(context);
    std::string prog_name = "include/bit.cl";
    cl::Program program(context, prog_name);

    std::string func_name = "bitonic_sort";
    cl::Kernel kernel(program, queue, func_name);

    cl::Buffer buf(queue, start_it, end_it, CL_MEM_READ_WRITE);

    for (int k = 2; k <= size; k *= 2) {
        for (int j = k / 2; j > 0; j /= 2) {
            kernel.SetArg(0, buf);
            kernel.SetArg(1, size);
            kernel.SetArg(2, j);
            kernel.SetArg(3, k);

            size_t global_work_size = size;
            kernel.Run(global_work_size);
            queue.Finish();
        }
    }

    buf.Read(start_it);
}

} // namespace bitonic_sort
