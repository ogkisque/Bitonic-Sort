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


// #include <stdio.h>
// #include <stdlib.h>
// #include <CL/cl.h>
// #include "cl.hpp"
// const int ARRAY_SIZE = 16;  // Must be a power of 2 for this implementation

// void initialize_data(int *data, int size) {
//     for (int i = 0; i < size; ++i) {
//         data[i] = rand() % 100; // Random numbers for sorting
//     }
// }

// void print_data(int *data, int size) {
//     for (int i = 0; i < size; i++) {
//         printf("%d ", data[i]);
//     }
//     printf("\n");
// }

// int main() {
//     int *data = (int *)malloc(sizeof(int) * ARRAY_SIZE);
//     initialize_data(data, ARRAY_SIZE);
    
//     printf("Unsorted data:\n");
//     print_data(data, ARRAY_SIZE);

//     // Set up OpenCL
//     cl_platform_id platform;
//     clGetPlatformIDs(1, &platform, NULL);

//     cl_device_id device;
//     clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

//     cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
//     cl_command_queue queue = clCreateCommandQueue(context, device, 0, NULL);
    
//     cl_mem data_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int) * ARRAY_SIZE, NULL, NULL);
//     clEnqueueWriteBuffer(queue, data_buffer, CL_TRUE, 0, sizeof(int) * ARRAY_SIZE, data, 0, NULL, NULL);

//     cl_program program;
//     std::ifstream stream("src/bit.cl");
//     auto text = std::string(std::istreambuf_iterator<char>(stream), 
//                             std::istreambuf_iterator<char>());
//     const char *kernel_source = text.data();
//     printf("%s\n", kernel_source);
//     program = clCreateProgramWithSource(context, 1, &kernel_source, NULL, NULL);
//     clBuildProgram(program, 1, &device, NULL, NULL, NULL);

//     cl_kernel kernel = clCreateKernel(program, "bitonic_sort", NULL);

//     // Perform bitonic sort
//     for (int k = 2; k <= ARRAY_SIZE; k *= 2) {
//         for (int j = k / 2; j > 0; j /= 2) {
//             clSetKernelArg(kernel, 0, sizeof(cl_mem), &data_buffer);
//             clSetKernelArg(kernel, 1, sizeof(int), &ARRAY_SIZE);
//             clSetKernelArg(kernel, 2, sizeof(int), &j);
//             clSetKernelArg(kernel, 3, sizeof(int), &k);

//             size_t global_work_size = ARRAY_SIZE;
//             clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
//             clFinish(queue);
//         }
//     }

//     // Read sorted data from the device
//     clEnqueueReadBuffer(queue, data_buffer, CL_TRUE, 0, sizeof(int) * ARRAY_SIZE, data, 0, NULL, NULL);

//     printf("Sorted data:\n");
//     print_data(data, ARRAY_SIZE);

//     // Clean up
//     clReleaseMemObject(data_buffer);
//     clReleaseKernel(kernel);
//     clReleaseProgram(program);
//     clReleaseCommandQueue(queue);
//     clReleaseContext(context);
//     free(data);

//     return 0;
// }
