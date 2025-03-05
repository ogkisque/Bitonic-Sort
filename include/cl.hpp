#pragma once

#include <CL/cl.h>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "support_structs.hpp"

namespace cl {
    namespace details {

        /*  Wrapper  */
        template <typename cl_type> class Wrapper {
        public:
            Wrapper(cl_type obj = NULL) : obj_(obj) { Retain(); }
            
            Wrapper(const Wrapper &other) : obj_(other.obj_) {
                Retain();
            }

            Wrapper &operator=(const Wrapper &other) {
                if (this != &other) {
                    Release();
                    obj_ = other.obj_;
                    Retain();
                }
                
                return *this;
            }

            Wrapper(Wrapper &&other) noexcept : obj_(other.obj_) {
                other.obj_ = NULL;
            }

            Wrapper &operator=(Wrapper &&other) noexcept {
                if (this != &other) {
                    Release();
                    obj_ = other.obj_;
                    other.obj_ = NULL;
                }
                
                return *this;
            }
            
            ~Wrapper() { if (obj_) Release(); }
            
            const cl_type& operator()() const { 
                return obj_;
            }

            cl_type& operator()() {
                return obj_;
            }
            
            cl_type Get() const {
                return obj_;
            }

            cl_int Retain() const {
                return ReferenceHandler<cl_type>::Retain(obj_);
            }

            cl_int Release() const {
                return ReferenceHandler<cl_type>::Release(obj_);
            }
        protected:
            cl_type obj_;
        }; // class Wrapper
    } // namespace details

    class Device;
    
    class Platform final : public details::Wrapper<cl_platform_id> {
    public:
        Platform() {
            cl_int err = 0;
            err |= clGetPlatformIDs(1, &obj_, NULL);
            PARSE_ERR("finding platform", err);
        }

        explicit Platform(cl_platform_id id) : details::Wrapper<cl_platform_id>(id) {}

        static void GetPlatforms(std::vector<Platform> &platforms) {
            cl_uint num_platforms;
            cl_int err = clGetPlatformIDs(0, NULL, &num_platforms);
            PARSE_ERR("finding platforms", err);

            if (num_platforms == 0) {
                throw std::runtime_error("platforms were not found");
            }

            std::vector<cl_platform_id> platform_ids(num_platforms);
            err = clGetPlatformIDs(num_platforms, platform_ids.data(), NULL);
            PARSE_ERR("getting platform IDs", err);

            platforms.clear();
            platforms.reserve(num_platforms);
            for (auto id : platform_ids) {
                platforms.emplace_back(id);
            }
        }

        void GetDevices(cl_device_type device_type, std::vector<Device> &devices) const;

        template <cl_platform_info param_name>
        typename details::param_traits<cl_platform_info, param_name>::type 
        GetInfo() const {
            return details::InfoManager<cl_platform_id>::template GetInfo<param_name>(obj_);
        }
    }; // class Platform

    class Device final : public details::Wrapper<cl_device_id> {
    public:
        Device() : platform_() {
            cl_int err = clGetDeviceIDs(platform_.Get(), CL_DEVICE_TYPE_DEFAULT, 1, &obj_, NULL);
            PARSE_ERR("finding device", err);
        }

        Device(cl_device_id id, const Platform& platform) : details::Wrapper<cl_device_id>(id), platform_(platform) {}

        Device(const Platform &platform) : platform_(platform) {
            cl_int err = clGetDeviceIDs(platform_.Get(), CL_DEVICE_TYPE_GPU, 1, &obj_, NULL);
            if (err == CL_DEVICE_NOT_FOUND)
                err = clGetDeviceIDs(platform_.Get(), CL_DEVICE_TYPE_CPU, 1, &obj_, NULL);

            PARSE_ERR("finding device", err);
        }

        Device(const Device &other) : details::Wrapper<cl_device_id>(other), platform_(other.platform_) {}

        Device &operator=(const Device &other) {
            details::Wrapper<cl_device_id>::operator=(other);
            platform_ = other.platform_;
            return *this;
        }

        Device(Device &&other) noexcept : details::Wrapper<cl_device_id>(std::move(other)) {
            std::swap(platform_, other.platform_);
        }

        Device &operator=(Device &&other) noexcept {
            if (this != &other) {
                details::Wrapper<cl_device_id>::operator=(std::move(other));
                std::swap(platform_, other.platform_);
            }

            return *this;
        }
        
        template <cl_device_info param_name>
        typename details::param_traits<cl_device_info, param_name>::type 
        GetInfo() {
            return details::InfoManager<cl_device_id>::template GetInfo<param_name>(obj_);
        }

    private:
        Platform platform_;
    }; // class Device

    void Platform::GetDevices(cl_device_type device_type, std::vector<Device> &devices) const {
        cl_uint num_devices;

        cl_int err = clGetDeviceIDs(obj_, device_type, 0, nullptr, &num_devices);
        
        if (num_devices == 0) {
            throw std::runtime_error("devices were not found in this platform");
        }

        std::vector<cl_device_id> device_ids(num_devices);
        err = clGetDeviceIDs(obj_, device_type, num_devices, device_ids.data(), nullptr);
        PARSE_ERR("getting device IDs", err);

        devices.clear();
        devices.reserve(num_devices);
        for (auto id : device_ids) {
            devices.emplace_back(id, *this);
        }
    }
    
    class Context final : public details::Wrapper<cl_context> {
    public:
        Context() : details::Wrapper<cl_context>(NULL), device_() {}
        
        Context(const Device &device) : device_(device) {
            cl_int err = 0;
            obj_ = clCreateContext(NULL, 1, &device_(), NULL, NULL, &err);
            PARSE_ERR("creating context", err);
        }

        Context(const Context &other) : details::Wrapper<cl_context>(other), device_(other.device_) {}

        Context &operator=(const Context &other) {
            details::Wrapper<cl_context>::operator=(other);
            device_ = other.device_;
            return *this;
        }

        Context(Context &&other) noexcept : details::Wrapper<cl_context>(std::move(other)) {
            std::swap(device_, other.device_);
        }

        Context &operator=(Context &&other) noexcept {
            if (this != &other) {
                details::Wrapper<cl_context>::operator=(std::move(other));
                std::swap(device_, other.device_);
            }

            return *this;
        }

        Device GetDevice() const {
            return device_;
        }

        template <cl_context_info param_name>
        typename details::param_traits<cl_context_info, param_name>::type 
        GetInfo() {
            return details::InfoManager<cl_context>::template GetInfo<param_name>(obj_);
        }

    private:
        Device device_;
    }; // class Context

    class Program final : public details::Wrapper<cl_program> {
    public:
        Program() : details::Wrapper<cl_program>(NULL), context_() {}
        
        Program(const Context &context, std::string &file_name) : context_(context) {
            cl_int err = 0;
            std::string program_text = ReadFileToString(file_name);
            const char *text = program_text.data();
            size_t size = program_text.size();

            obj_ = clCreateProgramWithSource(context_.Get(), 1, &text, &size, &err);
            err |= clBuildProgram(obj_, 1, &context_.GetDevice()(), NULL, NULL, NULL);

            PARSE_ERR("compiling program", err);
        }

        Program(const Context &context, const char **program_text) : context_(context) {
            cl_int err = 0;
            obj_ = clCreateProgramWithSource(context_.Get(), 1, program_text, NULL, &err);
            PARSE_ERR("compiling program", err);
            
            err |= clBuildProgram(obj_, 1, &context_.GetDevice()(), NULL, NULL, NULL);
            PARSE_ERR("compiling program", err);
        }
        
        Program(const Program &other) : details::Wrapper<cl_program>(other), context_(other.context_) {}

        Program& operator=(const Program &other) {
            details::Wrapper<cl_program>::operator=(other);
            context_ = other.context_;
            
            return *this;
        }

        Program(Program &&other) noexcept : details::Wrapper<cl_program>(std::move(other)) {
            std::swap(context_, other.context_);
        }

        Program &operator=(Program &&other) noexcept {
            if (this != &other) {
                details::Wrapper<cl_program>::operator=(std::move(other));
                std::swap(context_, other.context_);
            }

            return *this;
        }

        Context GetContext() const {
            return context_;
        }

        template <cl_program_info param_name>
        typename details::param_traits<cl_program_info, param_name>::type 
        GetInfo() {
            return details::InfoManager<cl_program>::template GetInfo<param_name>(obj_);
        }

    private:
        std::string ReadFileToString(std::string &file_name) {
            std::ifstream stream(file_name);
            return std::string(std::istreambuf_iterator<char>(stream), 
                                std::istreambuf_iterator<char>());
        }

        Context context_;
    }; // class Program

    class CommandQueue final : public details::Wrapper<cl_command_queue> {
    public:
        CommandQueue() : details::Wrapper<cl_command_queue>(NULL), context_() {}

        CommandQueue(const Context &context) : context_(context) {
            cl_int err = 0;
            obj_ = clCreateCommandQueue(context_.Get(), context_.GetDevice().Get(), 0, &err);
            PARSE_ERR("creating queue", err)
        }
        
        CommandQueue(const CommandQueue &other) : details::Wrapper<cl_command_queue>(other), context_(other.context_) {}

        CommandQueue &operator=(const CommandQueue &other) {
            details::Wrapper<cl_command_queue>::operator=(other);
            context_ = other.context_;

            return *this;
        }

        CommandQueue(CommandQueue &&other) noexcept : details::Wrapper<cl_command_queue>(std::move(other)) {
            std::swap(context_, other.context_);
        }

        CommandQueue &operator=(CommandQueue &&other) noexcept {
            if (this != &other) {
                details::Wrapper<cl_command_queue>::operator=(std::move(other));
                std::swap(context_, other.context_);
            }

            return *this;
        }

        void PutBarrier() {
            cl_int err = clEnqueueBarrier(obj_);
            PARSE_ERR("putting barrier", err)
        }

        void Finish() {
            cl_int err = clFinish(obj_);
            PARSE_ERR("finish tasks", err)
        }

        Context GetContext() const {
            return context_;
        }

        template <cl_command_queue_info param_name>
        typename details::param_traits<cl_command_queue_info, param_name>::type 
        GetInfo() {
            return details::InfoManager<cl_command_queue>::template GetInfo<param_name>(obj_);
        }

    private:
        Context context_;
    };

    class Buffer final : public details::Wrapper<cl_mem> {
    public:
        Buffer() : details::Wrapper<cl_mem>(NULL), queue_() {}

        Buffer(const CommandQueue &queue, cl_mem_flags flag, size_t size) : size_(size), queue_(queue) {
            cl_int err = 0;
            obj_ = clCreateBuffer(queue_.GetContext().Get(), flag, size, NULL, &err);
            PARSE_ERR("creating buffer", err);
        }

        Buffer(const Buffer &other) : details::Wrapper<cl_mem>(other), size_(other.size_), queue_(other.queue_) {}

        Buffer &operator=(const Buffer &other) {
            details::Wrapper<cl_mem>::operator=(other);
            size_ = other.size_;
            queue_ = other.queue_;
            
            return *this;
        }

        Buffer(Buffer &&other) noexcept : details::Wrapper<cl_mem>(std::move(other)) {
            std::swap(size_, other.size_);
            std::swap(queue_, other.queue_);
        }

        Buffer &operator=(Buffer &&other) noexcept {
            if (this != &other) {
                details::Wrapper<cl_mem>::operator=(std::move(other));
                std::swap(size_, other.size_);
                std::swap(queue_, other.queue_);
            }

            return *this;
        }

        template <typename IterT>
        Buffer(CommandQueue &queue, IterT start_it, IterT end_it, cl_mem_flags flag) :
                queue_(queue) {
            using T = typename std::iterator_traits<IterT>::value_type;
            cl_int err = 0;
            size_ = std::distance(start_it, end_it) * sizeof(T);
            obj_ = clCreateBuffer(queue_.GetContext().Get(), flag, size_, NULL, &err);
            PARSE_ERR("creating buffer", err)

            T *data = (T*) malloc(size_);
            std::copy(start_it, end_it, data);

            err = clEnqueueWriteBuffer(queue_.Get(), obj_, CL_TRUE, 0, size_, data, 0, NULL, NULL);
            free(data);
            PARSE_ERR("copy to buffer", err)
        }

        size_t GetSize() const {
            return size_;
        }

        template <typename IterT>
        void Read(IterT start_it) {
            using T = typename std::iterator_traits<IterT>::value_type;
            cl_int err = 0;
            T *res = (T*) malloc(size_);
            err |= clEnqueueReadBuffer(queue_.Get(), obj_, CL_TRUE, 0, size_, res, 0, NULL, NULL);
            PARSE_ERR("copy from cl buffer", err);

            size_t num_elems = size_ / sizeof(T);

            IterT it = start_it;
            for (size_t i = 0; i < num_elems; i++, it++)
                *it = res[i];

            free(res);
        }

        template <typename IterT>
        void Write(IterT start_it, IterT end_it) {
            using T = typename std::iterator_traits<IterT>::value_type;
            cl_int err = 0;
            T *data = (T*) malloc(size_);
            std::copy(start_it, end_it, data);
            err = clEnqueueWriteBuffer(queue_.Get(), obj_, CL_TRUE, 0, size_, data, 0, NULL, NULL);

            free(data);
            PARSE_ERR("copy to buffer", err)
        }

        template <cl_mem_info param_name>
        typename details::param_traits<cl_mem_info, param_name>::type 
        GetInfo() {
            return details::InfoManager<cl_mem>::template GetInfo<param_name>(obj_);
        }

    private:
        size_t size_ = 0;
        CommandQueue queue_;
    }; // class Buffer

    class Kernel final : public details::Wrapper<cl_kernel> {
    public:
        Kernel() : details::Wrapper<cl_kernel>(NULL), program_(), queue_() {}

        Kernel(const Program &program, const CommandQueue &queue, const std::string &func_name) :
                program_(program), queue_(queue) {
            cl_int err = 0;
            obj_ = clCreateKernel(program_.Get(), func_name.data(), &err);
            PARSE_ERR("creating kernel", err)
        }

        Kernel(const Kernel &other) : details::Wrapper<cl_kernel>(other), program_(other.program_), queue_(other.queue_) {}

        Kernel &operator=(const Kernel &other) {
            queue_ = other.queue_;
            program_ = other.program_;
            details::Wrapper<cl_kernel>::operator=(other);

            return *this;
        }

        Kernel(Kernel &&other) noexcept : details::Wrapper<cl_kernel>(std::move(other)) {
            std::swap(program_, other.program_);
            std::swap(queue_, other.queue_);
        }

        Kernel &operator=(Kernel &&other) noexcept {
            if (this != &other) {
                details::Wrapper<cl_kernel>::operator=(std::move(other));
                std::swap(program_, other.program_);
                std::swap(queue_, other.queue_);
            }

            return *this;
        }

        template <typename T>
        void SetArg(cl_uint arg_num, const T &arg_value) {
            cl_int err = clSetKernelArg(obj_, arg_num, sizeof(arg_value), &arg_value);
            PARSE_ERR("setting arg in kernel", err)
        }

        void SetArg(cl_uint arg_num, const Buffer &buf) {
            cl_mem mem = buf.Get();
            buf.Retain();
            cl_int err = clSetKernelArg(obj_, arg_num, sizeof(cl_mem), &mem);
            PARSE_ERR("setting arg in kernel", err)
        }

        template <typename IterT>
        void SetArgs(IterT start_it, IterT end_it, cl_uint first_arg_num) {
            cl_int err = 0;
            size_t size = sizeof(IterT*);
            cl_uint num = first_arg_num;
            for (IterT it = start_it; it != end_it; it++, num++) {
                cl_int err = clSetKernelArg(obj_, num, size, it);
                PARSE_ERR("setting arg in kernel", err)
            }
        }

        void Run(size_t global_work_size, size_t local_work_size) {
            cl_int err = 0;
            err |= clEnqueueNDRangeKernel(queue_.Get(), obj_, 1, NULL, &global_work_size, &local_work_size, 0, NULL, NULL);
            PARSE_ERR("invoke kernel", err)
            err |= clFinish(queue_.Get());
            PARSE_ERR("finish kernel", err)
        }

        template <cl_kernel_info param_name>
        typename details::param_traits<cl_kernel_info, param_name>::type 
        GetInfo() {
            return details::InfoManager<cl_kernel>::template GetInfo<param_name>(obj_);
        }

    private:
        Program program_;
        CommandQueue queue_;
    }; // class Kernel
} // namespace cl