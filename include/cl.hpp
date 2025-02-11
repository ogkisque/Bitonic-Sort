#pragma once

#include <CL/cl.h>
#include <stdexcept>
#include <string>
#include <fstream>

namespace cl {

#define PARSE_ERR(msg_text, err)  \
        if (err != CL_SUCCESS) {  \
            std::string msg = "Error with " + std::string(msg_text) + ". Code: " + std::to_string(err); \
            throw std::runtime_error(msg); \
        } \

namespace details {
    

} // namespace details

    class Platform final {
    public:
        Platform() {
            cl_int err = 0;
            err |= clGetPlatformIDs(1, &id_, NULL);
            PARSE_ERR("finding platform", err)
        }

        cl_platform_id GetId() const {
            return id_;
        }

    private:
        cl_platform_id id_;

    }; // class Platform

    class Device final {
    public:
        Device(Platform platform) : platform_(platform) {
            cl_int err = 0;
            err |= clGetDeviceIDs(platform.GetId(), CL_DEVICE_TYPE_GPU, 1, &id_, NULL);
            if (err == CL_DEVICE_NOT_FOUND)
                err = clGetDeviceIDs(platform.GetId(), CL_DEVICE_TYPE_CPU, 1, &id_, NULL);

            PARSE_ERR("finding device", err)
        }

        cl_device_id &GetId() {
            return id_;
        }

    private:
        Platform platform_;
        cl_device_id id_;
    }; // class Device

    class Context final {
    public:
        Context(Device device) : device_(device) {
            cl_int err = 0;
            context_ = clCreateContext(NULL, 1, &device.GetId(), NULL, NULL, &err);
            PARSE_ERR("creating context", err)
        }

        cl_context GetContext() const {
            return context_;
        }

        Device GetDevice() const {
            return device_;
        }

    private:
        Device device_;
        cl_context context_;

    }; // class Context

    class Program final {
    public:
        Program(Context context, std::string &file_name) : context_(context) {
            cl_int err = 0;
            std::string program_text = ReadFileToString(file_name);
            const char *text = program_text.data();
            size_t size = program_text.size();

            program_ = clCreateProgramWithSource(context_.GetContext(), 1, &text, &size, &err);
            err |= clBuildProgram(program_, 0, NULL, NULL, NULL, NULL);

            PARSE_ERR("compiling program", err);
        }

        Program(Context context, const char **program_text) : context_(context) {
            cl_int err = 0;

            program_ = clCreateProgramWithSource(context_.GetContext(), 1, program_text, NULL, &err);
            err |= clBuildProgram(program_, 0, NULL, NULL, NULL, NULL);

            PARSE_ERR("compiling program", err);
        }

        cl_program GetProgram() const
        {
            return program_;
        }

    private:
        std::string ReadFileToString(std::string &file_name)
        {
            std::ifstream stream(file_name);
            return std::string(std::istreambuf_iterator<char>(stream), 
                               std::istreambuf_iterator<char>());
        }

        Context context_;
        cl_program program_;
    }; // class Program

    class Kernel {
    public:
        Kernel(Program program, std::string &func_name) : program_(program) {
            cl_int err = 0;
            kernel_ = clCreateKernel(program_.GetProgram(), func_name.data(), &err);
            PARSE_ERR("creating kernel", err)
        }

    private:
        Program program_;
        cl_kernel kernel_;

    }; // class Kernel

    class CommandQueue {
    public:
        CommandQueue(Context context) : context_(context) {
            cl_int err = 0;
            queue_ = clCreateCommandQueue(context_.GetContext(), context_.GetDevice().GetId(), 0, &err);
            PARSE_ERR("creating queue", err)
        }

        cl_command_queue GetQueue() const {
            return queue_;
        }

        Context GetContext() const {
            return context_;
        }

    private:
        Context context_;
        cl_command_queue queue_;
    };

    class Buffer {
    public:
        Buffer(CommandQueue queue, cl_mem_flags flag, size_t size) : queue_(queue) {
            cl_int err = 0;
            buf_ = clCreateBuffer(queue_.GetContext().GetContext(), flag, size, NULL, &err);
            PARSE_ERR("creating buffer", err)
        }

        template <typename IterT>
        Buffer(CommandQueue queue, IterT start_it, IterT end_it, cl_mem_flags flag = CL_MEM_READ_ONLY) :
               queue_(queue) {
            cl_int err = 0;
            size_t size = std::distance(start_it, end_it) * sizeof(*start_it);
            buf_ = clCreateBuffer(queue_.GetContext().GetContext(), flag, size, NULL, &err);
            PARSE_ERR("creating buffer", err)

            void *data = malloc(size);
            std::copy(start_it, end_it, data);
            err = clEnqueueWriteBuffer(queue_.GetQueue(), buf_, CL_TRUE, 0, size, data, 0, NULL, NULL);
            PARSE_ERR("copy to buffer", err)
        }

    private:
        CommandQueue queue_;
        cl_mem buf_;
    }; // class Buffer

} // namespace cl