#include "cl.hpp"
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
    //ASSERT_NE(platform1(), NULL);

    cl::Platform platform2 = platform1;
    ASSERT_EQ(platform1(), platform2());

    cl::Platform platform3 = std::move(platform2);
    //ASSERT_EQ(platform2.Get(), NULL);
    ASSERT_EQ(platform1.Get(), platform3.Get());
}

TEST(OpenCLTest, DeviceTest) {
    std::vector<cl::Platform> platforms;
    cl::Platform::GetPlatforms(platforms);
    cl::Platform platform = platforms.front();
    
    std::vector<cl::Device> devices;
    platform.GetDevices(CL_DEVICE_TYPE_ALL, devices);
    cl::Device device1 = devices.front();

    //ASSERT_NE(device1.Get(), NULL);
    cl::Device device2 = device1;
    ASSERT_EQ(device1.Get(), device2.Get());

    cl::Device device3 = std::move(device2);
    //ASSERT_EQ(device2.Get(), NULL);
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

TEST(BitonicSortTest, Test1) {
    const int size1 = 16;
    const int size2 = 32;
    const int size3 = 64;
    const int size4 = 128;
    const int size5 = 256;
    const int size6 = 512;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 100);

    /////////////////////////////////////////////////////////////////////
    std::vector<float> data1;
    data1.reserve(size1);

    for (int i = 0; i < size1; i++)
        data1.push_back(distr(gen));

    std::vector<float> data1_copy{data1};

    bitonic_sort::BitonicSort(data1);
    std::sort(data1_copy.begin(), data1_copy.end());

    ASSERT_EQ(data1, data1_copy);

    /////////////////////////////////////////////////////////////////////
    std::vector<float> data2;
    data2.reserve(size2);

    for (int i = 0; i < size2; i++)
        data2.push_back(distr(gen));

    std::vector<float> data2_copy{data2};

    bitonic_sort::BitonicSort(data2);
    std::sort(data2_copy.begin(), data2_copy.end());

    ASSERT_EQ(data2, data2_copy);

    /////////////////////////////////////////////////////////////////////
    std::vector<float> data3;
    data3.reserve(size3);

    for (int i = 0; i < size3; i++)
        data3.push_back(distr(gen));

    std::vector<float> data3_copy{data3};

    bitonic_sort::BitonicSort(data3);
    std::sort(data3_copy.begin(), data3_copy.end());

    ASSERT_EQ(data3, data3_copy);

    /////////////////////////////////////////////////////////////////////
    std::vector<float> data4;
    data4.reserve(size4);

    for (int i = 0; i < size4; i++)
        data4.push_back(distr(gen));

    std::vector<float> data4_copy{data4};

    bitonic_sort::BitonicSort(data4);
    std::sort(data4_copy.begin(), data4_copy.end());

    ASSERT_EQ(data4, data4_copy);

    /////////////////////////////////////////////////////////////////////
    std::vector<float> data5;
    data5.reserve(size5);

    for (int i = 0; i < size5; i++)
        data5.push_back(distr(gen));

    std::vector<float> data5_copy{data5};

    bitonic_sort::BitonicSort(data5);
    std::sort(data5_copy.begin(), data5_copy.end());

    ASSERT_EQ(data5, data5_copy);

    /////////////////////////////////////////////////////////////////////
    std::vector<float> data6;
    data6.reserve(size6);

    for (int i = 0; i < size6; i++)
        data6.push_back(distr(gen));

    std::vector<float> data6_copy{data6};

    bitonic_sort::BitonicSort(data6);
    std::sort(data6_copy.begin(), data6_copy.end());

    ASSERT_EQ(data6, data6_copy);
}

TEST(BitonicSortTest, Test2) {
    const int size1 = 16;
    const int size2 = 32;
    const int size3 = 64;
    const int size4 = 128;
    const int size5 = 256;
    const int size6 = 512;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distr(0, 10);

    /////////////////////////////////////////////////////////////////////
    std::vector<float> data1;
    data1.reserve(size1);

    for (int i = 0; i < size1; i++)
        data1.push_back(distr(gen));

    std::vector<float> data1_copy{data1};

    bitonic_sort::BitonicSort(data1);
    std::sort(data1_copy.begin(), data1_copy.end());

    ASSERT_EQ(data1, data1_copy);

    /////////////////////////////////////////////////////////////////////
    std::vector<float> data2;
    data2.reserve(size2);

    for (int i = 0; i < size2; i++)
        data2.push_back(distr(gen));

    std::vector<float> data2_copy{data2};

    bitonic_sort::BitonicSort(data2);
    std::sort(data2_copy.begin(), data2_copy.end());

    ASSERT_EQ(data2, data2_copy);

    /////////////////////////////////////////////////////////////////////
    std::vector<float> data3;
    data3.reserve(size3);

    for (int i = 0; i < size3; i++)
        data3.push_back(distr(gen));

    std::vector<float> data3_copy{data3};

    bitonic_sort::BitonicSort(data3);
    std::sort(data3_copy.begin(), data3_copy.end());

    ASSERT_EQ(data3, data3_copy);

    /////////////////////////////////////////////////////////////////////
    std::vector<float> data4;
    data4.reserve(size4);

    for (int i = 0; i < size4; i++)
        data4.push_back(distr(gen));

    std::vector<float> data4_copy{data4};

    bitonic_sort::BitonicSort(data4);
    std::sort(data4_copy.begin(), data4_copy.end());

    ASSERT_EQ(data4, data4_copy);

    /////////////////////////////////////////////////////////////////////
    std::vector<float> data5;
    data5.reserve(size5);

    for (int i = 0; i < size5; i++)
        data5.push_back(distr(gen));

    std::vector<float> data5_copy{data5};

    bitonic_sort::BitonicSort(data5);
    std::sort(data5_copy.begin(), data5_copy.end());

    ASSERT_EQ(data5, data5_copy);

    /////////////////////////////////////////////////////////////////////
    std::vector<float> data6;
    data6.reserve(size6);

    for (int i = 0; i < size6; i++)
        data6.push_back(distr(gen));

    std::vector<float> data6_copy{data6};

    bitonic_sort::BitonicSort(data6);
    std::sort(data6_copy.begin(), data6_copy.end());

    ASSERT_EQ(data6, data6_copy);
}

TEST(BitonicSortTest, Test3) {
    const int size1 = 13;
    const int size2 = 24;
    const int size3 = 37;
    const int size4 = 99;
    const int size5 = 175;
    const int size6 = 431;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 100);

    /////////////////////////////////////////////////////////////////////
    std::vector<float> data1;
    data1.reserve(size1);

    for (int i = 0; i < size1; i++)
        data1.push_back(distr(gen));

    std::vector<float> data1_copy{data1};

    bitonic_sort::BitonicSort(data1);
    std::sort(data1_copy.begin(), data1_copy.end());

    ASSERT_EQ(data1, data1_copy);

    /////////////////////////////////////////////////////////////////////
    std::vector<float> data2;
    data2.reserve(size2);

    for (int i = 0; i < size2; i++)
        data2.push_back(distr(gen));

    std::vector<float> data2_copy{data2};

    bitonic_sort::BitonicSort(data2);
    std::sort(data2_copy.begin(), data2_copy.end());

    ASSERT_EQ(data2, data2_copy);

    /////////////////////////////////////////////////////////////////////
    std::vector<float> data3;
    data3.reserve(size3);

    for (int i = 0; i < size3; i++)
        data3.push_back(distr(gen));

    std::vector<float> data3_copy{data3};

    bitonic_sort::BitonicSort(data3);
    std::sort(data3_copy.begin(), data3_copy.end());

    ASSERT_EQ(data3, data3_copy);

    /////////////////////////////////////////////////////////////////////
    std::vector<float> data4;
    data4.reserve(size4);

    for (int i = 0; i < size4; i++)
        data4.push_back(distr(gen));

    std::vector<float> data4_copy{data4};

    bitonic_sort::BitonicSort(data4);
    std::sort(data4_copy.begin(), data4_copy.end());

    ASSERT_EQ(data4, data4_copy);

    /////////////////////////////////////////////////////////////////////
    std::vector<float> data5;
    data5.reserve(size5);

    for (int i = 0; i < size5; i++)
        data5.push_back(distr(gen));

    std::vector<float> data5_copy{data5};

    bitonic_sort::BitonicSort(data5);
    std::sort(data5_copy.begin(), data5_copy.end());

    ASSERT_EQ(data5, data5_copy);

    /////////////////////////////////////////////////////////////////////
    std::vector<float> data6;
    data6.reserve(size6);

    for (int i = 0; i < size6; i++)
        data6.push_back(distr(gen));

    std::vector<float> data6_copy{data6};

    bitonic_sort::BitonicSort(data6);
    std::sort(data6_copy.begin(), data6_copy.end());

    ASSERT_EQ(data6, data6_copy);
}