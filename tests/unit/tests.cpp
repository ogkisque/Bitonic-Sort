#include "bitonic.hpp"
#include <gtest/gtest.h>
#include <ctime>
#include <vector>
#include <random>
#include <algorithm>

TEST(OpenCLTest, PlatformTest) {
    std::vector<cl::Platform> platforms;
    cl::Platform::GetPlatforms(platforms);
    cl::Platform platform1 = platforms.front();

    cl::Platform platform2 = platform1;
    ASSERT_EQ(platform1(), platform2());

    cl::Platform platform3 = std::move(platform2);
    ASSERT_EQ(platform1.Get(), platform3.Get());
}

TEST(OpenCLTest, DeviceTest) {
    std::vector<cl::Platform> platforms;
    cl::Platform::GetPlatforms(platforms);
    cl::Platform platform = platforms.front();
    
    std::vector<cl::Device> devices;
    platform.GetDevices(CL_DEVICE_TYPE_ALL, devices);
    cl::Device device1 = devices.front();

    cl::Device device2 = device1;
    ASSERT_EQ(device1.Get(), device2.Get());

    cl::Device device3 = std::move(device2);
    ASSERT_EQ(device1.Get(), device3.Get());
}

TEST(OpenCLTest, ContextTest) {
    std::vector<cl::Platform> platforms;
    cl::Platform::GetPlatforms(platforms);
    cl::Platform platform = platforms.front();
    std::vector<cl::Device> devices;
    platform.GetDevices(CL_DEVICE_TYPE_ALL, devices);
    cl::Device device = devices.front();
    
    cl::Context context1(device);

    ASSERT_EQ(context1.template GetInfo<CL_CONTEXT_REFERENCE_COUNT>(), 1U);
    ASSERT_EQ(context1.template GetInfo<CL_CONTEXT_NUM_DEVICES>(), 1U);

    auto vector = context1.template GetInfo<CL_CONTEXT_DEVICES>(); 
    ASSERT_EQ(vector.front(), device.Get());
    ASSERT_EQ(context1.GetDevice().Get(), device.Get());
    cl::Context context2 = context1;
    ASSERT_EQ(context1.Get(), context2.Get());
    ASSERT_EQ(context1.template GetInfo<CL_CONTEXT_REFERENCE_COUNT>(), 2U);
    cl::Context context3 = std::move(context1);
    ASSERT_EQ(context2.Get(), context3.Get());
    ASSERT_EQ(context3.template GetInfo<CL_CONTEXT_REFERENCE_COUNT>(), 2U);
}

TEST(OpenCLTest, CommandQueueTest) {
    std::vector<cl::Platform> platforms;
    cl::Platform::GetPlatforms(platforms);
    cl::Platform platform = platforms.front();
    std::vector<cl::Device> devices;
    platform.GetDevices(CL_DEVICE_TYPE_ALL, devices);
    cl::Device device = devices.front();
    cl::Context context(device);

    cl::CommandQueue queue(context);
    ASSERT_EQ(queue.template GetInfo<CL_QUEUE_CONTEXT>(), context.Get());
    ASSERT_EQ(queue.template GetInfo<CL_QUEUE_DEVICE>(), device.Get());
    ASSERT_EQ(queue.template GetInfo<CL_QUEUE_REFERENCE_COUNT>(), 1U);

    cl::CommandQueue queue1(queue);
    cl::CommandQueue queue2(queue1);

    ASSERT_EQ(queue.Get(), queue1.Get());
    ASSERT_EQ(queue1.Get(), queue2.Get());
    ASSERT_EQ(queue2.template GetInfo<CL_QUEUE_REFERENCE_COUNT>(), 3U);

    cl::CommandQueue queue3(std::move(queue2));
    ASSERT_EQ(queue.Get(), queue3.Get());
    ASSERT_EQ(queue3.template GetInfo<CL_QUEUE_REFERENCE_COUNT>(), 3U);
}

TEST(OpenCLTest, ProgramTest) {
    const char *program_text = "__kernel void add(__global int *inputA, __global int *inputB, __global int *output)\n"\
    "{\n"\
    "   size_t id = get_global_id(0);\n"\
    "   output[id] = inputA[id] + inputB[id];\n"\
    "}\n";

    std::string program_src = program_text;

    std::vector<cl::Platform> platforms;
    cl::Platform::GetPlatforms(platforms);
    cl::Platform platform = platforms.front();    
    std::vector<cl::Device> devices;
    platform.GetDevices(CL_DEVICE_TYPE_ALL, devices);
    cl::Device device = devices.front();
    cl::Context context(device);

    cl::Program program(context, &program_text);
    ASSERT_EQ(program.template GetInfo<CL_PROGRAM_REFERENCE_COUNT>(), 1U);
    ASSERT_EQ(program.template GetInfo<CL_PROGRAM_CONTEXT>(), context.Get());
    ASSERT_EQ(program.template GetInfo<CL_PROGRAM_NUM_DEVICES>(), 1U);
    auto device_vec = program.template GetInfo<CL_PROGRAM_DEVICES>();
    ASSERT_EQ(device_vec.front(), device.Get());
    ASSERT_EQ(program.template GetInfo<CL_PROGRAM_NUM_KERNELS>(), 1U);

    ASSERT_EQ(program.template GetInfo<CL_PROGRAM_SOURCE>(), program_src + '\0');

    cl::Program program1 = program;
    cl::Program program2 = program1;

    ASSERT_EQ(program.Get(), program1.Get());
    ASSERT_EQ(program1.Get(), program2.Get());
    ASSERT_EQ(program2.template GetInfo<CL_PROGRAM_REFERENCE_COUNT>(), 3U);

    cl::Program program3(std::move(program2));
    ASSERT_EQ(program.Get(), program3.Get());
    ASSERT_EQ(program3.template GetInfo<CL_PROGRAM_REFERENCE_COUNT>(), 3U); 
}

TEST(OpenCLTest, BufferTest) {
    std::vector<cl::Platform> platforms;
    cl::Platform::GetPlatforms(platforms);
    cl::Platform platform = platforms.front();
    std::vector<cl::Device> devices;
    platform.GetDevices(CL_DEVICE_TYPE_ALL, devices);
    cl::Device device = devices.front();
    cl::Context context(device);

    cl::CommandQueue queue(context);
    cl::Buffer buf1{queue, CL_MEM_WRITE_ONLY, 1024U};
    ASSERT_EQ(buf1.template GetInfo<CL_MEM_TYPE>(), CL_MEM_OBJECT_BUFFER);
    ASSERT_EQ(buf1.template GetInfo<CL_MEM_FLAGS>(), CL_MEM_WRITE_ONLY);
    ASSERT_EQ(buf1.template GetInfo<CL_MEM_SIZE>(), 1024U);
    ASSERT_EQ(buf1.template GetInfo<CL_MEM_REFERENCE_COUNT>(), 1U);
    ASSERT_EQ(buf1.template GetInfo<CL_MEM_CONTEXT>(), context.Get());

    cl::Buffer buf2 = buf1;
    ASSERT_EQ(buf2.Get(), buf1.Get());
    ASSERT_EQ(buf1.template GetInfo<CL_MEM_REFERENCE_COUNT>(), 2U);

    cl::Buffer buf3 = std::move(buf1);
    ASSERT_EQ(buf3.template GetInfo<CL_MEM_REFERENCE_COUNT>(), 2U);
//  
    std::vector<int> in1;
    in1.reserve(1024);
    for (int i = 0; i < 1024; i++)
        in1.push_back(i);

    cl::Buffer buffer{queue, in1.begin(), in1.end(), CL_MEM_READ_WRITE};
    ASSERT_EQ(buffer.template GetInfo<CL_MEM_SIZE>(), in1.size() * sizeof(int));
    
    std::vector<int> out1;
    out1.resize(1024);
    buffer.Read(out1.begin());

    ASSERT_EQ(out1, in1);

    std::vector<int> in2;
    in2.reserve(1024);
    for (int i = 0; i < 1024; i++)
        in2.push_back(3 * i);
    
    buffer.Write(in2.begin(), in2.end());
    
    std::vector<int> out2;
    out2.resize(1024);
    buffer.Read(out2.begin());

    ASSERT_EQ(out2, in2);
}

TEST(OpenCLTest, KernelTest) {
    const char *program_text = "__kernel void add(__global int *first, __global int *second, __global int *output)\n"\
    "{\n"\
    "   size_t id = get_global_id(0);\n"\
    "   output[id] = second[id] + first[id];\n"\
    "}\n";

    std::string program_src = program_text;

    std::vector<cl::Platform> platforms;
    cl::Platform::GetPlatforms(platforms);
    cl::Platform platform = platforms.front();    
    std::vector<cl::Device> devices;
    platform.GetDevices(CL_DEVICE_TYPE_ALL, devices);
    cl::Device device = devices.front();
    cl::Context context(device);
    cl::CommandQueue queue(context);
    cl::Program program(context, &program_text);
    cl::Kernel kernel(program, queue, "add");
    ASSERT_EQ(kernel.template GetInfo<CL_KERNEL_NUM_ARGS>(), 3U);
    ASSERT_EQ(kernel.template GetInfo<CL_KERNEL_REFERENCE_COUNT>(), 1U);
    ASSERT_EQ(kernel.template GetInfo<CL_KERNEL_CONTEXT>(), context.Get());
    ASSERT_EQ(kernel.template GetInfo<CL_KERNEL_PROGRAM>(), program.Get());
    ASSERT_EQ(kernel.template GetInfo<CL_KERNEL_FUNCTION_NAME>(), "add");

    cl::Buffer first(queue, CL_MEM_READ_ONLY, 1024U);
    cl::Buffer second(queue, CL_MEM_READ_ONLY, 1024U);
    cl::Buffer output(queue, CL_MEM_READ_WRITE, 1024U);
    kernel.SetArg(0, first); kernel.SetArg(1, second); kernel.SetArg(2, output);

    cl::Kernel ker1 = kernel;
    ASSERT_EQ(kernel.Get(), ker1.Get());
    ASSERT_EQ(kernel.template GetInfo<CL_KERNEL_REFERENCE_COUNT>(), 2U);
    cl::Kernel ker2 = std::move(kernel);
    ASSERT_EQ(ker2.Get(), ker1.Get());
    ASSERT_EQ(ker2.template GetInfo<CL_KERNEL_REFERENCE_COUNT>(), 2U);
}

TEST(StartCheck, AddVector) {
    const char *program_text = "__kernel void add(__global int *inputA, __global int *inputB, __global int *output)\n"\
    "{\n"\
    "   int id = get_global_id(0);\n"\
    "   if (id < 1024U) {\n"\
    "       output[id] = inputA[id] + inputB[id];\n"\
    "   }\n"\
    "}\n";

    std::string program_src = program_text;
    std::vector<cl::Platform> platforms;
    cl::Platform::GetPlatforms(platforms);
    cl::Platform platform = platforms.front();    
    std::vector<cl::Device> devices;
    platform.GetDevices(CL_DEVICE_TYPE_ALL, devices);
    cl::Device device = devices.front();
    cl::Context context(device);
    cl::Program program(context, &program_text);
    cl::CommandQueue queue(context);
    cl::Kernel kernel(program, queue, "add");

    std::vector<int> vec1, vec2, vec3;
    vec1.reserve(1024U); vec2.reserve(1024U); vec3.resize(1024U); 
    for (int i = 0; i < 1024U; i++) { vec1.push_back(i); vec2.push_back(2 * i); }
    
    cl::Buffer first(queue, vec1.begin(), vec1.end(), CL_MEM_READ_ONLY);
    cl::Buffer second(queue, vec2.begin(), vec2.end(), CL_MEM_READ_ONLY);
    cl::Buffer output(queue, CL_MEM_WRITE_ONLY, sizeof(int) * 1024U);
    ASSERT_EQ(output.template GetInfo<CL_MEM_SIZE>(), first.template GetInfo<CL_MEM_SIZE>());
    kernel.SetArg(0, first); kernel.SetArg(1, second); kernel.SetArg(2, output);
    kernel.Run(1024U);
    output.Read(vec3.begin());
    for (int i = 0; i < 1024U; i++)
        ASSERT_EQ(vec3[i], 3 * i);
}

TEST(StartCheck, BitonicSortTest1) {
    const int size1 = 16, size2 = 32, size3 = 64, 
              size4 = 128, size5 = 256, size6 = 512;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 100);

    std::vector<float> data1, data2, data3, data4, data5, data6;
    data1.reserve(size1); data2.reserve(size2); data3.reserve(size3);
    data4.reserve(size4); data5.reserve(size5); data6.reserve(size6);

    for (int i = 0; i < size1; i++) data1.push_back(distr(gen));
    for (int i = 0; i < size2; i++) data2.push_back(distr(gen));
    for (int i = 0; i < size3; i++) data3.push_back(distr(gen));
    for (int i = 0; i < size4; i++) data4.push_back(distr(gen));
    for (int i = 0; i < size5; i++) data5.push_back(distr(gen));
    for (int i = 0; i < size6; i++) data6.push_back(distr(gen));

    std::vector<float> data1_copy{data1}, data2_copy{data2}, data3_copy{data3}, 
                       data4_copy{data4}, data5_copy{data5}, data6_copy{data6};

    bitonic_sort::BitonicSort(data1);
    std::sort(data1_copy.begin(), data1_copy.end());
    ASSERT_EQ(data1, data1_copy);

    bitonic_sort::BitonicSort(data2);
    std::sort(data2_copy.begin(), data2_copy.end());
    ASSERT_EQ(data2, data2_copy);

    bitonic_sort::BitonicSort(data3);
    std::sort(data3_copy.begin(), data3_copy.end());
    ASSERT_EQ(data3, data3_copy);

    bitonic_sort::BitonicSort(data4);
    std::sort(data4_copy.begin(), data4_copy.end());
    ASSERT_EQ(data4, data4_copy);

    bitonic_sort::BitonicSort(data5);
    std::sort(data5_copy.begin(), data5_copy.end());
    ASSERT_EQ(data5, data5_copy);

    bitonic_sort::BitonicSort(data6);
    std::sort(data6_copy.begin(), data6_copy.end());
    ASSERT_EQ(data6, data6_copy);
}

TEST(StartCheck, BitonicSortTest2) {
    const int size1 = 16, size2 = 32, size3 = 64,
              size4 = 128, size5 = 256, size6 = 512,
              size7 = 262144, size8 = 1048576, size9 = 900000;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distr(0, 10);

    std::vector<float> data1, data2, data3, data4, data5, data6, data7, data8, data9;
    data1.reserve(size1); data2.reserve(size2); data3.reserve(size3);
    data4.reserve(size4); data5.reserve(size5); data6.reserve(size6);
    data7.reserve(size7); data8.reserve(size8); data9.reserve(size9);

    for (int i = 0; i < size1; i++) data1.push_back(distr(gen));
    for (int i = 0; i < size2; i++) data2.push_back(distr(gen));
    for (int i = 0; i < size3; i++) data3.push_back(distr(gen));
    for (int i = 0; i < size4; i++) data4.push_back(distr(gen));
    for (int i = 0; i < size5; i++) data5.push_back(distr(gen));
    for (int i = 0; i < size6; i++) data6.push_back(distr(gen));
    for (int i = 0; i < size7; i++) data7.push_back(distr(gen));
    for (int i = 0; i < size8; i++) data8.push_back(distr(gen));
    for (int i = 0; i < size9; i++) data9.push_back(distr(gen));

    std::vector<float> data1_copy{data1}, data2_copy{data2}, data3_copy{data3}, 
                       data4_copy{data4}, data5_copy{data5}, data6_copy{data6},
                       data7_copy{data7}, data8_copy{data8}, data9_copy{data9};

    bitonic_sort::BitonicSort(data1);
    std::sort(data1_copy.begin(), data1_copy.end());
    ASSERT_EQ(data1, data1_copy);

    bitonic_sort::BitonicSort(data2);
    std::sort(data2_copy.begin(), data2_copy.end());
    ASSERT_EQ(data2, data2_copy);

    bitonic_sort::BitonicSort(data3);
    std::sort(data3_copy.begin(), data3_copy.end());
    ASSERT_EQ(data3, data3_copy);

    bitonic_sort::BitonicSort(data4);
    std::sort(data4_copy.begin(), data4_copy.end());
    ASSERT_EQ(data4, data4_copy);

    bitonic_sort::BitonicSort(data5);
    std::sort(data5_copy.begin(), data5_copy.end());
    ASSERT_EQ(data5, data5_copy);

    bitonic_sort::BitonicSort(data6);
    std::sort(data6_copy.begin(), data6_copy.end());
    ASSERT_EQ(data6, data6_copy);

    bitonic_sort::BitonicSort(data7);
    std::sort(data7_copy.begin(), data7_copy.end());
    ASSERT_EQ(data7, data7_copy);

    bitonic_sort::BitonicSort(data8);
    std::sort(data8_copy.begin(), data8_copy.end());
    ASSERT_EQ(data8, data8_copy);

    bitonic_sort::BitonicSort(data9);
    std::sort(data9_copy.begin(), data9_copy.end());
    ASSERT_EQ(data9, data9_copy);
}

TEST(StartCheck, BitonicSortTest3) {
    const int size1 = 13, size2 = 24, size3 = 37,
              size4 = 99, size5 = 175, size6 = 431;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 100);

    std::vector<float> data1, data2, data3, data4, data5, data6;
    data1.reserve(size1); data2.reserve(size2); data3.reserve(size3);
    data4.reserve(size4); data5.reserve(size5); data6.reserve(size6);

    for (int i = 0; i < size1; i++) data1.push_back(distr(gen));
    for (int i = 0; i < size2; i++) data2.push_back(distr(gen));
    for (int i = 0; i < size3; i++) data3.push_back(distr(gen));
    for (int i = 0; i < size4; i++) data4.push_back(distr(gen));
    for (int i = 0; i < size5; i++) data5.push_back(distr(gen));
    for (int i = 0; i < size6; i++) data6.push_back(distr(gen));

    std::vector<float> data1_copy{data1}, data2_copy{data2}, data3_copy{data3}, 
                       data4_copy{data4}, data5_copy{data5}, data6_copy{data6};

    bitonic_sort::BitonicSort(data1);
    std::sort(data1_copy.begin(), data1_copy.end());
    ASSERT_EQ(data1, data1_copy);

    bitonic_sort::BitonicSort(data2);
    std::sort(data2_copy.begin(), data2_copy.end());
    ASSERT_EQ(data2, data2_copy);

    bitonic_sort::BitonicSort(data3);
    std::sort(data3_copy.begin(), data3_copy.end());
    ASSERT_EQ(data3, data3_copy);

    bitonic_sort::BitonicSort(data4);
    std::sort(data4_copy.begin(), data4_copy.end());
    ASSERT_EQ(data4, data4_copy);

    bitonic_sort::BitonicSort(data5);
    std::sort(data5_copy.begin(), data5_copy.end());
    ASSERT_EQ(data5, data5_copy);

    bitonic_sort::BitonicSort(data6);
    std::sort(data6_copy.begin(), data6_copy.end());
    ASSERT_EQ(data6, data6_copy);
}