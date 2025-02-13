#pragma once

#include <CL/cl.h>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

namespace cl {

#define PARSE_ERR(msg_text, err)  \
    if (err != CL_SUCCESS) {  \
        std::string msg = "Error with " + std::string(msg_text) + ". Code: " + std::to_string(err); \
        throw std::runtime_error(msg); \
    } \


    namespace details {
        template <typename T, cl_int Name> struct param_traits {};
        template<> struct param_traits<cl_platform_info, CL_PLATFORM_NAME> {
            enum { value = CL_PLATFORM_NAME };
            using type = std::string;

            type CastToType(std::string &str) {
                return str;
            }
        };

        /*  Platform  */
        template<> struct param_traits<cl_platform_info, CL_PLATFORM_PROFILE> {
            enum { value = CL_PLATFORM_PROFILE };
            using type = std::string;

            type CastToType(std::string &str) {
                return str;
            }
        };
        template<> struct param_traits<cl_platform_info, CL_PLATFORM_VERSION> {
            enum { value = CL_PLATFORM_VERSION };
            using type = std::string;

            type CastToType(std::string &str) {
                return str;
            }
        };

        template<> struct param_traits<cl_platform_info, CL_PLATFORM_VENDOR> {
            enum { value = CL_PLATFORM_VENDOR };
            using type = std::string;

            type CastToType(std::string &str) {
                return str;
            }
        };

        /*  Device  */
        template<> struct param_traits<cl_device_info, CL_DEVICE_TYPE> {
            enum { value = CL_DEVICE_TYPE };
            using type = cl_device_type;

            type CastToType(const std::string &str) {
                return std::stoul(str);
            }
        };
        
        template<> struct param_traits<cl_device_info, CL_DEVICE_VENDOR_ID> {
            enum { value = CL_DEVICE_VENDOR_ID };
            using type = unsigned int;

            type CastToType(std::string &str) {
                return static_cast<unsigned int>(std::stoi(str));
            }
        };

        template<> struct param_traits<cl_device_info, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS> {
            enum { value = CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS };
            using type = unsigned int;

            type CastToType(std::string &str) {
                return static_cast<unsigned int>(std::stoi(str));
            }
        };

        template<> struct param_traits<cl_device_info, CL_DEVICE_MAX_WORK_ITEM_SIZES> {
            enum { value = CL_DEVICE_MAX_WORK_ITEM_SIZES };
            using type = std::vector<size_t>;

            type CastToType(std::string &str) {
                size_t size = str.size();
                type v; v.reserve(size / sizeof(size_t));
                for (size_t i = 0; i < size; i += sizeof(size_t)) {
                    size_t tmp = *reinterpret_cast<size_t*>(str.data());
                    v.push_back(tmp);
                }
                return v;
            }
        };

        template<> struct param_traits<cl_device_info, CL_DEVICE_MAX_WORK_GROUP_SIZE> {
            enum { value = CL_DEVICE_MAX_WORK_GROUP_SIZE };
            using type = size_t;

            type CastToType(std::string &str) {
                return std::stoull(str);
            }
        };
        
        /*  Context  */
        template<> struct param_traits<cl_context_info, CL_CONTEXT_REFERENCE_COUNT> {
            enum { value = CL_CONTEXT_REFERENCE_COUNT };
            using type = unsigned int;

            type CastToType(std::string &str) {
                return static_cast<unsigned int>(std::stoi(str));
            }
        };

        template<> struct param_traits<cl_context_info, CL_CONTEXT_NUM_DEVICES> {
            enum { value = CL_CONTEXT_NUM_DEVICES };
            using type = unsigned int;

            type CastToType(std::string &str) {
                return static_cast<unsigned int>(std::stoi(str));
            }
        };

        template<> struct param_traits<cl_context_info, CL_CONTEXT_DEVICES> {
            enum { value = CL_CONTEXT_NUM_DEVICES };
            using type = std::vector<cl_device_id>;

            type CastToType(std::string &str) {
                size_t size = str.size();
                type v; v.reserve(size / sizeof(cl_device_id));
                for (size_t i = 0; i < size; i += sizeof(cl_device_id)) {
                    cl_device_id tmp = *reinterpret_cast<cl_device_id*>(str.data());
                    v.push_back(tmp);
                }
                return v;
            }
        };

        template<> struct param_traits<cl_context_info, CL_CONTEXT_PROPERTIES> {
            enum { value = CL_CONTEXT_PROPERTIES };
            using type = std::vector<cl_context_properties>;

            type CastToType(std::string &str) {
                size_t size = str.size();
                type v; v.reserve(size / sizeof(cl_context_properties));
                for (size_t i = 0; i < size; i += sizeof(cl_context_properties)) {
                    cl_context_properties tmp = *reinterpret_cast<cl_context_properties*>(str.data());
                    v.push_back(tmp);
                }
                return v;
            }
        };
        
        /*  Program  */
        template<> struct param_traits<cl_program_info, CL_PROGRAM_REFERENCE_COUNT> {
            enum { value = CL_PROGRAM_REFERENCE_COUNT };
            using type = unsigned int;

            type CastToType(std::string &str) {
                return static_cast<unsigned int>(std::stoi(str));
            }
        };

        template<> struct param_traits<cl_program_info, CL_PROGRAM_CONTEXT> {
            enum { value = CL_PROGRAM_CONTEXT };
            using type = cl_context;

            type CastToType(std::string &str) {
                return *reinterpret_cast<cl_context*>(&str[0]);
            }
        };

        template<> struct param_traits<cl_program_info, CL_PROGRAM_NUM_DEVICES> {
            enum { value = CL_PROGRAM_NUM_DEVICES };
            using type = unsigned int;

            type CastToType(std::string &str) {
                return static_cast<unsigned int>(std::stoi(str));
            }
        };

        template<> struct param_traits<cl_program_info, CL_PROGRAM_DEVICES> {
            enum { value = CL_PROGRAM_DEVICES };
            using type = std::vector<cl_device_id>;

            type CastToType(std::string &str) {
                size_t size = str.size();
                type v; v.reserve(size / sizeof(cl_device_id));
                for (size_t i = 0; i < size; i += sizeof(cl_device_id)) {
                    cl_device_id tmp = *reinterpret_cast<cl_device_id*>(str.data());
                    v.push_back(tmp);
                }
                return v;
            }
        };

        template<> struct param_traits<cl_program_info, CL_PROGRAM_SOURCE> {
            enum { value = CL_PROGRAM_SOURCE };
            using type = std::string;

            type CastToType(std::string &str) {
                return str;
            }
        };
        
        template<> struct param_traits<cl_program_info, CL_PROGRAM_BINARY_SIZES> {
            enum { value = CL_PROGRAM_BINARY_SIZES };
            using type = std::vector<size_t>;

            type CastToType(std::string &str) {
                size_t size = str.size();
                type v; v.reserve(size / sizeof(size_t));
                for (size_t i = 0; i < size; i += sizeof(size_t)) {
                    size_t tmp = *reinterpret_cast<size_t*>(str.data());
                    v.push_back(tmp);
                }
                return v;
            }
        };

        // template<> struct param_traits<cl_program_info, CL_PROGRAM_BINARIES> {
        //     enum { value = CL_PROGRAM_BINARIES };
        //     using type = unsigned char**;

        //     type CastToType(std::string &str) {
        //         char* temp = new char[str.size() + 1U];
        //         std::strcpy(temp, str.c_str());
        //         return reinterpret_cast<type>(temp);
        //     }
        // };

        template<> struct param_traits<cl_program_info, CL_PROGRAM_NUM_KERNELS> {
            enum { value = CL_PROGRAM_NUM_KERNELS };
            using type = size_t;

            type CastToType(std::string &str) {
                return std::stoull(str);
            }
        };

        template<> struct param_traits<cl_program_info, CL_PROGRAM_KERNEL_NAMES> {
            enum { value = CL_PROGRAM_KERNEL_NAMES };
            using type = std::string;

            type CastToType(std::string &str) {
                return str;
            }
        };

        /*  Command Queue  */
        template<> struct param_traits<cl_command_queue_info, CL_QUEUE_CONTEXT> {
            enum { value = CL_QUEUE_CONTEXT };
            using type = cl_context;

            type CastToType(std::string &str) {
                return *reinterpret_cast<cl_context*>(&str[0]);
            }
        };

        template<> struct param_traits<cl_command_queue_info, CL_QUEUE_DEVICE> {
            enum { value = CL_QUEUE_DEVICE };
            using type = cl_device_id;

            type CastToType(std::string &str) {
                auto tmp = *reinterpret_cast<cl_device_id*>(&str[0]);
                return tmp;
            }
        };

        template<> struct param_traits<cl_command_queue_info, CL_QUEUE_REFERENCE_COUNT> {
            enum { value = CL_QUEUE_REFERENCE_COUNT };
            using type = unsigned int;

            type CastToType(std::string &str) {
                return static_cast<unsigned int>(std::stoi(str));
            }
        };

        // template<> struct param_traits<cl_command_queue_info, CL_QUEUE_SIZE> {
        //     enum { value = CL_QUEUE_SIZE };
        //     using type = unsigned int;

        //     type CastToType(std::string &str) {
        //         return static_cast<unsigned int>(std::stoi(str));
        //     }
        // };
    
        /*  Memory Object  */
        template<> struct param_traits<cl_mem_info, CL_MEM_TYPE> {
            enum { value = CL_MEM_TYPE };
            using type = cl_mem_object_type;

            type CastToType(std::string &str) {
                return static_cast<unsigned int>(std::stoi(str));
            }
        };

        template<> struct param_traits<cl_mem_info, CL_MEM_FLAGS> {
            enum { value = CL_MEM_FLAGS };
            using type = cl_mem_flags;

            type CastToType(std::string &str) {
                return std::stoul(str);
            }
        };

        template<> struct param_traits<cl_mem_info, CL_MEM_SIZE> {
            enum { value = CL_MEM_SIZE };
            using type = size_t;

            type CastToType(std::string &str) {
                return std::stoull(str);
            }
        };

        template<> struct param_traits<cl_mem_info, CL_MEM_HOST_PTR> {
            enum { value = CL_MEM_HOST_PTR };
            using type = void*;

            type CastToType(std::string &str) {
                return reinterpret_cast<void*>(&str[0]);
            }
        };

        template<> struct param_traits<cl_mem_info, CL_MEM_MAP_COUNT> {
            enum { value = CL_MEM_MAP_COUNT };
            using type = unsigned int;

            type CastToType(std::string &str) {
                return static_cast<unsigned int>(std::stoi(str));
            }
        };

        template<> struct param_traits<cl_mem_info, CL_MEM_REFERENCE_COUNT> {
            enum { value = CL_MEM_REFERENCE_COUNT };
            using type = unsigned int;

            type CastToType(std::string &str) {
                return static_cast<unsigned int>(std::stoi(str));
            }
        };

        template<> struct param_traits<cl_mem_info, CL_MEM_CONTEXT> {
            enum { value = CL_MEM_CONTEXT };
            using type = cl_context;

            type CastToType(std::string &str) {
                return *reinterpret_cast<cl_context*>(&str[0]);
            }
        };

        /*  Kernel  */
        template<> struct param_traits<cl_kernel_info, CL_KERNEL_FUNCTION_NAME> {
            enum { value = CL_KERNEL_FUNCTION_NAME };
            using type = std::string;

            type CastToType(std::string &str) {
                return str;
            }
        };

        template<> struct param_traits<cl_kernel_info, CL_KERNEL_NUM_ARGS> {
            enum { value = CL_KERNEL_NUM_ARGS };
            using type = unsigned int;

            type CastToType(std::string &str) {
                return static_cast<unsigned int>(std::stoi(str));
            }
        };

        template<> struct param_traits<cl_kernel_info, CL_KERNEL_REFERENCE_COUNT> {
            enum { value = CL_KERNEL_REFERENCE_COUNT };
            using type = unsigned int;

            type CastToType(std::string &str) {
                return static_cast<unsigned int>(std::stoi(str));
            }
        };

        template<> struct param_traits<cl_kernel_info, CL_KERNEL_CONTEXT> {
            enum { value = CL_KERNEL_CONTEXT };
            using type = cl_context;

            type CastToType(std::string &str) {
                return *reinterpret_cast<cl_context*>(&str[0]);
            }
        };

        template<> struct param_traits<cl_kernel_info, CL_KERNEL_PROGRAM> {
            enum { value = CL_KERNEL_PROGRAM };
            using type = cl_program;

            type CastToType(std::string &str) {
                return *reinterpret_cast<cl_program*>(&str[0]);
            }
        };

        /*  ReferenceHandler  */
        template <typename T> struct ReferenceHandler {};

        template <> struct ReferenceHandler<cl_platform_id> {
            static cl_int Retain(cl_platform_id platform) {
                (void) platform;
                return 0;
            }
            
            static cl_int Release(cl_platform_id platform) { 
                (void) platform;
                return 0;
            }
        };

        template <> struct ReferenceHandler<cl_device_id> {
            static cl_int Retain(cl_device_id device) { 
                std::cout << "retain device" << std::endl;
                return ::clRetainDevice(device);
            }

            static cl_int Release(cl_device_id device) { 
                std::cout << "release device" << std::endl;
                return ::clReleaseDevice(device);
            }
        };

        template <> struct ReferenceHandler<cl_context> {
            static cl_int Retain(cl_context context) { 
                std::cout << "retain context" << std::endl;
                return ::clRetainContext(context); 
            }
            static cl_int Release(cl_context context) { 
                std::cout << "release context" << std::endl;
                return ::clReleaseContext(context); 
            }
        };
        
        template <> struct ReferenceHandler<cl_program> {
            static cl_int Retain(cl_program program) { 
                std::cout << "retain program" << std::endl;                
                return ::clRetainProgram(program); 
            }
            static cl_int Release(cl_program program) {
                std::cout << "release program" << std::endl;
                return ::clReleaseProgram(program); 
            }
        };

        template <> struct ReferenceHandler<cl_command_queue> {
            static cl_int Retain(cl_command_queue command_queue) { 
                std::cout << "retain com" << std::endl;
                return ::clRetainCommandQueue(command_queue); 
            }
            static cl_int Release(cl_command_queue command_queue) { 
                std::cout << "release com" << std::endl;
                return ::clReleaseCommandQueue(command_queue);
            }
        };

        template <> struct ReferenceHandler<cl_mem> {
            static cl_int Retain(cl_mem memory) { 
                std::cout << "retain mem" << std::endl;
                return ::clRetainMemObject(memory); 
            }
            static cl_int Release(cl_mem memory) { 
                std::cout << "release mem" << std::endl;
                return ::clReleaseMemObject(memory); 
            }
        };

        template <> struct ReferenceHandler<cl_kernel> {
            static cl_int Retain(cl_kernel kernal) { 
                std::cout << "retain kernel" << std::endl;
                return ::clRetainKernel(kernal); 
            }
            static cl_int Release(cl_kernel kernal) { 
                std::cout << "release kernel" << std::endl;
                return ::clReleaseKernel(kernal); 
            }
        };

        /*  Info Manager  */
        template <typename T> struct InfoManager {};

        template <> struct InfoManager<cl_platform_id> {
            template <cl_platform_info param_name>
            static typename details::param_traits<cl_platform_info, param_name>::type
            GetInfo(cl_platform_id platform) {
                size_t info_size = 0;
                clGetPlatformInfo(platform, param_name, 0, NULL, &info_size);
                std::string value{info_size, '\0'};
                clGetPlatformInfo(platform, param_name, info_size, value.c_str(), NULL);
                
                return details::param_traits<cl_platform_info, param_name>::CastToType(value);
            }
        };

        template <> struct InfoManager<cl_device_id> {
            template <cl_device_info param_name>
            static typename details::param_traits<cl_device_info, param_name>::type
            GetInfo(cl_device_id device) {
                size_t info_size = 0;
                clGetDeviceInfo(device, param_name, 0, NULL, &info_size);
                std::string value{info_size, '\0'};
                clGetDeviceInfo(device, param_name, info_size, value.c_str(), NULL);
                
                return details::param_traits<cl_device_info, param_name>::CastToType(value);
            }
        };

        template <> struct InfoManager<cl_context> {
            template <cl_context_info param_name>
            static typename details::param_traits<cl_context_info, param_name>::type
            GetInfo(cl_context context) {
                size_t info_size = 0;
                clGetContextInfo(context, param_name, 0, NULL, &info_size);
                std::string value{info_size, '\0'};
                clGetContextInfo(context, param_name, info_size, value.c_str(), NULL);
                
                return details::param_traits<cl_context_info, param_name>::CastToType(value);
            }
        };
        
        template <> struct InfoManager<cl_program> {
            template <cl_program_info param_name>
            static typename details::param_traits<cl_program_info, param_name>::type
            GetInfo(cl_program program) {
                size_t info_size = 0;
                clGetProgramInfo(program, param_name, 0, NULL, &info_size);
                std::string value{info_size, '\0'};
                clGetProgramInfo(program, param_name, info_size, value.c_str(), NULL);
                
                return details::param_traits<cl_program_info, param_name>::CastToType(value);
            }
        };

        template <> struct InfoManager<cl_command_queue> {
            template <cl_command_queue_info param_name>
            static typename details::param_traits<cl_command_queue_info, param_name>::type
            GetInfo(cl_command_queue command_queue) {
                size_t info_size = 0;
                clGetCommandQueueInfo(command_queue, param_name, 0, NULL, &info_size);
                std::string value{info_size, '\0'};
                clGetCommandQueueInfo(command_queue, param_name, info_size, value.c_str(), NULL);
                
                return details::param_traits<cl_command_queue_info, param_name>::CastToType(value);
            }
        };

        template <> struct InfoManager<cl_mem> {
            template <cl_mem_info param_name>
            static typename details::param_traits<cl_mem_info, param_name>::type
            GetInfo(cl_mem memory) {
                size_t info_size = 0;
                clGetMemObjectInfo(memory, param_name, 0, NULL, &info_size);
                std::string value{info_size, '\0'};
                clGetMemObjectInfo(memory, param_name, info_size, value.c_str(), NULL);
                
                return details::param_traits<cl_mem_info, param_name>::CastToType(value);
            }
        };

        template <> struct InfoManager<cl_kernel> {
            template <cl_kernel_info param_name>
            static typename details::param_traits<cl_kernel_info, param_name>::type
            GetInfo(cl_kernel kernel) {
                size_t info_size = 0;
                clGetKernelInfo(kernel, param_name, 0, NULL, &info_size);
                std::string value{info_size, '\0'};
                clGetKernelInfo(kernel, param_name, info_size, value.c_str(), NULL);
                
                return details::param_traits<cl_kernel_info, param_name>::CastToType(value);
            }
        };

        /*  Wrapper  */
        template <typename cl_type> class Wrapper {
        public:
            Wrapper(cl_type obj = NULL) : obj_(obj) { }
            
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

            void GetInfo() const {
                InfoManager<cl_type>::GetInfo(obj_);
            }
        protected:
            cl_type obj_;
        }; // class Wrapper
    } // namespace details


    class Platform final : public details::Wrapper<cl_platform_id> {
    public:
        Platform() {
            cl_int err = 0;
            err |= clGetPlatformIDs(1, &obj_, NULL);
            PARSE_ERR("finding platform", err)
        }
    }; // class Platform

    class Device final : public details::Wrapper<cl_device_id> {
    public:
        Device() : platform_() {
            cl_int err = 0;
            err |= clGetDeviceIDs(platform_.Get(), CL_DEVICE_TYPE_DEFAULT, 1, &obj_, NULL);
            PARSE_ERR("finding device", err);
        }

        Device(const Platform &platform) : platform_(platform) {
            cl_int err = 0;
            err |= clGetDeviceIDs(platform_.Get(), CL_DEVICE_TYPE_GPU, 1, &obj_, NULL);
            if (err == CL_DEVICE_NOT_FOUND)
                err = clGetDeviceIDs(platform_.Get(), CL_DEVICE_TYPE_CPU, 1, &obj_, NULL);

            PARSE_ERR("finding device", err)
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

    private:
        Platform platform_;
    }; // class Device
    
    class Context final : public details::Wrapper<cl_context> {
    public:
        Context() : details::Wrapper<cl_context>(NULL), device_() {}
        
        Context(const Device &device) : device_(device) {
            cl_int err = 0;
            obj_ = clCreateContext(NULL, 1, &device(), NULL, NULL, &err);
            PARSE_ERR("creating context", err)
        }

        Context(const Context &other) : details::Wrapper<cl_context>(std::move(other)), device_(other.device_) {}

        Context &operator=(const Context &other) {
            details::Wrapper<cl_context>::operator=(std::move(other));
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
            err |= clBuildProgram(obj_, 0, NULL, NULL, NULL, NULL);

            PARSE_ERR("compiling program", err);
        }

        Program(const Context &context, const char **program_text) : context_(context) {
            cl_int err = 0;
            obj_ = clCreateProgramWithSource(context_.Get(), 1, program_text, NULL, &err);
            PARSE_ERR("compiling program", err);

            err |= clBuildProgram(obj_, 0, NULL, NULL, NULL, NULL);
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

        Context GetContext() const {
            return context_;
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
            PARSE_ERR("creating buffer", err)
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
        void Copy(IterT start_it) {
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

    private:
        size_t size_ = 0;
        CommandQueue queue_;
    }; // class Buffer

    class Kernel final : public details::Wrapper<cl_kernel> {
    public:
        Kernel() : details::Wrapper<cl_kernel>(NULL), program_(), queue_() {}

        Kernel(const Program &program, const CommandQueue &queue, std::string &func_name) :
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
            ::clRetainMemObject(mem);
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

        void Run(size_t data_size) {
            cl_int err = 0;
            err |= clEnqueueNDRangeKernel(queue_.Get(), obj_, 1, NULL, &data_size, NULL, 0, NULL, NULL);
            PARSE_ERR("invoke kernel", err)
            err |= clFinish(queue_.Get());
            PARSE_ERR("finish kernel", err)
        }

    private:
        Program program_;
        CommandQueue queue_;
    }; // class Kernel
} // namespace cl