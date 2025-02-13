#ifndef CL_TARGET_OPENCL_VERSION
#define CL_TARGET_OPENCL_VERSION 120
#endif

#include "cl.hpp"

#include <iostream>
#include <vector>

namespace {
    const char *ProgramSource =
    "__kernel void add(__global int *inputA, __global int *inputB, __global int *output)\n"\
    "{\n"\
    "   size_t id = get_global_id(0);\n"\
    "   output[id] = inputA[id] + inputB[id];\n"\
    "}\n";
}

int main() {
    cl::Platform platform;
    cl::Device device(platform);
    cl::Context context(device);

    cl::CommandQueue queue(context);
    cl::Program program(context, &ProgramSource);

    std::string func_name = "add";
    cl::Kernel kernel(program, queue, func_name);

    std::vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    cl::Buffer buf1(queue, v1.begin(), v1.end(), CL_MEM_READ_ONLY);
    std::vector<int> v2{11, 22, 33, 44, 55, 66, 77, 88, 99, 100, 111, 122, 133, 144, 155, 166};
    cl::Buffer buf2(queue, v2.begin(), v2.end(), CL_MEM_READ_ONLY);
    cl::Buffer buf_res(queue, CL_MEM_WRITE_ONLY, buf1.GetSize());

    kernel.SetArg(0, buf1);
    kernel.SetArg(1, buf2);
    kernel.SetArg(2, buf_res);

    kernel.Run(buf1.GetSize());

    std::vector<int> res = {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    buf_res.Copy(res.begin());

    for (auto &it : res)
        std::cout << it << std::endl;

    return 0;
}

