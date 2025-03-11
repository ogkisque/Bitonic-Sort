#pragma once

#include <CL/cl.h>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "CL/cl.hpp"

namespace cl {

    namespace details {
        template <typename T, cl_int Name> struct param_traits {};

        /*  Platform  */
        template<> struct param_traits<cl_platform_info, CL_PLATFORM_NAME> {
            enum { value = CL_PLATFORM_NAME };
            using type = std::string;

            static type CastToType(std::string &str) {
                return str;
            }
        };
        
        template<> struct param_traits<cl_platform_info, CL_PLATFORM_PROFILE> {
            enum { value = CL_PLATFORM_PROFILE };
            using type = std::string;

            static type CastToType(std::string &str) {
                return str;
            }
        };

        template<> struct param_traits<cl_platform_info, CL_PLATFORM_VERSION> {
            enum { value = CL_PLATFORM_VERSION };
            using type = std::string;

            static type CastToType(std::string &str) {
                return str;
            }
        };

        template<> struct param_traits<cl_platform_info, CL_PLATFORM_VENDOR> {
            enum { value = CL_PLATFORM_VENDOR };
            using type = std::string;

            static type CastToType(std::string &str) {
                return str;
            }
        };

        /*  Device  */
        template<> struct param_traits<cl_device_info, CL_DEVICE_TYPE> {
            enum { value = CL_DEVICE_TYPE };
            using type = cl_device_type;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type*>(str.data());
            }
        };
        
        template<> struct param_traits<cl_device_info, CL_DEVICE_VENDOR_ID> {
            enum { value = CL_DEVICE_VENDOR_ID };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<const unsigned int*>(str.data());
            }
        };

        template<> struct param_traits<cl_device_info, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS> {
            enum { value = CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type*>(str.data());
            }
        };

        template<> struct param_traits<cl_device_info, CL_DEVICE_MAX_WORK_ITEM_SIZES> {
            enum { value = CL_DEVICE_MAX_WORK_ITEM_SIZES };
            using type = std::vector<size_t>;

            static type CastToType(std::string &str) {
                size_t size = str.size();
                type v; v.reserve(size / sizeof(size_t));
                for (size_t i = 0; i < size; i += sizeof(size_t)) {
                    size_t tmp = *reinterpret_cast<size_t*>(&str[i]);
                    v.push_back(tmp);
                }
                return v;
            }
        };

        template<> struct param_traits<cl_device_info, CL_DEVICE_MAX_WORK_GROUP_SIZE> {
            enum { value = CL_DEVICE_MAX_WORK_GROUP_SIZE };
            using type = size_t;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type*>(str.data());
            }
        };
        
        /*  Context  */
        template<> struct param_traits<cl_context_info, CL_CONTEXT_REFERENCE_COUNT> {
            enum { value = CL_CONTEXT_REFERENCE_COUNT };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type*>(str.data());
            }
        };

        template<> struct param_traits<cl_context_info, CL_CONTEXT_NUM_DEVICES> {
            enum { value = CL_CONTEXT_NUM_DEVICES };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type*>(str.data());
            }
        };

        template<> struct param_traits<cl_context_info, CL_CONTEXT_DEVICES> {
            enum { value = CL_CONTEXT_DEVICES };
            using type = std::vector<cl_device_id>;

            static type CastToType(std::string &str) {
                size_t size = str.size();
                type v; v.reserve(size / sizeof(cl_device_id));
                for (size_t i = 0; i < size; i += sizeof(cl_device_id)) {
                    cl_device_id tmp = *reinterpret_cast<cl_device_id*>(&str[i]);
                    v.push_back(tmp);
                }
                return v;
            }
        };

        template<> struct param_traits<cl_context_info, CL_CONTEXT_PROPERTIES> {
            enum { value = CL_CONTEXT_PROPERTIES };
            using type = std::vector<cl_context_properties>;

            static type CastToType(std::string &str) {
                size_t size = str.size();
                type v; v.reserve(size / sizeof(cl_context_properties));
                for (size_t i = 0; i < size; i += sizeof(cl_context_properties)) {
                    cl_context_properties tmp = *reinterpret_cast<cl_context_properties*>(&str[i]);
                    v.push_back(tmp);
                }
                return v;
            }
        };
        
        /*  Program  */
        template<> struct param_traits<cl_program_info, CL_PROGRAM_REFERENCE_COUNT> {
            enum { value = CL_PROGRAM_REFERENCE_COUNT };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type*>(str.data());
            }
        };

        template<> struct param_traits<cl_program_info, CL_PROGRAM_CONTEXT> {
            enum { value = CL_PROGRAM_CONTEXT };
            using type = cl_context;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<cl_context*>(str.data());
            }
        };

        template<> struct param_traits<cl_program_info, CL_PROGRAM_NUM_DEVICES> {
            enum { value = CL_PROGRAM_NUM_DEVICES };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type*>(str.data());
            }
        };

        template<> struct param_traits<cl_program_info, CL_PROGRAM_DEVICES> {
            enum { value = CL_PROGRAM_DEVICES };
            using type = std::vector<cl_device_id>;

            static type CastToType(std::string &str) {
                size_t size = str.size();
                type v; v.reserve(size / sizeof(cl_device_id));
                for (size_t i = 0; i < size; i += sizeof(cl_device_id)) {
                    cl_device_id tmp = *reinterpret_cast<cl_device_id*>(&str[i]);
                    v.push_back(tmp);
                }
                return v;
            }
        };

        template<> struct param_traits<cl_program_info, CL_PROGRAM_SOURCE> {
            enum { value = CL_PROGRAM_SOURCE };
            using type = std::string;

            static type CastToType(std::string &str) {
                return str;
            }
        };
        
        template<> struct param_traits<cl_program_info, CL_PROGRAM_BINARY_SIZES> {
            enum { value = CL_PROGRAM_BINARY_SIZES };
            using type = std::vector<size_t>;

            static type CastToType(std::string &str) {
                size_t size = str.size();
                type v; v.reserve(size / sizeof(size_t));
                for (size_t i = 0; i < size; i += sizeof(size_t)) {
                    size_t tmp = *reinterpret_cast<size_t*>(&str[i]);
                    v.push_back(tmp);
                }
                return v;
            }
        };

        template<> struct param_traits<cl_program_info, CL_PROGRAM_NUM_KERNELS> {
            enum { value = CL_PROGRAM_NUM_KERNELS };
            using type = size_t;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type*>(str.data());
            }
        };

        template<> struct param_traits<cl_program_info, CL_PROGRAM_KERNEL_NAMES> {
            enum { value = CL_PROGRAM_KERNEL_NAMES };
            using type = std::string;

            static type CastToType(std::string &str) {
                return str;
            }
        };

        /*  Command Queue  */
        template<> struct param_traits<cl_command_queue_info, CL_QUEUE_CONTEXT> {
            enum { value = CL_QUEUE_CONTEXT };
            using type = cl_context;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<cl_context*>(str.data());
            }
        };

        template<> struct param_traits<cl_command_queue_info, CL_QUEUE_DEVICE> {
            enum { value = CL_QUEUE_DEVICE };
            using type = cl_device_id;

            static type CastToType(std::string &str) {
                auto tmp = *reinterpret_cast<cl_device_id*>(str.data());
                return tmp;
            }
        };

        template<> struct param_traits<cl_command_queue_info, CL_QUEUE_REFERENCE_COUNT> {
            enum { value = CL_QUEUE_REFERENCE_COUNT };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type *>(str.data());
            }
        };

        /*  Memory Object  */
        template<> struct param_traits<cl_mem_info, CL_MEM_TYPE> {
            enum { value = CL_MEM_TYPE };
            using type = cl_mem_object_type;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type *>(str.data());
            }
        };

        template<> struct param_traits<cl_mem_info, CL_MEM_FLAGS> {
            enum { value = CL_MEM_FLAGS };
            using type = cl_mem_flags;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type *>(str.data());
            }
        };

        template<> struct param_traits<cl_mem_info, CL_MEM_SIZE> {
            enum { value = CL_MEM_SIZE };
            using type = size_t;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type *>(str.data());
            }
        };

        template<> struct param_traits<cl_mem_info, CL_MEM_HOST_PTR> {
            enum { value = CL_MEM_HOST_PTR };
            using type = void*;

            static type CastToType(std::string &str) {
                return reinterpret_cast<type>(str.data());
            }
        };

        template<> struct param_traits<cl_mem_info, CL_MEM_MAP_COUNT> {
            enum { value = CL_MEM_MAP_COUNT };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type *>(str.data());
            }
        };

        template<> struct param_traits<cl_mem_info, CL_MEM_REFERENCE_COUNT> {
            enum { value = CL_MEM_REFERENCE_COUNT };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type *>(str.data());
            }
        };

        template<> struct param_traits<cl_mem_info, CL_MEM_CONTEXT> {
            enum { value = CL_MEM_CONTEXT };
            using type = cl_context;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<cl_context*>(&str[0]);
            }
        };

        /*  Kernel  */
        template<> struct param_traits<cl_kernel_info, CL_KERNEL_FUNCTION_NAME> {
            enum { value = CL_KERNEL_FUNCTION_NAME };
            using type = std::string;

            static type CastToType(std::string &str) {
                return str.data();
            }
        };

        template<> struct param_traits<cl_kernel_info, CL_KERNEL_NUM_ARGS> {
            enum { value = CL_KERNEL_NUM_ARGS };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type *>(str.data());
            }
        };

        template<> struct param_traits<cl_kernel_info, CL_KERNEL_REFERENCE_COUNT> {
            enum { value = CL_KERNEL_REFERENCE_COUNT };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type *>(str.data());
            }
        };

        template<> struct param_traits<cl_kernel_info, CL_KERNEL_CONTEXT> {
            enum { value = CL_KERNEL_CONTEXT };
            using type = cl_context;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<cl_context*>(str.data());
            }
        };

        template<> struct param_traits<cl_kernel_info, CL_KERNEL_PROGRAM> {
            enum { value = CL_KERNEL_PROGRAM };
            using type = cl_program;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<cl_program*>(str.data());
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
                return clRUN(clRetainDevice, device);
            }

            static cl_int Release(cl_device_id device) { 
                return clRUN(clReleaseDevice, device);
            }
        };

        template <> struct ReferenceHandler<cl_context> {
            static cl_int Retain(cl_context context) { 
                return clRUN(clRetainContext, context); 
            }
            static cl_int Release(cl_context context) { 
                return clRUN(clReleaseContext, context); 
            }
        };
        
        template <> struct ReferenceHandler<cl_program> {
            static cl_int Retain(cl_program program) {          
                return clRUN(clRetainProgram, program); 
            }
            static cl_int Release(cl_program program) {
                return clRUN(clReleaseProgram, program); 
            }
        };

        template <> struct ReferenceHandler<cl_command_queue> {
            static cl_int Retain(cl_command_queue command_queue) { 
                return clRUN(clRetainCommandQueue, command_queue); 
            }
            static cl_int Release(cl_command_queue command_queue) { 
                return clRUN(clReleaseCommandQueue, command_queue);
            }
        };

        template <> struct ReferenceHandler<cl_mem> {
            static cl_int Retain(cl_mem memory) { 
                return clRUN(clRetainMemObject, memory); 
            }
            static cl_int Release(cl_mem memory) { 
                return clRUN(clReleaseMemObject, memory); 
            }
        };

        template <> struct ReferenceHandler<cl_kernel> {
            static cl_int Retain(cl_kernel kernal) { 
                return clRUN(clRetainKernel, kernal); 
            }
            static cl_int Release(cl_kernel kernal) { 
                return clRUN(clReleaseKernel, kernal); 
            }
        };

        /*  Info Manager  */
        template <typename T> struct InfoManager {};

        template <> struct InfoManager<cl_platform_id> {
            template <cl_platform_info param_name>
            static typename param_traits<cl_platform_info, param_name>::type
            GetInfo(cl_platform_id platform) {
                size_t info_size = 0;
                clRUN(clGetPlatformInfo, platform, param_name, 0, nullptr, &info_size);

                std::string value(info_size, 0);
                clRUN(clGetPlatformInfo, platform, param_name, info_size, value.data(), nullptr);
                
                return param_traits<cl_platform_info, param_name>::CastToType(value);
            }
        };

        template <> struct InfoManager<cl_device_id> {
            template <cl_device_info param_name>
            static typename details::param_traits<cl_device_info, param_name>::type
            GetInfo(cl_device_id device) {
                size_t info_size = 0;
                clRUN(clGetDeviceInfo, device, param_name, 0, nullptr, &info_size);

                std::string value(info_size, 0);
                clRUN(clGetDeviceInfo, device, param_name, info_size, value.data(), nullptr);
                
                return param_traits<cl_device_info, param_name>::CastToType(value);
            }
        };

        template <> struct InfoManager<cl_context> {
            template <cl_context_info param_name>
            static typename details::param_traits<cl_context_info, param_name>::type
            GetInfo(cl_context context) {
                size_t info_size = 0;
                clRUN(clGetContextInfo, context, param_name, 0, nullptr, &info_size);

                std::string value(info_size, 0);
                clRUN(clGetContextInfo, context, param_name, info_size, value.data(), nullptr);
                
                return param_traits<cl_context_info, param_name>::CastToType(value);
            }
        };
        
        template <> struct InfoManager<cl_program> {
            template <cl_program_info param_name>
            static typename details::param_traits<cl_program_info, param_name>::type
            GetInfo(cl_program program) {
                size_t info_size = 0;
                clRUN(clGetProgramInfo, program, param_name, 0, nullptr, &info_size);

                std::string value(info_size, 0);
                clRUN(clGetProgramInfo, program, param_name, info_size, value.data(), nullptr);
                
                return param_traits<cl_program_info, param_name>::CastToType(value);
            }
        };

        template <> struct InfoManager<cl_command_queue> {
            template <cl_command_queue_info param_name>
            static typename details::param_traits<cl_command_queue_info, param_name>::type
            GetInfo(cl_command_queue command_queue) {
                size_t info_size = 0;
                clRUN(clGetCommandQueueInfo, command_queue, param_name, 0, nullptr, &info_size);

                std::string value(info_size, 0);
                clRUN(clGetCommandQueueInfo, command_queue, param_name, info_size, value.data(), nullptr);
                
                return param_traits<cl_command_queue_info, param_name>::CastToType(value);
            }
        };

        template <> struct InfoManager<cl_mem> {
            template <cl_mem_info param_name>
            static typename details::param_traits<cl_mem_info, param_name>::type
            GetInfo(cl_mem memory) {
                size_t info_size = 0;
                clRUN(clGetMemObjectInfo, memory, param_name, 0, nullptr, &info_size);

                std::string value(info_size, 0);
                clRUN(clGetMemObjectInfo, memory, param_name, info_size, value.data(), nullptr);
                
                return param_traits<cl_mem_info, param_name>::CastToType(value);
            }
        };

        template <> struct InfoManager<cl_kernel> {
            template <cl_kernel_info param_name>
            static typename details::param_traits<cl_kernel_info, param_name>::type
            GetInfo(cl_kernel kernel) {
                size_t info_size = 0;
                clRUN(clGetKernelInfo, kernel, param_name, 0, nullptr, &info_size);

                std::string value(info_size, 0);
                clRUN(clGetKernelInfo, kernel, param_name, info_size, value.data(), nullptr);
                
                return param_traits<cl_kernel_info, param_name>::CastToType(value);
            }
        };
    } // namespace details

    namespace details {
        /*  Wrapper  */
        template <typename cl_type> class Wrapper {
        public:
            Wrapper(cl_type obj = NULL) : obj_(obj) {}
            
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
} // namespace cl