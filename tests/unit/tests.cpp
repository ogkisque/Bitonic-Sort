#include "cl.hpp"
#include <gtest/gtest.h>

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
    std::cerr << "1" << std::endl;
    cl::Context context(device);

    std::cerr << "2" << std::endl;
    cl::CommandQueue queue(context);
    std::cerr << "2.5" << std::endl;
    ASSERT_EQ(queue.template GetInfo<CL_QUEUE_CONTEXT>(), context.Get());
    std::cerr << "2.6" << std::endl;
    ASSERT_EQ(queue.template GetInfo<CL_QUEUE_DEVICE>(), device.Get());
    std::cerr << "2.7" << std::endl;
    ASSERT_EQ(queue.template GetInfo<CL_QUEUE_REFERENCE_COUNT>(), 1U);
    std::cerr << "3" << std::endl;
    cl::CommandQueue queue1(queue);
    cl::CommandQueue queue2(queue1);
    std::cerr << "4" << std::endl;
    ASSERT_EQ(queue.Get(), queue1.Get());
    ASSERT_EQ(queue1.Get(), queue2.Get());
    ASSERT_EQ(queue2.template GetInfo<CL_QUEUE_REFERENCE_COUNT>(), 3U);

    std::cerr << "5" << std::endl;
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
    ASSERT_EQ(program.template GetInfo<CL_PROGRAM_NUM_KERNELS>(), 0U);

    ASSERT_EQ(program.template GetInfo<CL_PROGRAM_SOURCE>(), program_src);

    cl::Program program1 = program;
    cl::Program program2 = program1;

    ASSERT_EQ(program.Get(), program1.Get());
    ASSERT_EQ(program1.Get(), program2.Get());
    ASSERT_EQ(program2.template GetInfo<CL_QUEUE_REFERENCE_COUNT>(), 3U);

    cl::Program program3(std::move(program2));
    ASSERT_EQ(program.Get(), program3.Get());
    ASSERT_EQ(program3.template GetInfo<CL_QUEUE_REFERENCE_COUNT>(), 3U);    
}